#include "Server.h"


Server::Server(string name){
    server_name = name;
    current_db = NULL;
    num_databases = 0;
}

void Server::create_db(string db_name){
    if(databases.count(db_name) > 0){
        error("database already exists on server");
        return;
    }
    Database *db = new Database(db_name);
    db->set_parent_server(this);
    databases.insert(pair<string, Database*>(db_name, db));
    num_databases++;
}

bool Server::use_db(string db_name){
    if(databases.count(db_name) == 0){
        error("database does not exist");
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
        error("cannot delete: database does not exist");
        return;
    }
    Database* db = databases.find(db_name)->second;
    databases.erase(db_name);
    num_databases--;
    delete db;
}
