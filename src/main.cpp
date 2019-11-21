#include <iostream>
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
using namespace std;

int main(int argc, char** argv) {
	{

		vector<string> tags;
		tags.push_back("Drama");
		tags.push_back("Crime");
		//Movie* movie(long id = 1, string name = "Titanic", int length = 150, vector<string>(tags));
		LengthRecommenderUser user1(string("Ruben"));
		cout << user1.getName() << "\naddress is: "<< &user1.getName() << endl;
		cout << "test of the new branch" << endl;
		
	}

	return 0;
}