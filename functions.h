//OPEN HEADER GUARD
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "Server.h"

using namespace std;

//run_server.cpp
void parse_server_command(string command);
void run_server(string server_name);
vector<string> command_to_vector(string command);

//run_database.cpp
void run_database(Database* db);
void parse_db_command(string command);
void alter_table(vector<string> cmd);
void toggle_unique(string attr_name, bool unique);
void toggle_primary_key(string attr_name, bool iskey);
void toggle_nullable(string attr_name, bool n);
void create_table_from_init_list(string command);
bool validate_table_init(string init, string &tname, string& list);
bool add_table_field(string field_init);

//run_sql.cpp

void parse_sql_select(vector<string> cmd);
void parse_sql_select_distinct(string cname, string tname);
void parse_sql_insert(vector<string> cmd);


#endif
