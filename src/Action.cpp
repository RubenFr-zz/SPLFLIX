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

        switch (type)
        {
            case(Type::len):
                user = new LengthRecommenderUser(name);
                sess.getUserMap().insert({name,user});
                complete();
                break;
            case(Type::rer):
                user = new RerunRecommenderUser(name);
                sess.getUserMap().insert({name,user});
                complete();
                break;
            case(Type::gen):
                user = new GenreRecommenderUser(name);
                sess.getUserMap().insert({name,user});
                complete();
                break;
            case(Type::null):
                error("Incorrect user type");
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

//-----------DeleteUser class--------------
void DeleteUser::act(Session &sess)
{
    std::vector action = sess.getAction();
    std::unordered_map<std::string, User*> users = sess.getUserMap();
    if (action.size() != 2)
    {
        error("Input not valid to delete a User");
    }else{
        std::string toDelete = action[1];
        if (users.count(toDelete) > 0) {
            users.erase(toDelete);
            complete();
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

void DuplicateUser::act(Session &sess) {}

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
    User *active_user = sess.getActiveUser();
    std::vector<Watchable *> watch_history = active_user->get_history();

    std::cout << "Watch history for " << active_user->getName() << std::endl;

    if (!watch_history.empty()) {
        for (std::vector<Watchable *>::iterator it = watch_history.begin(); it != watch_history.end(); ++it) {
            Watchable *tmp = *it;
            std::cout << tmp->toString();
        }
        complete();
    }else { error("The User has no history..."); }
    return;
}

std::string PrintWatchHistory::toString() const {
    std::string str = "PrintWatchHistory " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " + getErrorMsg(); }
    return str;
}


//-----------Watch class--------------

void Watch::act(Session &sess) {

}

std::string Watch::toString() const {
    std::string str = "Watch " + getStatusToString().at(getStatus());
    if (getStatus() == ActionStatus::ERROR) { str += ": " +getErrorMsg(); }
    return str;
}

//-----------PrintActionsLog class-----------
void PrintActionsLog::act(Session &sess) {

    std::vector<BaseAction*> log = sess.getActionsLog();
    for (auto it = log.begin(); it != log.end(); ++it) {
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
