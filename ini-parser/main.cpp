#include <iostream>
#include "ini-parser.h"


using namespace std;

int main() {
    IniFile infile;
    infile.load("main.ini");
    // infile.show();
    
    // const string & ip = infile["service"]["ip"];
    // cout << ip << endl;

    // infile["service"]["ip"] = "1.1.1.1";
    // const string & ip = infile["service"]["ip"];
    // cout << ip << endl;

    // cout << infile.has("service") << endl;

    // cout << infile.has("service", "ip") << endl;

    // infile.remove("service", "ip");
    // cout << infile.has("service", "ip") << endl;
    // infile.show();
    // infile.remove("student");
    // infile.show();

    // infile.remove("student");
    // infile.show();
    // infile.save("main.ini");

    // infile.clear();
    // infile.show();
}