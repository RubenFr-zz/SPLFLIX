#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

#include "../include/Action.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"


class User;
class Watchable;

class Session {
public:
	Session(const std::string& configFilePath); //Constructor
	~Session();	//Destructor
	Session(const Session& other); //Copy constructor
	Session& operator=(Session& other); //Copy assignment operator
	Session(Session&& other); //Move constructor
	Session& operator=(Session&& other); //Move assignment operator

	void start();

	//getters
	std::vector<Watchable*> getContent();// Available watching content 
	std::vector<BaseAction*> getActionsLog();// History of all actions
	std::unordered_map<std::string, User*> getUserMap();// List of users
	User* getActiveUser();// Currently active user

private:
	std::vector<Watchable*> content;
	std::vector<BaseAction*> actionsLog;
	std::unordered_map<std::string, User*> userMap;
	User* activeUser;
};
#endif