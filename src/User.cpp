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
	for (std::vector<Watchable*>::iterator it = history.begin(); it != history.end(); ++it)
	{
		delete *it;
	}
}

//Copy Constructor
User::User(const User& other) : name(other.getName()) {
	for (std::vector<Watchable*>::iterator it = other.get_history().begin(); it != other.get_history().end(); ++it) {
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
//		delete name;
		std::string name = other.getName();
		history = other.get_history();
//		delete other.getName();
		
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
	while (it2 != s.getContent().end()) {
		std::vector<Watchable*>::iterator itv = std::find(get_history().begin(), get_history().end(), *it2);// Is that show is in the history?
		if (itv == get_history().end())// Means that the user didnt watched that
			lenVec.push_back(std::make_pair(*it2, std::abs(avgLen - (*it2)->getLength())));// Vector with pointer to length good shows and how close is their length to avgLen
		it2++;
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
	//Look at the history and create vector of the most loved genres
	for (std::vector<Watchable*>::const_iterator it = get_history().begin(); it != get_history().end(); it++) {// Check every content in the history list
		//Check if the tag is already in the vector
		int flag = 0;
		for (std::vector<std::string>::const_iterator it2 = (*it)->getTags().begin(); it2 != (*it)->getTags().end(); it2++) {// Check every tag of each show
			std::vector<std::pair<std::string, int>>::iterator it3 = loveVec.begin();
			while (it3 != loveVec.end() && flag != 1) {
				if ((*it3).first.compare((*it2)) == 1) {// means we already have that tag in the loveVec
					(*it3).second++;
					flag = 1;
				}
				it3++;
			}
			if (it3 == loveVec.end() && flag == 0)// Means this is a new tag
				loveVec.push_back(std::make_pair((*it2), 1));// Adding the new tag to the loveVec
		}
	}
	// Now we will find which genre is most popular in the history and check if there are two or more loved genres
	int max = std::max_element((*loveVec.begin()).second, (*loveVec.end()).second);// The most watched genre
	std::vector<std::string> bestGenres;
	for (std::vector<std::pair<std::string, int>>::const_iterator itMost = loveVec.begin(); itMost != loveVec.end(); itMost++) {
		if ((*itMost).second == max)// Means is one of the most popular tags
			bestGenres.push_back((*itMost).first);// We took the name of the genre to a vector with the best genres
	}
	// Sorting "bestGenres" lexicographic
	std::string bestGenre = "";
	for (int i = 0; i < bestGenres.size(); i++) {// Number of popular tags
			for (int j = i + 1; j < bestGenres.size() + 1; j++)
			{
				// Positive values indicate that the second string comes first lexicographically
				if (bestGenres.at(i).compare(bestGenres.at(j)) > 0) {
					std::string tmp = bestGenres.at(j);// Sorting
					bestGenres.at(j) = bestGenres.at(i);
					bestGenres.at(i) = tmp;
				}
				else {
				// Negative values mean your string object comes first lexicographically.
				std::string tmp = bestGenres.at(i);// Sorting
				bestGenres.at(i) = bestGenres.at(j);
				bestGenres.at(j) = tmp;
				}
				// 0 indicates equality, but that cant be
			}
	}
	// Now we will look at the list of all availble content and pick one with the genre "bestGenre" that wasnt watched yet
	Watchable* recommendation = nullptr;
	int check = 0;
	std::vector<std::string>::iterator itM = bestGenres.begin();
	while (itM != bestGenres.end() && check != 1) {// In case we dont find the first most loved genre
		bestGenre = *itM;
		std::vector<Watchable*>::const_iterator itS = s.getContent().begin();
		while (itS != s.getContent().end() && check != 1) {// Check every show possible
			std::vector<Watchable*>::iterator itv = std::find_if(get_history().begin(), get_history().end(), *itS);// Is that show is in the history?
			if (itv == get_history().end()) {// Means that the user didnt watched that, good for us!
				std::vector<std::string>::iterator itP = std::find_if((*itS)->getTags().begin(), (*itS)->getTags().end(), bestGenre);// Is that show has the famous tag?
				if (itP != ((*itS)->getTags().end())) {// Means that this show has "bestGenre", good for us!
					recommendation = *itS;// The appropriate show
					check = 1;
				}
			}
			itS++;
		}
		itM++;
	}
	return recommendation;
}