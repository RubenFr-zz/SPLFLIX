#include "../include/Watchable.h"

using namespace std;
//------------Watchable class-------------

//Constructor
Watchable::Watchable(long id, int length, const vector<string> &tags) : id(id), length(length), tags(tags) {}

//Destructor
Watchable::~Watchable() {}

//Getters
long Watchable::getID() const { return id; }

int Watchable::getLength() const { return length; }

vector<string> Watchable::getTags() const { return tags; }


//-----------Movie class------------------
Movie::Movie(long id, string &name, int length, vector<string> &tags) :
        Watchable(id, length, tags), name(name) {}

string Movie::toString() const {
	string str = "";
	str += to_string(getID()) + ". ";
	str += getName() + " ";
	str += to_string(getLength()) + " minutes ";
	str += "[";
	for (vector<string>::iterator it = getTags().begin(); it != getTags().end(); ++it) {
		str += *it;
		if (*it != getTags().back()) { str += ", "; }

	}
	str += "]\n";

	return str;
}

Watchable *Movie::getNextWatchable(Session &) const {
    return nullptr;
}

string Movie::getName() const { return name; }

//-------------Episode class---------------
Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
                 const vector<string> &tags) :
        Watchable(id, length, tags), episode(episode), season(season), seriesName(seriesName) {
    nextEpisodeId = 0;

}

string Episode::toString() const {
    string str = "";
    str += to_string(getID()) + ". ";
    str += getSeriesName();
    str += " S" + getSeason();
    str += "E" + getEpisode();
    str += " " + getLength();
    str += " minutes ";
    str += "[";
    for (vector<string>::iterator it = getTags().begin(); it != getTags().end(); ++it) {
        str += *it;
        if (*it != getTags().back()) { str += ", "; }

    }
    str += "]\n";

    return str;
}

Watchable *Episode::getNextWatchable(Session &) const {
    return nullptr;
}

string Episode::getSeriesName() const { return seriesName; }

int Episode::getSeason() const { return season; }

int Episode::getEpisode() const { return episode; }

long Episode::getNextEpisodeId() { return nextEpisodeId; }