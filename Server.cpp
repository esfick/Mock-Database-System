#include "Server.h"

Server::Server(string name){
    server_name = name;
    current_db = NULL;
    num_databases = 0;
}

void Server::create_db(string db_name){
    if(databases.count(db_name) > 0){
        cout << "Error: database already exists on server" << endl;
        return;
    }
    Database *db = new Database(db_name);
    db->set_parent_server(this);
    databases.insert(pair<string, Database*>(db_name, db));
    num_databases++;
}

bool Server::use_db(string db_name){
    if(databases.count(db_name) == 0){
        cout << "Error: database does not exist" << endl;
        return false;
    }
    current_db = databases.find(db_name)->second;
    return true;
}

void Server::list_dbs(){
    cout << "Databases (" << num_databases << " total):" << endl;
    for(map<string, Database*>::iterator it = databases.begin(); it != databases.end(); ++it){
        cout << it->first << endl;
    }
}

void Server::delete_db(string db_name){
    if(databases.count(db_name) == 0){
        cout << "Error: cannot delete: database does not exist" << endl;
        return;
    }
    Database* db = databases.find(db_name)->second;
    databases.erase(db_name);
    num_databases--;
    delete db;
}
