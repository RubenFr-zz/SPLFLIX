#include <iostream>
#include "../include/Session.h"
#include "../include/Action.h"


using namespace std;

int main(int argc, char** argv) {

//    const string path = "C:/Users/Ruben/source/repos/RubenFr/SPLFLIX/config2.json";
//    Session *test = new Session(path);
//    vector<Watchable*> content = test->getContent();
//
//    cout << endl << "List of all the content: " << endl << endl;
//    for (auto it = content.begin(); it != content.end(); ++it) {
//        Watchable* tmp = *it;
//        cout << tmp->toString();
//    }
    CreateUser tmp;
    string type_str = "";
    cin >> type_str;
    Type type = Type::null;
    if (tmp.getStringToType().count(type_str) > 0) { type = tmp.getStringToType().at(type_str); }

    switch (type)
    {
        case(Type::len):
            cout << "get into len" << endl;
            break;
        case(Type::rer):
            cout << "get into rer" << endl;
            break;
        case(Type::gen):
            cout << "get into gen" << endl;
            break;
        default:
            cout << "error" << endl;
    }
    return 0;
}