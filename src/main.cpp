#include <iostream>
#include "../include/Session.h"
#include "../include/Action.h"


using namespace std;

int main(int argc, char** argv) {

//    Tester for reader of Json file
    const string path = "C:/Users/Ruben/source/repos/RubenFr/SPLFLIX/config1.json";
    Session *test = new Session(path);
    vector<Watchable*> content = test->getContent();

    cout << endl << "List of all the content: " << endl << endl;
    for (auto it = content.begin(); it != content.end(); ++it) {
        Watchable* tmp = *it;
        cout << tmp->toString();
    }

// Tester for the switch case with enum
//    CreateUser tmp;
//    string type_str = "";
//    cin >> type_str;
//    Type type = Type::null;
//    if (tmp.getStringToType().count(type_str) > 0) { type = tmp.getStringToType().at(type_str); }
//
//    switch (type)
//    {
//        case(Type::len):
//            cout << "get into len" << endl;
//            break;
//        case(Type::rer):
//            cout << "get into rer" << endl;
//            break;
//        case(Type::gen):
//            cout << "get into gen" << endl;
//            break;
//        case(Type::null):
//            cout << "error !!" << endl;
//    }

// Tester for function Session::split()
//    Session tmp;
//    vector<string> array = tmp.split("Just a small test to check if that works");
//    for(auto it = array.begin(); it != array.end(); ++it){
//        cout << *it << endl;
//    }

// Tester for Session
//    Session *sess = new Session("C:/Users/Ruben/source/repos/RubenFr/SPLFLIX/config2.json");
//    sess->start();
    return 0;
}