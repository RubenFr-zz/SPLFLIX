#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include "../include/Watchable.h"
#include "../include/Session.h"

class Watchable;
class Session;

class User {
public:
//	CONSTRUCTORS
	User(const std::string& name);
	virtual ~User();
	User(const User& other);
//	User& operator=(const User& other);
//	User(User&& other);
//	User& operator=(User&& other);

	virtual Watchable* getRecommendation(Session& s) = 0;
	virtual User* clone() = 0;
	std::string getName() const;
	std::vector<Watchable*> get_history() const;
	void modifName(std::string other);
	void watched(Watchable& content);

    template<class InputIterator, class T>
    InputIterator find(InputIterator first, InputIterator last, const T& val)
    {
        while (first!=last) {
            if (*first==val) return first;
            ++first;
        }
        return last;
    }

    Watchable* find(std::vector<Watchable*> vector, const int toFind);

protected:
    bool nextEpisode(Session& s) ;
	std::vector<Watchable*> history;
private:
	std::string name;
	long id;

};


class LengthRecommenderUser : public User {
public:
	LengthRecommenderUser(const std::string& name);
//	LengthRecommenderUser(const LengthRecommenderUser& other);
	virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
private:
    int getAvgLen() const;
};

class RerunRecommenderUser : public User {
public:
	RerunRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
private:
    int cycle;
};

class GenreRecommenderUser : public User {
public:
	GenreRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
private:
    std::unordered_map<std::string, int> initLoveMap(std::vector<Watchable *> &content) const;
};

#endif