#include <iostream>
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

//		vector<string> tags;
//		tags.push_back("Drama");
//		tags.push_back("Crime");
//		Movie* movie(long id = 1, string name = "Titanic", int length = 150, vector<string>(tags));
//		LengthRecommenderUser user1(string("Ruben"));
//		cout << user1.getName() << "\naddress is: "<< &user1.getName() << endl;
//		cout << "amir" << endl;
//		cout << "hello to you" << endl;

    std::ifstream i("C:/Users/Ruben/CLionProjects/SPLFLIX/json_config.json");
    nlohmann::json j;
    i >> j;
    nlohmann::json movies = j["movies"];
    nlohmann::json series = j["tv_series"];


//    cout << "number of movies in json: " << movies.size() << endl;
//    cout << "name of the first movie: " << movies[0]["name"] << endl;
//    cout << "tags in first movie" << movies[0]["tags"] << endl;
//    cout << "number of tags in first movie: " << movies[0]["tags"].size() << endl;
//
    for (int i = 0; i < movies.size(); i++) {
        std::vector<std::string> tags;
        for (int j = 0; j < movies[i]["tags"].size(); j++) {
            tags.push_back(movies[i]["tags"][j]);
        }
        cout << "[";
        for (auto it = tags.begin(); it != tags.end(); ++it) {
            cout << *it;
            if (*it != tags.back()) { cout << ", "; }

        }
        cout << "]" << endl;
    }

    return 0;
}