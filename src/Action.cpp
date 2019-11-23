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
    getErrorMsg() = errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

//Getters---------------------------------
const std::unordered_map<std::string, Type> BaseAction::getStringToType() const { return StringToType; };
const std::unordered_map<Type, std::string> BaseAction::getTypeToString() const { return TypeToString; };

//-----------CreateUser class--------------

void CreateUser::act(Session &sess)
{
    std::vector<std::string> action = sess.getAction();
    std::string name;
    std::string type_str;
    User* user;

    if (action.size() != 3)
    {
        error("input not valid");
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
    std::string str = "CreateUser" + this->getStatus();
    return str;
}

//-----------DeleteUser class--------------
void DeleteUser::act(Session &sess) {}

std::string DeleteUser::toString() const {
    std::string str = "DeleteUser" + this->getStatus();
    return str;
}

//-----------DuplicateUser class--------------

void DuplicateUser::act(Session &sess) {}

std::string DuplicateUser::toString() const
{
    std::string str = "DuplicateUser" + this->getStatus();
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
    std::string str = "PrintContentList" + this->getStatus();
    return str;
}

//-----------PrintWatchHistory class--------------

void PrintWatchHistory::act(Session &sess) {
    User *active_user = sess.getActiveUser();
    std::vector<Watchable *> watch_history = active_user->get_history();

    std::cout << "Watch history for " << active_user->getName() << std::endl;

    for (auto it = watch_history.begin(); it != watch_history.end(); ++it) {
        Watchable *tmp = *it;
        std::cout << tmp->toString();
    }

    complete();
    return;
}

std::string PrintWatchHistory::toString() const {
    std::string str = "PrintWatchHistory" + this->getStatus();
    return str;
}


//-----------Watch class--------------

void Watch::act(Session &sess) {

}

std::string Watch::toString() const {
    return "";
}

//-----------PrintActionsLog class-----------
void PrintActionsLog::act(Session &sess) {

    std::vector<BaseAction *> log = sess.getActionsLog();
    for (auto it = log.begin(); it != log.end(); ++it) {
        BaseAction *action = *it;
        std::string str = action->toString();
        std::cout << str << std::endl;
    }

    complete();
    return;
}

std::string PrintActionsLog::toString() const {
    std::string str = "PrintActionsLog" + this->getStatus();
    return str;
}

//-----------Exit class--------------

void Exit::act(Session &sess) {}

std::string Exit::toString() const {
    std::string str = "PrintContentList" + this->getStatus();
    return str;
}
