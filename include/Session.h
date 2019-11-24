#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

#include "../include/Action.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"


enum ActionType // List of all the possible actions
{
    createuser_A, changeuser_A, deleteuser_A, dupuser_A, content_A, watchhist_A, watch_A, log_A, exit_A, null
}; //A as Action because some of the name was already used for variables or function

class User;
class Watchable;
class BaseAction;

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
    std::vector<Watchable*> getContent() const;
    std::vector<BaseAction*> getActionsLog() const;
    std::unordered_map<std::string, User*> getUserMap() const;
    User* getActiveUser() const;
    std::vector<std::string> getAction();
    std::unordered_map<std::string,ActionType> getStringToAction() const;

protected:
    std::vector<std::string> split(std::string action_in) const; // Function that split the array string into a vector of every word "action"

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

    std::string action_in; // Action typed in by the user
    std::vector<std::string> action; // Vector of strings that divide each word of action_in
    // (for example if [action_in = CreateUser Amir len] is entered [action = {action_in, Amir, len}]

    /// Map from strings to Action values
    const std::unordered_map<std::string, ActionType> StringToAction =
            {
                    { "createuser", ActionType::createuser_A },
                    { "changeuser", ActionType::changeuser_A },
                    { "deleteuser", ActionType::deleteuser_A },
                    { "dupuser", ActionType::dupuser_A },
                    { "content", ActionType::content_A },
                    { "watchhist", ActionType::watchhist_A },
                    { "watch", ActionType::watch_A },
                    { "log", ActionType::log_A },
                    { "exit", ActionType::exit_A }
            };

};
#endif