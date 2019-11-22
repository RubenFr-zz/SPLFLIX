#include "../include/User.h"
#include <iostream>

//-------------User class--------------//

//Constructor
User::User(const std::string& name) : name(name) {}

//Destructor
User::~User() 
{
	for (std::vector<Watchable*>::const_iterator it = history.begin(); it != history.end(); it++) 
	{
		delete *it;
	}
}

//Copy Constructor
User::User(const User& other) : name(other.getName()) {
	for (std::vector<Watchable*>::const_iterator it = other.get_history().begin(); it != other.get_history().end(); it++) {
		history.push_back(*it);
	}
}

//Move Constructor
User::User(User&& other) : name(other.getName()), history(std::move(other.get_history())) 
{
}  


//Copy Assignment Operator
User& User::operator=(const User &other) {
	if (this != &other)
	{
		delete& name;
		std::string name = std::string(std::move(other.getName()));
		history = std::move(other.get_history());
	}
	return *this;
}
//Move Assigment Operator
User& User::operator=(User &&other){
	if (this != &other)
	{
		delete& name;
		std::string name = other.getName();
		history = other.get_history();
		delete& other.getName();
		
		//other.get_history().begin() = nullptr;		HOW TO DELETE A VECTOR<WATCHABLE*> ?
	}
	return *this;
}


std::string User::getName() const {
	return name;
}

std::vector<Watchable*> User::get_history() const {
	return history;
}


//---------------LengthRecommenderUser class---------------//

//Contructor
LengthRecommenderUser::LengthRecommenderUser(const std::string& name) : User(name){}  //Check if ok

Watchable* LengthRecommenderUser::getRecommendation(Session& s) {
	int sum = 0;
	for (std::vector<Watchable*>::const_iterator it = get_history().begin(); it != get_history().end(); ++it) {
		sum += (*it).getLength();// Problemmmm

	}
	return NULL;
}


//---------------RerunRecommenderUser class---------------//

//Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string& name) : User(name) {}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
	return NULL;
}

//---------------GenreRecommenderUser class---------------//

//Constructor
GenreRecommenderUser::GenreRecommenderUser(const std::string& name) : User(name) {}

Watchable* GenreRecommenderUser::getRecommendation(Session& s) { return NULL; }