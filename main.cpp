#include <iostream>
#include <string>
#include <fstream>
#include "functions.h"
#include "Server.h"
using namespace std;


extern string current_loc_string;
extern Server *server;
ifstream inputfile;
//TO do: usage MyDb servername
//use bin file servername.bin
int main(const int argc, const char * argv[]){
    inputfile.open("test_input.txt");
    string server_name = "test";
    run_server(server_name);
}
