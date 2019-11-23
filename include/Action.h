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
	ActionStatus getStatus() const;
	virtual void act(Session& sess) = 0;
	virtual std::string toString() const = 0;
//protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;

	//Getters
    const std::unordered_map<std::string, Type> getStringToType() const;
    const std::unordered_map<Type, std::string> getTypeToString() const;

private:
	std::string errorMsg;
	ActionStatus status;

/// Map from strings to enum values
    const std::unordered_map<std::string, Type> StringToType =
            {
                    { "len", Type::len },
                    { "rer", Type::rer },
                    { "gen", Type::gen}
            };

/// Map from enum values to strings
    const std::unordered_map<Type, std::string> TypeToString =
            {
                    { Type::len, "len" },
                    { Type::rer, "rer" },
                    { Type::gen, "gen"}
            };
};

class CreateUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};
#endif