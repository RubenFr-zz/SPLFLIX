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
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


//-----------CreateUser class--------------

void CreateUser::act(Session &sess)
{

}

std::string CreateUser::toString() const {
    return "";
}

//-----------DeleteUser class--------------
void DeleteUser::act(Session &sess) {}

std::string DeleteUser::toString() const {
    return "";
}

//-----------DuplicateUser class--------------

void DuplicateUser::act(Session &sess) {}

std::string DuplicateUser::toString() const {
    return "";
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
    return "";
}
