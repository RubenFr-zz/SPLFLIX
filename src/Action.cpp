#include "../include/Action.h"

//-----------BaseAction class--------------

BaseAction::BaseAction() : status(ActionStatus::PENDING) {
    errorMsg = "";
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(const std::string &errorMsg) {
    status = ActionStatus::ERROR;
    changeErrorMsg(errorMsg);
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::changeErrorMsg(const std::string& msg)
{
    errorMsg = msg;
}

//Getters---------------------------------
const std::unordered_map<std::string, Type> BaseAction::getStringToType() const { return StringToType; };
//const std::unordered_map<Type, std::string> BaseAction::getTypeToString() const { return TypeToString; };
const std::unordered_map<ActionStatus, std::string> BaseAction::getStatusToString() const { return StatusToString; }

//-----------CreateUser class--------------

void CreateUser::act(Session &sess)
{
    std::vector<std::string> action = sess.getAction(); //get the action entered
    std::string name; // name of the new user to be
    std::string type_str; // Recommendation picked by the user
    User* user; // Create a new user yet to determine what type it will be of

    if (action.size() != 3)
    {
        error("Input not valid to create a User");  // To create a user 3 words must me entered !
    }else{
        name = action[1];
        type_str = action[2];
        Type type = Type::null;
        if (getStringToType().count(type_str) > 0) { type = getStringToType().at(type_str); }
		if (sess.getUserMap().count(name) > 0) { error("A user with the same name already exists"); return; }
        switch (type)
        {
            case(Type::len):
                user = new LengthRecommenderUser(name);
                sess.addUser(*user);
                complete();
                break;
            case(Type::rer):
                user = new RerunRecommenderUser(name);
                sess.addUser(*user);
                complete();
                break;
            case(Type::gen):
                user = new GenreRecommenderUser(name);
                sess.addUser(*user);
                complete();
                break;
            case(Type::null):
                error("Incorrect user type");
                break;
        }
    }
	return;
}

std::string CreateUser::toString() const
{
    std::string str = "CreateUser " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------ChangeActiveUser class--------------

void ChangeActiveUser::act(Session &sess)
{
	std::unordered_map<std::string, User*> users = sess.getUserMap();
    std::vector<std::string> action = sess.getAction();
    std::string name;

    if (action.size() != 2)
    {
        error("Input not valid to change the active User");  // To create a user 2 words must me entered !
    }else{
        name = action[1];
        if (users.count(name) > 0) 
		{  
			sess.changeActiveUser(*users.at(name));
			complete();
		}
		else {
			error("no user with the name: " + name);
		}
    }

}

std::string ChangeActiveUser::toString() const
{
    std::string str = "ChangeActiveUser " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------DeleteUser class--------------
void DeleteUser::act(Session &sess)
{
    std::vector<std::string> action = sess.getAction();
    std::unordered_map<std::string, User*> users = sess.getUserMap();
    if (action.size() != 2)
    {
        error("Input not valid to delete a User");
    }else{
        std::string toDelete = action[1];
        if (users.count(toDelete) > 0) {
            if ( users.at(toDelete) == sess.getActiveUser()) { error("Cannot delete the active user");
            }
            else {
                sess.deleteUser(toDelete);
                complete();
            }
        }else {
            error("The user doesn't exist");
        }
    }
}

std::string DeleteUser::toString() const {
    std::string str = "DeleteUser " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------DuplicateUser class--------------

void DuplicateUser::act(Session &sess)
{
    std::vector<std::string> action = sess.getAction();
    std::unordered_map<std::string, User*> users = sess.getUserMap();
    if (action.size() != 3)
    {
        error("Input not valid to duplicate a User");
    }
    else{
        std::string toCopy = action[1];
        std::string newUserName = action[2];
        if (users.count(newUserName) > 0)
        {
            error("The user name is already taken");
        }
        else if (users.count(toCopy) == 0)
        {
            error("The user doesn't exist");
        }
        else
        {
            User* newUser(users.at(toCopy));
            newUser->modifName(newUserName);
            sess.addUser(*newUser);
            complete();
        }
    }
}

std::string DuplicateUser::toString() const
{
    std::string str = "DuplicateUser " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------PrintContentList class--------------

void PrintContentList::act(Session &sess) {
    std::vector<Watchable *> content = sess.getContent();

    for (auto it = content.begin(); it != content.end(); ++it) {
        Watchable *tmp = *it;
        std::cout << tmp->toString();
    }
    complete();
    return;
}

std::string PrintContentList::toString() const {
    std::string str = "PrintContentList " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}

//-----------PrintWatchHistory class--------------

void PrintWatchHistory::act(Session &sess) {
    if (sess.getActiveUser() == nullptr) { error("No active User"); return; }
    User *active_user = sess.getActiveUser();
    std::vector<Watchable *> watch_history = active_user->get_history();

    std::cout << "Watch history for: " << active_user->getName() << std::endl;

    if (!watch_history.empty()) {
        long id = 1;
        for (std::vector<Watchable *>::iterator it = watch_history.begin(); it != watch_history.end(); ++it) {
            Watchable *tmp = *it;
            std::cout << std::to_string(id++) +". "+ tmp->toStringShort() << std::endl;
        }
        complete();
    }else { error("The User has no history"); }

    return;
}

std::string PrintWatchHistory::toString() const {
    std::string str = "PrintWatchHistory " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}


//-----------Watch class--------------

void Watch::act(Session &sess) {

    if (!sess.getActiveUser()) { error("No active User"); return; }  //tmp is NULL

    std::vector<Watchable*> listOfContent = sess.getContent();
    std::vector<std::string> action = sess.getAction();

    if (action.size() != 2)
    {
        error("Input not valid to watch a content");
    }else{
        long id = std::stoi(action[1]); //Convert a string to an Int
        if ( id > listOfContent.size())
        {
            error("The content doesn't exist");
        }else{
            Watchable* content = listOfContent[id-1];
            sess.getActiveUser()->watched(*content);
            complete();
        }
    }

}

std::string Watch::toString() const {
    std::string str = "Watch " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------PrintActionsLog class-----------
void PrintActionsLog::act(Session &sess) {

    std::vector<BaseAction*> log = sess.getActionsLog();
    for (std::vector<BaseAction*>::iterator it = log.begin(); it != log.end(); ++it) {
        BaseAction *action = *it;
        std::string str = action->toString();
        std::cout << str << std::endl;
    }
    complete();
    return;
}

std::string PrintActionsLog::toString() const {
    std::string str = "PrintActionsLog " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------Exit class--------------

void Exit::act(Session &sess) {}

std::string Exit::toString() const {
    std::string str = "Exit " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;;
}
