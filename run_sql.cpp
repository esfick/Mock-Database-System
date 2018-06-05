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
extern Database * database;
extern Table * table;
extern ifstream inputfile;


void parse_sql_select(string command){
    bool has_where_clause;
    size_t from_ind = command.find("from");
    size_t where_ind = command.find("where");
    if(from_ind == string::npos || from_ind < 9){
        error("invalid sql command");
        return;
    }
    string table_name;
    if(where_ind == string::npos){
        has_where_clause = false;
        table_name = command.substr(from_ind + 5, command.length() - from_ind - 5);
    }
    else {
        has_where_clause = true;
        table_name = command.substr(from_ind + 5, where_ind - from_ind - 6);
    }
    if(!table_exists(table_name)){
        return;
    }
    string from_attrs = command.substr(7, from_ind-8);
    algorithm::trim(from_attrs);
    vector<Attribute*> from_attrs_vect;
    vector<Row*> selected_rows;
    if(from_attrs == "*"){
        from_attrs_vect = table->attribs;
    }
    else {
        vector<string> from_attrs_string_vect = list_to_vector(from_attrs);
        for(string s: from_attrs_string_vect){
            Attribute *a = table->get_attribute(s);
            if(a == NULL){
                return;
            }
            from_attrs_vect.push_back(a);
        }
    }
    if(has_where_clause){
        // selected_rows specified
    }
    else {
        selected_rows = table->rows;
    }
    table->display(from_attrs_vect, selected_rows);
}

void parse_sql_select_distinct(string cname, string tname){
    table = database->get_table(tname);
    Attribute * a = table->get_attribute(cname);
    if(a == NULL){
        return;
    }
}

void parse_sql_delete(vector<string> cmd){
    Attribute * a = table->get_attribute(cmd.at(4));
    if(a == NULL){
        return;
    }
}

void parse_sql_create_index(string command, vector<string> cmd){
    size_t lpar = command.find("(");
    size_t rpar = command.find(")");
    if(lpar == string::npos || rpar == string::npos){
        error("must specify column(s) to create index on");
        return;
    }
    if(lpar > rpar){
        error("invalid sql command");
        return;
    }
    string index_name = cmd.at(2);
    vector<string> cols = list_to_vector(command.substr(lpar+1, rpar-lpar-1));
    for(string c: cols){
        //create index
        Attribute *a = table->get_attribute(c);
        if(a == NULL){
            return;
        }
        cout << c << endl;
    }
    cout << "index: " << index_name << ", cols: " << cols.size() << endl;
}

void parse_sql_insert(vector<string> cmd, string command){
    string fields;
    string vals;
    if(!validate_insert_string(command, fields, vals)){
        error("invalid syntax for insertion");
        return;
    }
    vector<string> field_vect = list_to_vector(fields);
    vector<string> val_vect = list_to_vector(vals);
    if(field_vect.size() != val_vect.size()){
        error("invalid syntax for insertion: fields and values do not match");
        return;
    }
    if(!attributes_match(field_vect, val_vect)){
        error("fields do not match those declared in table");
        return;
    }

    Row * row = new Row();
    row->set_attribs(table->attribs);
    string incorrect_attrs;
    if(!validate_attribute_values(field_vect, val_vect, incorrect_attrs)){
        error("invalid value for attribute(s)", incorrect_attrs);
        return;
    }
    row->set_attrib_vals(val_vect);
    for(int i = 0; i < field_vect.size(); i++){
        row->add_val_to_map(field_vect.at(i), val_vect.at(i));
    }
    if(!table->add_row(row)){
        return;
    }
}

void parse_sql_update(vector<string> cmd){

}

bool validate_insert_string(string insert, string &fields, string& vals){
    if(insert.at(insert.length() - 1) != ')'){
        return false;
    }
    size_t lpar = insert.find("(");
    size_t rpar = insert.find(")");
    size_t values = insert.find("values");
    if(lpar == string::npos || values == string::npos || rpar == string::npos){
        return false;
    }
    fields = insert.substr(lpar+1, rpar-lpar-1);
    insert = insert.substr(values, insert.length()-values);
    lpar = insert.find("(");
    rpar = insert.find(")");
    if(lpar == string::npos || rpar == string::npos){
        return false;
    }
    vals = insert.substr(lpar+1, rpar-lpar-1);
    return true;
}

vector<string> list_to_vector(string list){
    vector<string> list_vect;
    char_separator<char> sep(",");
    tokenizer<char_separator<char> > tok(list, sep);
    for(tokenizer<char_separator<char> >::iterator it = tok.begin(); it != tok.end(); ++it){
        string item = *it;
        algorithm::trim(item);
        list_vect.push_back(item);
    }
    return list_vect;
}

bool attributes_match(vector<string> fields, vector<string> vals){
    if(table == NULL){
        return false;
    }
    if(fields.size() > table->num_attributes){
        return false;
        //change later
    }
    for(int i = 0; i < fields.size(); i++){
        //change later
        if(fields.at(i) != table->attribs.at(i)->attr_name){
            return false;
        }
    }
    return true;
}

bool validate_attribute_values(vector<string> fields, vector<string> vals, string &err){
    err = "";
    //todo: check if each attr value matches attribute given
    //for now keeping all values as strings
    for(int i = 0; i < table->attribs.size(); i++){
        Attr_Type *at = table->attribs.at(i)->get_attr_type();
        if(at != NULL){
            bool valid = at->validate_attr_val(vals.at(i));
            if(!valid){
                err = table->attribs.at(i)->attr_name;
                return false;
            }
        }
    }
    return true;
}
