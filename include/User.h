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
	User(const std::string& name);          //Constructor
	virtual ~User();                        //Destructor
	User(const User& other);                //Copy constructor
	User& operator=(const User& other);     // Copy assignment operator
	User(User&& other);                     //Move constructor
	User& operator=(User&& other);          //Move assignment operator

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

    Watchable* find(const std::vector<Watchable*>& vector, const unsigned int toFind);

protected:
    bool nextEpisode(Session& s) ;
	std::vector<Watchable*> history;
private:
	std::string name;
//	long id;

};


class LengthRecommenderUser : public User {
public:
	LengthRecommenderUser(const std::string& name);                             //Constructor
    virtual ~LengthRecommenderUser();                                           //Destructor
    LengthRecommenderUser(const LengthRecommenderUser& other);                  //Copy constructor
    LengthRecommenderUser& operator=(const LengthRecommenderUser& other);       //Copy assignment operator
    LengthRecommenderUser(LengthRecommenderUser&& other);                       //Move constructor
    LengthRecommenderUser& operator=(LengthRecommenderUser&& other);            //Move assignment operator

	virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
protected:
    int getAvgLen() const;
};

class RerunRecommenderUser : public User {
public:
	RerunRecommenderUser(const std::string& name);
	virtual ~RerunRecommenderUser();
	RerunRecommenderUser(const RerunRecommenderUser& other);
    RerunRecommenderUser& operator=(const RerunRecommenderUser& other);
    RerunRecommenderUser(RerunRecommenderUser&& other);
    RerunRecommenderUser& operator=(RerunRecommenderUser&& other);

	virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
private:
    int cycle;
};

class GenreRecommenderUser : public User {
public:
	GenreRecommenderUser(const std::string& name);
	virtual ~GenreRecommenderUser();
	GenreRecommenderUser(const GenreRecommenderUser& other);
    GenreRecommenderUser& operator=(const GenreRecommenderUser& other);
    GenreRecommenderUser(GenreRecommenderUser&& other);
    GenreRecommenderUser& operator=(GenreRecommenderUser&& other);

	virtual Watchable* getRecommendation(Session& s);
    virtual User* clone();
private:
    static std::unordered_map<std::string, int> initLoveMap(std::vector<Watchable *> &content) ;
};

#endif