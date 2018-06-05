#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "functions.h"
#include "Server.h"
using namespace std;

string current_loc_string;
Server *server;
extern ifstream inputfile;

void run_server(string server_name){
    server = new Server(server_name);
    bool running = true;
    string input;
    current_loc_string = "MyDb:" + server->get_server_name();
    while(running){
      cout << current_loc_string << ": ";
     // getline(cin, input);
      if(inputfile.eof()){
          cout << endl;
          running = false;
          break;
      }
      getline(inputfile, input);
      if(cin.eof() || input == "exit"){
        cout << endl;
        running = false;
      }
      if(input.at(input.length()-1) == ';'){
          input = input.substr(0, input.length()-1);
          parse_server_command(input);
      }

    }
}

vector<string> command_to_vector(string command){
    vector<string> cmd;
    string word;
    stringstream ss(command);
    while(ss >> word){
        cmd.push_back(word);
    }
    return cmd;
}

void parse_server_command(string command){
    transform(command.begin(), command.end(), command.begin(), ::tolower);
    cout << command << endl;
    vector<string> cmd = command_to_vector(command);
    if(cmd.at(0) == "create" && cmd.at(1) == "database" && cmd.size()==3){
        server->create_db(cmd.at(2));
    }
    else if(cmd.at(0) == "show" && cmd.at(1) == "databases" && cmd.size()==2){
        server->list_dbs();
    }
    else if(cmd.at(0) == "use" && cmd.size()==2){
        if(server->use_db(cmd.at(1))){
            run_database(server->get_curr_db());
            current_loc_string = "MyDb:" + server->get_server_name();
        }
    }
    else if(cmd.at(0) == "drop" && cmd.at(1) == "database" && cmd.size()==3){
        server->delete_db(cmd.at(2));
    }
}
