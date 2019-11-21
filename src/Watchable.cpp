#include "../include/Watchable.h"

//------------Watchable class-------------

//Constructor
Watchable::Watchable(long id, int length, const std::vector<std::string>& tags) : id(id), length(length), tags(tags) {}

//Destructor
Watchable::~Watchable() {} 

//-----------Movie class------------------
Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags) : 
	Watchable(id, length, tags), name(name){}

std::string Movie::toString() const {
	return "";
}
Watchable* Movie::getNextWatchable(Session&) const {
	return nullptr;
}

//-------------Episode clas---------------
Episode::Episode(long id, const std::string& seriesName, int length, int season, int episode, const std::vector<std::string>& tags) :
	Watchable(id, length, tags), episode(episode), season(season), seriesName(seriesName) 
{
	nextEpisodeId = 0;

}

std::string Episode::toString() const {
	return "";
}

Watchable* Episode::getNextWatchable(Session&) const {
	return nullptr;
}