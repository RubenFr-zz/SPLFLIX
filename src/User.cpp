#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
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
User::User(User&& other) : name(other.getName()), history(std::move(other.get_history())){
	//doesnt work, we need to earse the other pointer: other.get_history() = nullptr;
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
		sum += (*it)->getLength();// Im not sure about that, need to check
	}
	int size = get_history().size();
	int avgLen = (int)std::round(sum / size);// To have an integer length
	Watchable* recommenedShow;// The recommendation

	// Create new vector
	std::vector<Watchable*>::const_iterator it2 = s.getContent().begin();
	std::vector< std::pair <Watchable*, int>> lenVec;
	while (it2 != s.getContent().end()) {// O(n^2) maybe better?
		std::vector<Watchable*>::iterator itv = std::find(get_history().begin(), get_history().end(), *it2);// Is that show is in the history?
		if (itv != get_history().end())// Means it appears in the history
			it2++;
		else// Means that the user didnt watched that
			lenVec.push_back(std::make_pair(*it2, std::abs(avgLen - (*it2)->getLength())));// Vector with pointer to length good shows and how close is their length to avgLen
	}

	int min = std::min_element((*lenVec.begin()).second, (*lenVec.end()).second);// Min dis
	std::vector< std::pair <Watchable*, int>>::const_iterator it3 = std::find_if(lenVec.begin(), lenVec.end(), (*it3).second==min);// find the right pair
	recommenedShow = (*it3).first;
	// We need to check if we need to clean memory here!!
	return recommenedShow;
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

Watchable* GenreRecommenderUser::getRecommendation(Session& s) {
	std::vector<std::pair<std::string, int>>loveVec;
	//Look at the history
	for (std::vector<Watchable*>::const_iterator it = get_history().begin(); it != get_history().end(); it++) {// Check every content in the history list
		//Check if the tag is already in the vector!!!!!!!!!
		for (std::vector<std::string>::const_iterator it2 = (*it)->getTags().begin(); it2 != (*it)->getTags().end(); it2++) {// Check every tag of each show
			std::vector<std::pair<std::string, int>>::const_iterator it3 = loveVec.begin();
			while (it3 != loveVec.end()) {
				if((*it3).first.compare((*it2))==1)// means we already have that tag in the loveVec

			}
	}
	return NULL;
}