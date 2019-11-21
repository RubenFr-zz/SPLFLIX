#include "../include/Action.h"

//-----------BaseAction class--------------

BaseAction::BaseAction() : status(ActionStatus::PENDING) 
{
	errorMsg = "";
}
ActionStatus BaseAction::getStatus() const {
	return status;
}
void BaseAction::complete() 
{
	status = ActionStatus::COMPLETED;
}

void BaseAction::error(const std::string& errorMsg) 
{
	status = ActionStatus::ERROR;
}

std::string BaseAction::getErrorMsg() const 
{
	return "";
}


//-----------CreateUser class--------------

void CreateUser::act(Session& sess) {}
std::string CreateUser::toString() const 
{
	return "";
}

//-----------DeleteUser class--------------
void DeleteUser::act(Session& sess) {}
std::string DeleteUser::toString() const 
{
	return "";
}

//-----------DuplicateUser class--------------

void DuplicateUser::act(Session& sess) {}
std::string DuplicateUser::toString() const 
{
	return "";
}

//-----------PrintContentList class--------------

void PrintContentList::act(Session& sess) {}
std::string PrintContentList::toString() const 
{
	return "";
}

//-----------PrintWatchHistory class--------------

void PrintWatchHistory::act(Session& sess) {}
std::string PrintWatchHistory::toString() const 
{
	return "";
}


//-----------Watch class--------------

void Watch::act(Session& sess) {}
std::string Watch::toString() const 
{
	return "";
}

//-----------PrintActionsLog class-----------
void PrintActionsLog::act(Session& sess) {}
std::string PrintActionsLog::toString() const 
{
	return "";
}

//-----------Exit class--------------

void Exit::act(Session& sess) {}
std::string Exit::toString() const 
{
	return "";
}
