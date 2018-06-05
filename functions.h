#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Server.h"


/*
    Header definitions for all functions
*/

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
bool table_exists(string tname);

//run_sql.cpp
void parse_sql_select(string command);
void parse_sql_delete(vector<string> cmd);
void parse_sql_select_distinct(string cname, string tname);
void parse_sql_insert(vector<string> cmd, string command);
void parse_sql_update(vector<string> cmd);
void parse_sql_create_index(string command, vector<string> cmd);
bool validate_insert_string(string insert, string &fields, string& vals);
vector<string> list_to_vector(string list);
bool attributes_match(vector<string> fields, vector<string> vals);
bool validate_attribute_values(vector<string> fields, vector<string> vals, string &err);

#endif
