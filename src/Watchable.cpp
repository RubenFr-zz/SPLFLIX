#include "../include/Watchable.h"

//------------Watchable class-------------

//Constructor
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags(tags) {}


//getters
long Watchable::getID() const { return id; }

int Watchable::getLength() const { return length; }

std::vector<std::string> Watchable::getTags() const { return tags; }


//-----------Movie class------------------
Movie::Movie(long id, std::string &name, int length, std::vector<std::string> &tags) :
        Watchable(id, length, tags), name(name) {}

std::string Movie::toString() const
{
    std::string str;
    str += std::to_string(getID()) + ". ";
    str += name;
    str += " " + std::to_string(getLength());
    str += " minutes ";
    str += "[";
    std::vector<std::string> tags = getTags();
    for (auto it = tags.begin(); it != tags.end(); ++it)
    {
        str += *it;
        if (*it != tags.back()) { str += ", "; }
    }
    str += "]\n";

    return str;
}

std::string Movie::toStringShort() const {
    return name;
}

Watchable *Movie::getNextWatchable(Session &) const {
    return nullptr;
}

std::string Movie::getName() const { return name; }

Watchable *Movie::clone() {
    Watchable *toClone = new Movie(*this);
    return toClone;
}


//-------------Episode class---------------
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags) :
        Watchable(id, length, tags), seriesName(seriesName), season(season),  episode(episode), nextEpisodeId() {}

std::string Episode::toString() const {
    std::string str;
    str += std::to_string(getID()) + ". ";
    str += seriesName;
    str += " S";
    if (getSeason() < 10) str += "0";
    str += std::to_string(getSeason());
    str += "E";
    if (getEpisode() < 10) str += "0";
    str += std::to_string(getEpisode());
    str += " " + std::to_string(getLength());
    str += " minutes ";
    str += "[";
    std::vector<std::string> tags = getTags();
    for (auto it = tags.begin(); it != tags.end(); ++it)
    {
        str += *it;
        if (*it != tags.back()) { str += ", "; }
    }
    str += "]\n";

    return str;
}

std::string Episode::toStringShort() const {
    std::string str = seriesName + " S";
    if (season < 10) {str += "0"; }
    str += std::to_string(season) + "E";
    if (episode < 10) {str += "0" ;}
    str += std::to_string(episode);
    return str;
}

Watchable *Episode::getNextWatchable(Session &) const {
    return nullptr;
}

// Getters
std::string Episode::getName() const { return seriesName; }
int Episode::getSeason() const { return season; }
int Episode::getEpisode() const { return episode; }

Watchable *Episode::clone() {
    Watchable *toClone = new Episode(*this);
    return toClone;
}
