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
struct Database;
struct Table;
struct Attribute;
template <typename T> struct Value_Type;

inline void error(string msg){
    fprintf(stderr, "Error: %s\n", msg.c_str());
}

inline void error(string msg, string msg2){
    fprintf(stderr, "Error: %s %s\n", msg.c_str(), msg2.c_str());
}


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

    bool validate_attr_val(string v){
        if(type == INVALID){
            return false;
        }
        if(type == INTEGER){
            return isInt(v);
        }
        else if(type == DOUBLE){
            size_t dot = v.find(".");
            if(dot == string::npos){
                return isInt(v);
            }
            if(count(v.begin(), v.end(), '.') > 1){
                return false;
            }
            string before = v.substr(0, dot);
            string after = v.substr(dot+1, v.length()-dot-1);
            return(isInt(before) && isInt(after));
        }
        else if(type == CHAR){
            if(v.length() > 30){
                return false;
            }
        }
        else { //boolean
            if(v != "true" && v != "false"){
                return false;
            }
        }
        return true;
    }

    bool isInt(string v){
        return (v.find_first_not_of("0123456789") == string::npos);
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

struct Attr_Value {
    string valstring;
    Type type;

    Attr_Value(string v, Type t){
        valstring = v;
        type = t;
    }

    string get_attr_val(){
        return valstring;
    }
};

template <typename T> struct Value: public Attr_Value {
    T val;

    Value(T v):val(v){}

};

struct Attribute {
    string attr_name;
    bool is_unique;
    Attr_Type *a_type;
    bool is_p_key;
    bool nullable;
    Table *parent_table;
    //int placeholder;

    Attribute(string a_name, string typ){
        attr_name = a_name;
        is_unique = false;
        a_type = new Attr_Type(typ);
        is_p_key = false;
        nullable = true;
    }

    string get_attr_name(){
        return attr_name;
    }

    void set_unique(bool u){
        is_unique = u;
    }

    Attr_Type *get_attr_type(){
        if(a_type->type == INVALID){
            error("invalid attribute type");
            return NULL;
        }
        else {
            return a_type;
        }
    }

    void set_pkey(bool k){
        is_p_key = k;
        if(k){
            is_unique = true;
        }
    }

    void set_nullable(bool n){
        nullable = n;
    }

    void set_parent_table(Table *t){
        parent_table = t;
    }

    Table * get_parent_table(){
        return parent_table;
    }



};

struct Row {
    Table* parent_table;
    int num_attributes;
    vector<Attribute*> attribs;
    map<string, string> attrib_to_val_map;
    Attribute * pkey;
    string pkey_val;
    vector<Attr_Value*> attrib_vals;
    //vector<string> attrib_vals;

    Table *get_parent_table(){
        return parent_table;
    }

    void set_parent_table(Table *t){
        parent_table = t;
    }

    void set_attribs(vector<Attribute*> a){
        attribs = a;
        num_attributes = a.size();
    }

    void set_attrib_vals(vector <string> vals){
        for(int i = 0; i < vals.size(); i++){
            add_val(vals.at(i), attribs.at(i)->a_type);
        }
    //    attrib_vals = vals;
    }

    void add_val(string val, Attr_Type *at){
        if(at->type == INTEGER){

        }
        else if(at->type == DOUBLE){

        }
        else if(at->type == BOOLEAN){

        }
        else if(at->type == CHAR){

        }
        Attr_Value* av = new Attr_Value(val, at->type);
        attrib_vals.push_back(av);

    }

    string get_pkey_val(){
        if(pkey != NULL){
            return attrib_to_val_map.find(pkey->get_attr_name())->second;
        }
        else {
            return NULL;
        }
    }

    void add_val_to_map(string attr_name, string val){
        attrib_to_val_map.insert(pair<string, string>(attr_name, val));
    }

};


struct Table {
    string table_name;
    vector<Attribute*> attribs;
    vector<Row*> rows;
    map<string, Attribute*> attr_map;
    map<string, Row*> row_pkey_map;
    int num_attributes;
    int num_rows;
    Attribute *primary_key;
    Database *parent_db;

    Table(string t_name){
        table_name = t_name;
        num_attributes = 0;
        num_rows = 0;
        primary_key = NULL;
    }

    void describe(){
        cout << "Table " << table_name << ", " << num_attributes << " columns, " << num_rows << " rows" << endl;
        cout << "Field\t\tType\t\t Unique\t\t Nullable " << endl;
        for(Attribute *a : attribs){
            cout << a->attr_name;
            if(a->is_p_key){
                cout << "(key)";
            }
            cout << "\t\t "<< a->a_type->type_to_string();
            cout << "\t\t" << a->is_unique;
            cout << "\t\t" << a->nullable <<endl;
        }
    }

    bool add_attribute(string attr_name, string attr_type){
        if(attr_map.count(attr_name) > 0){
            error("column already exists in table");
            return false;
        }
        Attr_Type *at = new Attr_Type(attr_type);
        if(at->type == INVALID){
            error("cannot create column: invalid type");
            delete at;
            return false;
        }
        delete at;
        Attribute *attr = new Attribute(attr_name, attr_type);
        attr->set_parent_table(this);
        attr_map.insert(pair<string, Attribute*>(attr_name, attr));
        attribs.push_back(attr);
        num_attributes++;
        return true;
    }

    bool add_row(Row* row){
        if(row == NULL){
            error("cannot add row");
            return false;
        }
        row->set_parent_table(this);
        row->pkey = primary_key;
        if(primary_key != NULL){
            row->pkey_val = row->attrib_to_val_map.find(primary_key->attr_name)->second;
        }
        rows.push_back(row);
        num_rows++;
        return true;
    }

    Attribute* get_attribute(string attr_name){
        if(attr_map.count(attr_name) == 0){
            error("column is not contained in table");
            return NULL;
        }
        return attr_map.find(attr_name)->second;
    }

    void change_attribute(string old_name, string new_name){
        if(attr_map.count(old_name) == 0){
            error("cannot change: column is not contained in table");
            return;
        }
        if(attr_map.count(new_name) > 0){
            error("cannot change: new column name already exists in table");
            return;
        }
        Attribute* a = attr_map.find(old_name)->second;
        attr_map.erase(old_name);
        attr_map.insert(pair<string, Attribute*>(new_name, a));
        a->attr_name = new_name;
    }

    void modify_attr_type(string attr_name, string new_t){
        if(attr_map.count(attr_name) == 0){
            error("cannot change type: column does not exist in table");
            return;
        }
        Attribute* a = attr_map.find(attr_name)->second;
        Type old_type = a->a_type->type;
        a->a_type->set_type(new_t);
        if(a->a_type->type == INVALID){
            error("cannot change type: invalid type");
            a->a_type->type = old_type;
            return;
        }
    }

    void delete_attribute(string attr_name){
        if(attr_map.count(attr_name) == 0){
            error("cannot delete: column does not exist in table");
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

    void set_parent_db(Database *d){
        parent_db = d;
    }

    Database * get_parent_db(){
        return parent_db;
    }

    void display(vector<Attribute*> a, vector<Row*> r){
        for(Attribute *attr : a){
            cout << attr->attr_name;
            if(attr->is_p_key){
                cout << " (key)";
            }
            cout << "\t\t";
        }
        cout << endl;
        for(Row * rw : r){
            for(Attribute *attr : a){
                string val = rw->attrib_to_val_map.find(attr->attr_name)->second;
                cout << val << "\t\t";
            }
            cout << endl;
        }
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
            error("table already exists in database");
            return;
        }
        Table *t = new Table(table_name);
        t->set_parent_db(this);
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
            error("table does not exist");
            return NULL;
        }
        return tables.find(t_name)->second;
    }

    void delete_table(string table_name){
        if(tables.count(table_name) == 0){
            error("cannot delete: table does not exist");
            return;
        }
        Table* t = tables.find(table_name)->second;
        tables.erase(table_name);
        delete t;
        num_tables--;
    }

};




class Server {

//run off file

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
