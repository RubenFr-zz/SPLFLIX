#include "../include/Watchable.h"

//------------Watchable class-------------

//Constructor
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags(tags) {}

//Destructor
Watchable::~Watchable() {}

//getters
long Watchable::getID() const { return id; }

int Watchable::getLength() const { return length; }

std::vector<std::string> Watchable::getTags() const { return tags; }


//-----------Movie class------------------
Movie::Movie(long id, std::string &name, int length, std::vector<std::string> &tags) :
        Watchable(id, length, tags), name(name) {}

std::string Movie::toString() const
{
    std::string str = "";
    str += std::to_string(getID()) + ". ";
    str += getName();
    str += " " + std::to_string(getLength());
    str += " minutes ";
    str += "[";
    std::vector<std::string> tags = getTags();
    for (std::vector<std::string>::iterator it = tags.begin(); it != tags.end(); ++it)
    {
        str += *it;
        if (*it != tags.back()) { str += ", "; }
    }
    str += "]\n";

    return str;
}

Watchable *Movie::getNextWatchable(Session &) const {
    return nullptr;
}

std::string Movie::getName() const { return name; }

//-------------Episode class---------------
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :
        Watchable(id, length, tags), episode(episode), season(season), seriesName(seriesName) {
    nextEpisodeId = 0;

}

std::string Episode::toString() const {
    std::string str = "";
    str += std::to_string(getID()) + ". ";
    str += getSeriesName();
    str += " S" + std::to_string(getSeason());
    str += "E" + std::to_string(getEpisode());
    str += " " + std::to_string(getLength());
    str += " minutes ";
    str += "[";
    std::vector<std::string> tags = getTags();
    for (std::vector<std::string>::iterator it = tags.begin(); it != tags.end(); ++it)
    {
        str += *it;
        if (*it != tags.back()) { str += ", "; }
    }
    str += "]\n";

    return str;
}

Watchable *Episode::getNextWatchable(Session &) const {
    return nullptr;
}

std::string Episode::getSeriesName() const { return seriesName; }

int Episode::getSeason() const { return season; }

int Episode::getEpisode() const { return episode; }

long Episode::getNextEpisodeId() { return nextEpisodeId; }