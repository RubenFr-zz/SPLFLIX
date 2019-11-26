#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

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
	int getCycle() const;
	void setCycle(int value);
protected:
	std::vector<Watchable*> history;
private:
	const std::string name;
	int cycle=0;// For the Rerun recommendation
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