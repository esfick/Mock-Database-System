#ifndef SERVER_H
#define SERVER_H
#include <map>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

class Server;

enum Type { INTEGER, DOUBLE, CHAR, BOOLEAN, INVALID };

struct Attr_Type {
    Type type;

    Attr_Type(string a_type){
        set_type(a_type);
    }

    void set_type(string a_type){
        if(a_type == "integer" || a_type == "int"){
            type = INTEGER;
        }
        else if(a_type == "double"){
            type = DOUBLE;
        }
        else if(a_type == "char"){
            type = CHAR;
        }
        else if(a_type == "bool" ||a_type == "boolean"){
            type = BOOLEAN;
        }
        else {
            type = INVALID;
        }
    }

    string type_to_string(){
        switch(type){
            case INTEGER:
                return "Integer";
                break;
            case DOUBLE:
                return "Double";
                break;
            case CHAR:
                return "Char";
                break;
            case BOOLEAN:
                return "Boolean";
                break;
            case INVALID:
                return "Invalid";
                break;
        }
        return "Invalid";
    }
};

struct Attribute {
    string attr_name;
    bool is_unique;
    Attr_Type *a_type;
    bool is_p_key;
    //int placeholder;

    Attribute(string a_name, string typ){
        attr_name = a_name;
        is_unique = false;
        a_type = new Attr_Type(typ);
        is_p_key = false;
    }

    void set_unique(bool u){
        is_unique = u;
    }

    void set_pkey(bool k){
        is_p_key = k;
    }


};

struct Table {
    string table_name;
    vector<Attribute*> attribs;
    map<string, Attribute*> attr_map;
    int num_attributes;
    Attribute *primary_key;

    Table(string t_name){
        table_name = t_name;
        num_attributes = 0;
        primary_key = NULL;
    }

    void describe(){
        cout << "Table " << table_name << ", " << num_attributes << " columns" << endl;
        cout << "Field \t Type \t Unique \t" << endl;
        for(Attribute *a : attribs){
            cout << a->attr_name;
            if(a->is_p_key){
                cout << "(key)";
            }
            cout << "\t "<< a->a_type->type_to_string();
            cout << "\t" << a->is_unique << endl;
        }
    }

    void add_attribute(string attr_name, string attr_type){
        if(attr_map.count(attr_name) > 0){
            cout << "Error: column already exists in table" << endl;
            return;
        }
        Attr_Type *at = new Attr_Type(attr_type);
        if(at->type == INVALID){
            cout << "Error: cannot create column: invalid type" << endl;
            delete at;
            return;
        }
        delete at;
        Attribute *attr = new Attribute(attr_name, attr_type);
        attr_map.insert(pair<string, Attribute*>(attr_name, attr));
        attribs.push_back(attr);
        num_attributes++;
    }

    Attribute* get_attribute(string attr_name){
        if(attr_map.count(attr_name) == 0){
            cout << "Error: column does not exist in table" << endl;
            return NULL;
        }
        return attr_map.find(attr_name)->second;
    }

    void change_attribute(string old_name, string new_name){
        if(attr_map.count(old_name) == 0){
            cout << "Error: cannot change: column does not exist in table" << endl;
            return;
        }
        if(attr_map.count(new_name) > 0){
            cout << "Error: cannot change: new column name exists in table" << endl;
            return;
        }
        Attribute* a = attr_map.find(old_name)->second;
        attr_map.erase(old_name);
        attr_map.insert(pair<string, Attribute*>(new_name, a));
        a->attr_name = new_name;
    }

    void modify_attr_type(string attr_name, string new_t){
        if(attr_map.count(attr_name) == 0){
            cout << "Error: cannot change type: column does not exist in table" << endl;
            return;
        }
        Attribute* a = attr_map.find(attr_name)->second;
        Type old_type = a->a_type->type;
        a->a_type->set_type(new_t);
        if(a->a_type->type == INVALID){
            cout << "Error: cannot change column type: invalid type" << endl;
            a->a_type->type = old_type;
            return;
        }
    }

    void delete_attribute(string attr_name){
        if(attr_map.count(attr_name) == 0){
            cout << "Error: cannot delete: column does not exist in table" << endl;
            return;
        }
        Attribute* a = attr_map.find(attr_name)->second;
        attr_map.erase(attr_name);
        vector<Attribute*>::iterator pos = find(attribs.begin(), attribs.end(), a);
        if(pos != attribs.end()){
            attribs.erase(pos);
        }
        delete a;
        num_attributes--;
    }


};


struct Database {
    string db_name;
    map<string, Table*> tables;
    int num_tables;
    Server *parentSrver;

    Database(string name){
        db_name = name;
        num_tables = 0;
    }

    void set_parent_server(Server* ps){
        parentSrver = ps;
    }

    void create_table(string table_name){
        if(tables.count(table_name) > 0){
            cout << "Error: table already exists in database" << endl;
            return;
        }
        Table *t = new Table(table_name);
        tables.insert(pair<string, Table*>(table_name, t));
        num_tables++;
    }

    void list_tables(){
        cout << "Tables (" << num_tables << " total):" << endl;
        for(map<string, Table*>::iterator it = tables.begin(); it != tables.end(); ++it){
            cout << it->first << endl;
        }
    }

    Table* get_table(string t_name){
        if(tables.count(t_name) == 0){
            cout << "Error: table does not exist" << endl;
            return NULL;
        }
        return tables.find(t_name)->second;
    }

    void delete_table(string table_name){
        if(tables.count(table_name) == 0){
            cout << "Error: cannot delete: table does not exist" << endl;
            return;
        }
        Table* t = tables.find(table_name)->second;
        tables.erase(table_name);
        delete t;
        num_tables--;
    }

};




class Server {
private:
    string server_name;
    map<string, Database*> databases;
    Database *current_db;
    int num_databases;
public:
    Server(string name);

    void create_db(string db_name);

    bool use_db(string db_name);

    void delete_db(string db_name);

    void list_dbs();


    string get_server_name(){
        return server_name;
    }

    Database *get_curr_db(){
        return current_db;
    }


    int get_num_dbs(){
        return num_databases;
    }
};

#endif
