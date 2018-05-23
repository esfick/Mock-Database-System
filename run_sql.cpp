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


void parse_sql_select(vector<string> cmd){
    table = database->get_table(cmd.at(3));
    if(table == NULL){
        cout << "Error: table does not exist" << endl;
        return;
    }
    if(cmd.size() > 4){
        if(cmd.at(4) != "where"){
            cout << "Error: invalid sql command" << endl;
            return;
        }
    }
}

void parse_sql_select_distinct(string cname, string tname){
    table = database->get_table(tname);
    if(table == NULL){
        cout << "Error: table does not exist" << endl;
        return;
    }
    Attribute * a = table->get_attribute(cname);
    if(a == NULL){
        cout << "Error: column does not exist in table" << endl;
        return;
    }
}

void parse_sql_insert(vector<string> cmd){
    table = database->get_table(cmd.at(2));
    if(table == NULL){
        cout << "Error: table does not exist" << endl;
        return;
    }
}
