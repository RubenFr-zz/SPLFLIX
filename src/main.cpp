#include <iostream>
#include "../include/Session.h"


using namespace std;

int main(int argc, char** argv) {

    const string path = "C:/Users/Ruben/source/repos/RubenFr/SPLFLIX/config2.json";
    Session *test = new Session(path);
    vector<Watchable*> content = test->getContent();

    cout << endl << "List of all the content: " << endl << endl;
    for (auto it = content.begin(); it != content.end(); ++it) {
        Watchable* tmp = *it;
        cout << tmp->toString();
    }

    return 0;
}