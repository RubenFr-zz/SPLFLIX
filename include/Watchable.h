#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include "../include/User.h"


class Session;

class Watchable {
public:
	Watchable(long id, int length, const std::vector<std::string>& tags);
	virtual ~Watchable() {}
	virtual std::string toString() const = 0;
    virtual std::string toStringShort() const = 0;
	virtual Watchable* getNextWatchable(Session&) const = 0;
	virtual std::string getName() const = 0; // Polymorphism !!
	virtual Watchable* clone() = 0; //visitor pattern


	//getters
	long getID() const;
	int getLength() const;
    std::vector<std::string> getTags() const;

private:
	const long id;
	int length;
	std::vector<std::string> tags;
};

class Movie : public Watchable {
public:
	Movie(long id, std::string& name, int length, std::vector<std::string>& tags);
	virtual ~Movie() {}
	virtual std::string toString() const;
    virtual std::string toStringShort() const;
	virtual Watchable* getNextWatchable(Session&) const;
	virtual std::string getName() const;
    virtual Watchable* clone();
private:
	std::string name;
};


class Episode : public Watchable {
public:
	Episode(long id, const std::string& seriesName, int length, int season, int episode, const std::vector<std::string>& tags);
	virtual ~Episode() {}
	virtual std::string toString() const;
    virtual std::string toStringShort() const;
	virtual Watchable* getNextWatchable(Session&) const;
	virtual std::string getName() const;
	int getSeason() const;
	int getEpisode() const;
    virtual Watchable* clone();
private:
	std::string seriesName;
	int season;
	int episode;
	long nextEpisodeId;
};

#endif