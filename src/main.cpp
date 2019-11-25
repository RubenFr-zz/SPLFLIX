#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){
//    cout << argc << endl;
//    cout << argv << endl;
//	if(argc!=2)
//	{
//		cout << "usage splflix input_file" << endl;
//		return 0;
//	}
	Session s("C:/Users/Ruben/source/repos/RubenFr/SPLFLIX/config1.json");
	s.start();

//	for(auto  it = s.getActionsLog().begin(); it != s.getActionsLog().end(); ++it){
//	    auto tmp = *it;
//	    cout << tmp->toString() << endl;
//	}
	return 0;
}