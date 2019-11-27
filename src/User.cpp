#include "../include/User.h"

//-------------User class--------------//

//Constructor
User::User(const std::string &name) : name(name) {
    id = 1;
}

//Destructor
User::~User() {
//    for(auto it : history) delete it; // Every content has already been destroyed
    history.clear();
}

//Copy Constructor
User::User(const User &other) : name(other.getName()) {
    for (std::vector<Watchable *>::iterator it = other.get_history().begin(); it != other.get_history().end(); ++it) {
        history.push_back(*it);
    }
}

//Move Constructor
User::User(User &&other) : name(other.getName()), history(std::move(other.get_history())) {
}


//Copy Assignment Operator
User &User::operator=(const User &other) {
    if (this != &other) {
        delete &name;
        std::string name = std::string(std::move(other.getName()));
        history = std::move(other.get_history());
    }
    return *this;
}

//Move Assigment Operator
User &User::operator=(User &&other) {
    if (this != &other) {
        std::string name = other.getName();
        history = other.get_history();
    }
    return *this;
}


std::string User::getName() const {
    return name;
}

std::vector<Watchable *> User::get_history() const {
    return history;
}

void User::modifName(std::string other) {
    name = other;
}

void User::watched(Watchable &content) {
    history.push_back(&content);
}

bool User::nextEpisode(Session &s) {
    Watchable *watched = history.back();
    if (dynamic_cast<Episode*>(watched) != nullptr) {
        // If the element the user just watched is not the last content and the next content is in the same serie
        if (s.getContent().back() != watched && s.getContent().at(watched->getID())->getName() == watched->getName()) {
            return true;
        }
    }
    return false;
}


//---------------LengthRecommenderUser class---------------//

//Contructor
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {

    Watchable *recommendation = nullptr; // The recommendation

    //If there is a next episode return it
    if (nextEpisode(s)) return s.getContent().at(history.back()->getID());

    int avgLen = getAvgLen();
    if (avgLen == -1) return recommendation;

    std::vector<Watchable *> content = s.getContent(); //vector of every content available
    int min = -1;

    for (auto it = content.begin(); it != content.end(); ++it) //iterate over all the content available
    {
        auto itv = find(history.begin(), history.end(), *it); // Is that show is in the history?
        if (itv == history.end()) {
            int tmp = std::abs(avgLen - (*it)->getLength());
            if ((min == -1) || (tmp < min)) {
                recommendation = *it;
                min = tmp;
            }
        }
    }
    return recommendation;
}

int LengthRecommenderUser::getAvgLen() const {
    int sum = 0;
    if (history.empty()) return -1;
    for (auto it = history.begin(); it != history.end(); ++it) {
		int len = (*it)->getLength();
        sum += len;
    }
    return (int) (sum / history.size());
}


//---------------RerunRecommenderUser class---------------//

//Constructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name) { cycle = -1; }

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {

    //If there is a next episode return it
    if (nextEpisode(s)) return s.getContent().at(history.back()->getID());

    Watchable *recommendation = nullptr;
    if (history.empty()) return recommendation;
    ++cycle %= history.size();
    recommendation = history.at(cycle);
    return recommendation;

}


//---------------GenreRecommenderUser class---------------//

//Constructor
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {}


Watchable *GenreRecommenderUser::getRecommendation(Session &s) {

    //If there is a next episode return it
    if (nextEpisode(s)) return s.getContent().at(history.back()->getID());

    std::vector<Watchable*> content = s.getContent();
    std::unordered_map<std::string, int> loveMap = initLoveMap(content);

    //Look at the history and create vector of the most loved genres
    for (auto it = history.begin(); it != history.end(); ++it) { // Check every content in the history list
        //Check if the tag is already in the vector
        std::vector<std::string> tags = (*it)->getTags();
        for (auto it2 = tags.begin(); it2 != tags.end(); ++it2) {
            if (loveMap.count(*it2) > 0) loveMap[*it2]++;
        }
    }
    std::vector<std::pair<std::string, int>> loveVec(loveMap.begin(), loveMap.end());
    std:sort(loveVec.begin(), loveVec.end(), [](const std::pair<std::string, int> &p1,
                                                const std::pair<std::string, int> &p2){
                                                if(p1.second == p2.second) return p1.first < p2.first;
                                                else return p1.second > p2.second;});


//    Now we will look at the list of all available content and pick one with the genre "bestGenre" that wasn't watched yet
    std::string bestGenre = "";
    for(auto genre = loveVec.begin(); genre != loveVec.end(); ++genre) {// In case we dont find the first most loved genre
        bestGenre = (*genre).first;
        for(auto show = content.begin(); show != content.end(); ++show) {// Check every show possible
            auto hist = find(history.begin(), history.end(), *show);// Is that show is in the history?
            if (hist == history.end()) {// Means that the user didnt watched that, good for us!
				std::vector<std::string> tags = (*show)->getTags();
				auto tag = std::find(tags.begin(), tags.end(), bestGenre);// Is that show has the famous tag?
                if (tag != tags.end()) { // Means that this show has "bestGenre", good for us!
                    return *show; // The appropriate show

                }
            }
        }
    }
    return nullptr;
}

std::unordered_map<std::string, int> GenreRecommenderUser::initLoveMap(std::vector<Watchable *> &content) const {
    std::unordered_map<std::string, int> loveMap;
    for (auto it = content.begin(); it != content.end(); ++it){
        std::vector<std::string> tags = (*it)->getTags();
        for (auto it2 = tags.begin(); it2 != tags.end(); ++it2) {
            if (loveMap.count(*it2) == 0) loveMap.insert({*it2, 0});
        }
    }
    return loveMap;
}

