#include "../include/Action.h"

//-----------BaseAction class--------------

BaseAction::BaseAction() : errorMsg(), status(ActionStatus::PENDING) {}


ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(const std::string &Msg) {
    status = ActionStatus::ERROR;
    changeErrorMsg(Msg);
    std::cout << "Error - " + Msg << std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::changeErrorMsg(const std::string& msg)
{
    errorMsg = msg;
}

//Getters---------------------------------
std::unordered_map<std::string, Type> BaseAction::getStringToType() const { return StringToType; }
std::unordered_map<ActionStatus, std::string> BaseAction::getStatusToString() const { return StatusToString; }


//-----------CreateUser class--------------

void CreateUser::act(Session &sess)
{
    std::vector<std::string> action = sess.getAction(); //get the action entered
    std::string name; // name of the new user to be
    std::string type_str; // Recommendation picked by the user
    User *user; // Create a new user yet to determine what type it will be of

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
                error("recommendation algorithm is invalid");
                break;
        }
    }
}

std::string CreateUser::toString() const
{
    std::string str = "CreateUser " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

BaseAction *CreateUser::clone() {
    BaseAction *toClone = new CreateUser(*this);
    return toClone;
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
        if (users.count(name) <= 0)
            error("username does not exist");
        else {
            sess.changeActiveUser(*users.at(name));
            complete();
        }
    }
}

std::string ChangeActiveUser::toString() const
{
    std::string str = "ChangeActiveUser " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

BaseAction *ChangeActiveUser::clone() {
    BaseAction *toClone = new ChangeActiveUser(*this);
    return toClone;
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
            if ( users.at(toDelete) == sess.getActiveUser()) { error("username is already in use");
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

BaseAction *DeleteUser::clone() {
    BaseAction *toClone = new DeleteUser(*this);
    return toClone;
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
        std::string NameToCopy = action[1];
        std::string newUserName = action[2];
        if (users.count(newUserName) > 0)
        {
            error("The name is already taken");
        }
        else if (users.count(NameToCopy) == 0)
        {
            error("The user doesn't exist");
        }
        else
        {
            User* toCopy = users.at(NameToCopy);
            User* newUser = toCopy->clone();
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

BaseAction *DuplicateUser::clone() {
    BaseAction *toClone = new DuplicateUser(*this);
    return toClone;
}

//-----------PrintContentList class--------------

void PrintContentList::act(Session &sess) {
    std::vector<Watchable *> content = sess.getContent();

    for (auto tmp : content) {
        std::cout << tmp->toString();
    }
    complete();
}

std::string PrintContentList::toString() const {
    std::string str = "PrintContentList " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}

BaseAction *PrintContentList::clone() {
    BaseAction *toClone = new PrintContentList(*this);
    return toClone;
}

//-----------PrintWatchHistory class--------------

void PrintWatchHistory::act(Session &sess) {
    if (sess.getActiveUser() == nullptr) { error("No active User"); return; }
    User *active_user = sess.getActiveUser();
    std::vector<Watchable *> watch_history = active_user->get_history();


    if (!watch_history.empty()) {
        std::cout << "Watch history for: " << active_user->getName() << std::endl;
        long id = 1;
        for (auto tmp : watch_history) {
            std::cout << std::to_string(id++) + ". " + tmp->toStringShort() << std::endl;
        }
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    std::string str = "PrintWatchHistory " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}

BaseAction *PrintWatchHistory::clone() {
    BaseAction *toClone = new PrintWatchHistory(*this);
    return toClone;
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
        if ( (unsigned) id > listOfContent.size())
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

BaseAction *Watch::clone() {
    BaseAction *toClone = new Watch(*this);
    return toClone;
}

//-----------PrintActionsLog class-----------
void PrintActionsLog::act(Session &sess) {

    std::vector<BaseAction*> log = sess.getActionsLog();
    for (auto it = log.rbegin(); it != log.rend(); ++it) {
        BaseAction *action = *it;
        std::string str = action->toString();
        std::cout << str << std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    std::string str = "PrintActionsLog " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}

BaseAction *PrintActionsLog::clone() {
    BaseAction *toClone = new PrintActionsLog(*this);
    return toClone;
}

//-----------Exit class--------------

void Exit::act(Session &sess) { complete(); }

std::string Exit::toString() const {
    std::string str = "Exit " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}

BaseAction *Exit::clone() {
    BaseAction *toClone = new Exit(*this);
    return toClone;
}
