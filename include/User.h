#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <typeinfo>
#include <unordered_set>
#include <unordered_map>

#include "../include/Watchable.h"

class Watchable;
class Session;

class User {
public:
	//User();
	User(const std::string& name);
	~User();
	User(const User& other);
	User& operator=(const User& other);
	User(User&& other);
	User& operator=(User&& other);

	virtual Watchable* getRecommendation(Session& s) = 0;
	std::string getName() const;
	std::vector<Watchable*> get_history() const;

	void modifName(std::string other);
	void watched(Watchable& content);
	int getCycle() const;
	void setCycle(int value);
protected:
	std::vector<Watchable*> history;
private:
	const std::string name;
	int cycle = 0;// For the Rerun recommendation
	std::string name;
	long id;

};


class LengthRecommenderUser : public User {
public:
	LengthRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
private:
};

class RerunRecommenderUser : public User {
public:
	RerunRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
private:
};

class GenreRecommenderUser : public User {
public:
	GenreRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
private:
};

#endif