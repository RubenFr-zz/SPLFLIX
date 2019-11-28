#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};

enum class Type{
    len, rer, gen, null
};

class BaseAction {
public:
	BaseAction();
	~BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess) = 0;
	virtual std::string toString() const = 0;
	virtual BaseAction* clone() = 0;

    std::unordered_map<std::string, Type> getStringToType() const;
    std::unordered_map<ActionStatus , std::string> getStatusToString() const;
protected:
	void complete();
	void error(const std::string& Msg);
	std::string getErrorMsg() const;
	void changeErrorMsg(const std::string& errorMsg);

private:
	std::string errorMsg;
	ActionStatus status;

/// Map from strings to Type values
    const std::unordered_map<std::string, Type> StringToType =
            {
                    { "len", Type::len },
                    { "rer", Type::rer },
                    { "gen", Type::gen}
            };

/// Map from ActionStatus values to strings
    const std::unordered_map<ActionStatus , std::string> StatusToString =
            {
                    { ActionStatus::PENDING, "PENDING" },
                    { ActionStatus::COMPLETED, "COMPLETED" },
                    { ActionStatus::ERROR, "ERROR"}
            };

};

class CreateUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintContentList : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* clone();
};
#endif