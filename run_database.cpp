#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include "functions.h"
#include "Server.h"
using namespace std;
using namespace boost;

extern string current_loc_string;
extern Server *server;
Database * database;
Table * table;
extern ifstream inputfile;

void run_database(Database *db){
    database = db;
    bool running = true;
    string input;
    current_loc_string = "MyDb:" + server->get_server_name() + "/" + database->db_name;
    while(running){
      cout << current_loc_string << ": ";
      //getline(cin, input);
      getline(inputfile, input);
      if(cin.eof() || input == "exit"){
        cout << endl;
        running = false;
        break;
      }
      if(input.at(input.length()-1) == ';'){
          input = input.substr(0, input.length()-1);
          parse_db_command(input);
      }
    }
}

void parse_db_command(string command){
    transform(command.begin(), command.end(), command.begin(), ::tolower);
    cout << command << endl;
    vector<string> cmd = command_to_vector(command);
    if(cmd.at(0) == "create" && cmd.at(1) == "table" && cmd.size()==3){
        database->create_table(cmd.at(2));
    }
    if(cmd.at(0) == "create" && cmd.at(1) == "table" && cmd.size() > 3){
        create_table_from_init_list(command);
    }
    else if(cmd.at(0) == "show" && cmd.at(1) == "tables" && cmd.size()==2){
        database->list_tables();
    }
    else if(cmd.at(0) == "drop" && cmd.at(1) == "table" && cmd.size()==3){
        database->delete_table(cmd.at(2));
    }
    else if(cmd.at(0) == "describe" && cmd.size()==2 && table_exists(cmd.at(1))){
        table->describe();
    }
    else if(cmd.at(0) == "describe" && cmd.size()==2){
        table = database->get_table(cmd.at(1));
        if(table != NULL){
            table->describe();
        }
    }
    else if(cmd.at(0) == "alter" && cmd.at(1) == "table" && cmd.size() > 3){
        if(table_exists(cmd.at(2))){
            alter_table(cmd);
        }
        /*table = database->get_table(cmd.at(2));
        if(table != NULL){
            alter_table(cmd);
        }*/
    }
    else if(cmd.at(0) == "select"){
        //sql
        parse_sql_select(command);
    }
    else if(cmd.at(0) == "select" && cmd.at(1) == "distinct" && cmd.at(3) == "from" && cmd.size() == 5 && table_exists(cmd.at(4))){
        //sql
        parse_sql_select_distinct(cmd.at(2), cmd.at(4));
    }
    else if(cmd.at(0) == "insert" && cmd.at(1) == "into" && cmd.size() >= 4 && table_exists(cmd.at(2))){
        parse_sql_insert(cmd, command);
    }
    else if(cmd.at(0) == "update" && cmd.at(2) == "set" && cmd.size() >= 5 && table_exists(cmd.at(1))){
        parse_sql_update(cmd);
    }
    else if(cmd.at(0) == "delete" && cmd.at(1) == "from" && cmd.at(3) == "where" && cmd.size() >= 7 && table_exists(cmd.at(2))){
        parse_sql_delete(cmd);
    }
    else if(cmd.at(0) == "create" && cmd.at(1) == "index" && cmd.at(3) == "on" && cmd.size() >= 7 && table_exists(cmd.at(4))){
        parse_sql_create_index(command, cmd);
    }
}

void alter_table(vector<string> cmd){
    if(cmd.at(3) == "add" && cmd.at(4) == "column" && cmd.size()==7){
        table->add_attribute(cmd.at(5), cmd.at(6));
    }
    else if(cmd.at(3) == "drop" && cmd.at(4) == "column" && cmd.size()==6){
        table->delete_attribute(cmd.at(5));
    }
    else if(cmd.at(3) == "change" && cmd.size()==6){
        table->change_attribute(cmd.at(4), cmd.at(5));
    }
    else if(cmd.at(3) == "modify" && cmd.size()==6){
        table->modify_attr_type(cmd.at(4), cmd.at(5));
    }
    else if(cmd.at(3) == "add" && cmd.at(4) == "unique" && cmd.size()==6){
        toggle_unique(cmd.at(5), true);
    }
    else if(cmd.at(3) == "drop" && cmd.at(4) == "unique" && cmd.size()==6){
        toggle_unique(cmd.at(5), false);
    }
    else if(cmd.at(3) == "set" && cmd.at(4) == "key" && cmd.size()==6){
        toggle_primary_key(cmd.at(5), true);
    }
    else if(cmd.at(3) == "unset" && cmd.at(4) == "key" && cmd.size()==6){
        toggle_primary_key(cmd.at(5), false);
    }
    else if(cmd.at(3) == "set" && cmd.at(4) == "nullable" && cmd.size()==6){
        toggle_nullable(cmd.at(5), true);
    }
    else if(cmd.at(3) == "unset" && cmd.at(4) == "nullable" && cmd.size()==6){
        toggle_nullable(cmd.at(5), false);
    }
}

void toggle_unique(string attr_name, bool unique){
    Attribute *a = table->get_attribute(attr_name);
    if(table != NULL && a != NULL){
        a->set_unique(unique);
    }
}

void toggle_primary_key(string attr_name, bool iskey){
    Attribute *a = table->get_attribute(attr_name);
    if(table != NULL && a != NULL){
        if(table->primary_key != NULL){
            table->primary_key->is_p_key = false;
        }
        table->primary_key = a;
        a->set_pkey(iskey);
    }
}

void toggle_nullable(string attr_name, bool n){
    Attribute *a = table->get_attribute(attr_name);
    if(table != NULL && a != NULL){
        a->set_nullable(n);
    }
}

void create_table_from_init_list(string command){
    string table_name;
    string list;
    if(!validate_table_init(command, table_name, list)){
        error("invalid table initialization");
        return;
    }
    database->create_table(table_name);
    if(!table_exists(table_name)){
        return;
    }
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tok(list, sep);
    for(tokenizer<char_separator<char> >::iterator it = tok.begin(); it != tok.end(); ++it){
        string field = *it;
        algorithm::trim(field);
        if(!add_table_field(field)){
            error("invalid table initialization");
            database->delete_table(table_name);
            return;
        }
    }
}

bool validate_table_init(string init, string &tname, string& list){
    if(init.at(init.length() - 1) != ')'){
        return false;
    }
    size_t lpar = init.find("(");
    if(lpar == string::npos || lpar == init.length()-2){
        return false;
    }
    tname = init.substr(13, lpar-13);
    list = init.substr(lpar+1, init.length()-lpar-2);
    algorithm::trim(tname);
    algorithm::trim(list);
    return true;
}

bool add_table_field(string field_init){
    vector<string> fieldvect = command_to_vector(field_init);
    if(fieldvect.size() < 2 || fieldvect.size() > 5){
        //error
        return false;
    }
    if(!(table->add_attribute(fieldvect.at(0), fieldvect.at(1)))){
        return false;
    }
    if(fieldvect.size() > 2){
        for(int i = 2; i < fieldvect.size(); i++){
            if(fieldvect.at(i) == "!null"){
                toggle_nullable(fieldvect.at(0), false);
            }
            else if(fieldvect.at(i) == "pkey"){
                toggle_primary_key(fieldvect.at(0), true);
            }
            else if(fieldvect.at(i) == "unique"){
                toggle_unique(fieldvect.at(0), true);
            }
            else {
                //error
                return false;
            }
        }
    }
    return true;
}

bool table_exists(string tname){
    table = database->get_table(tname);
    if(table == NULL){
        return false;
    }
    return true;
}
