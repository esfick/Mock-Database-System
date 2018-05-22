#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "functions.h"
#include "Server.h"
using namespace std;

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
    cout << command << endl;
    vector<string> cmd = command_to_vector(command);
    if(cmd.at(0) == "create" && cmd.at(1) == "table" && cmd.size()==3){
        database->create_table(cmd.at(2));
    }
    else if(cmd.at(0) == "show" && cmd.at(1) == "tables" && cmd.size()==2){
        database->list_tables();
    }
    else if(cmd.at(0) == "drop" && cmd.at(1) == "table" && cmd.size()==3){
        database->delete_table(cmd.at(2));
    }
    else if(cmd.at(0) == "describe" && cmd.size()==2){
        table = database->get_table(cmd.at(1));
        if(table != NULL){
            table->describe();
        }
    }
    else if(cmd.at(0) == "describe" && cmd.size()==2){
        table = database->get_table(cmd.at(1));
        if(table != NULL){
            table->describe();
        }
    }
    else if(cmd.at(0) == "alter" && cmd.at(1) == "table" && cmd.size() > 3){
        table = database->get_table(cmd.at(2));
        if(table != NULL){
            alter_table(cmd);
        }
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
    else if(cmd.at(3) == "drop" && cmd.at(4) == "index" && cmd.size()==6){
        toggle_unique(cmd.at(5), false);
    }
    else if(cmd.at(3) == "set" && cmd.at(4) == "key" && cmd.size()==6){
        toggle_primary_key(cmd.at(5), true);
    }
    else if(cmd.at(3) == "unset" && cmd.at(4) == "key" && cmd.size()==6){
        toggle_primary_key(cmd.at(5), false);
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
