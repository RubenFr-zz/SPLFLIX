#include "../include/User.h"

#include <utility>

//-------------User class--------------//

//Constructor
User::User(const std::string &name) : history(), name(name) {}

//Destructor
User::~User() {
    for (auto it : history) delete it;
    history.clear(); //Not sure if really need it
}

//Copy Constructor
User::User(const User &other) : history(), name(other.getName()) {
    auto otherHist = other.get_history();
    for (auto show : otherHist) {
        Watchable *toClone = show->clone();
        history.push_back(toClone);
    }
}

//Move Constructor
User::User(User &&other) : history(std::move(other.history)), name(other.name) {}

//Copy Assignment Operator
User &User::operator=(const User &other) {
    if (this != &other) {
        auto hist = other.history;
        delete this;

        for (auto show : hist) {
            Watchable *clone = show->clone();
            history.push_back(clone);
        }
        name = other.name;
    }
    return *this;
}

//Move Assigment Operator
User &User::operator=(User &&other) {
    if (this != &other) {
        delete this;
        name = other.name;
        history = std::move(other.get_history());
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
    name = std::move(other);
}

void User::watched(Watchable &content) {
    Watchable *toClone = content.clone();
    history.push_back(toClone);
}

bool User::nextEpisode(Session &s) {
    Watchable *watched = history.back();
    if (dynamic_cast<Episode *>(watched) != nullptr) {
        // If the element the user just watched is not the last content and the next content is in the same serie
        if (s.getContent().back()->getID() != watched->getID() &&
            s.getContent().at(watched->getID())->getName() == watched->getName()) {
            return true;
        }
    }
    return false;
}

Watchable *User::find(const std::vector<Watchable *> &vector, const unsigned int toFind) {
    for (auto it : vector) {
        if (it->getID() == toFind) return it;
    }
    return nullptr;
}


//---------------LengthRecommenderUser class---------------//

//Contructor
LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name) {}

//Destructor
LengthRecommenderUser::~LengthRecommenderUser() {}

//Copy Constructor
LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other) : User(other) {}

//Copy assignment operator
LengthRecommenderUser& LengthRecommenderUser::operator=(const LengthRecommenderUser& other)
{
    if (this != &other) {
        auto hist = other.history;
        delete this;
        modifName(other.getName());

        for (auto show : hist) {
            Watchable *clone = show->clone();
            history.push_back(clone);
        }

    }
    return *this;
}

//Move constructor
LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser&& other) : User(other) {}

//Move assignment operator
LengthRecommenderUser& LengthRecommenderUser::operator=(LengthRecommenderUser&& other)
{
    if (this != &other) {
        delete this;
        modifName(other.getName());
        history = std::move(other.get_history());
    }
    return *this;
}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {

    Watchable *recommendation = nullptr; // The recommendation

    //If there is a next episode return it
    if (nextEpisode(s)) return s.getContent().at(history.back()->getID());

    int avgLen = getAvgLen();
    if (avgLen == -1) return recommendation;

    std::vector<Watchable *> content = s.getContent(); //vector of every content available
    int min = -1;

    for (auto show : content) //iterate over all the content available
    {
        Watchable *found = find(history, show->getID()); // Is that show is in the history?
        if (found == nullptr) {
            int tmp = std::abs(avgLen - show->getLength());
            if ((min == -1) || (tmp < min)) {
                recommendation = show;
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

User *LengthRecommenderUser::clone() {
    User *toClone = new LengthRecommenderUser(*this);
    return toClone;
}

//---------------RerunRecommenderUser class---------------//

//Contructor
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name), cycle(-1) {}

//Destructor
RerunRecommenderUser::~RerunRecommenderUser() {}

//Copy Constructor
RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser &other) : User(other), cycle(other.cycle) {}

//Copy assignment operator
RerunRecommenderUser& RerunRecommenderUser::operator=(const RerunRecommenderUser& other)
{
    if (this != &other) {
        auto hist = other.history;
        delete this;
        modifName(other.getName());

        for (auto show : hist) {
            Watchable *clone = show->clone();
            history.push_back(clone);
        }

    }
    return *this;
}

//Move constructor
RerunRecommenderUser::RerunRecommenderUser(RerunRecommenderUser&& other) : User(other), cycle(other.cycle) {}

//Move assignment operator
RerunRecommenderUser& RerunRecommenderUser::operator=(RerunRecommenderUser&& other)
{
    if (this != &other) {
        delete this;
        modifName(other.getName());
        history = std::move(other.get_history());
    }
    return *this;
}

Watchable *RerunRecommenderUser::getRecommendation(Session &s) {

    //If there is a next episode return it
    if (nextEpisode(s)) return s.getContent().at(history.back()->getID());

    Watchable *recommendation = nullptr;
    if (history.empty()) return recommendation;
    ++cycle %= history.size();
    recommendation = history.at(cycle);
    return recommendation;

}

User *RerunRecommenderUser::clone() {
    User *toClone = new RerunRecommenderUser(*this);
    return toClone;
}


//---------------GenreRecommenderUser class---------------//

//Contructor
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name) {}

//Destructor
GenreRecommenderUser::~GenreRecommenderUser() {}

//Copy Constructor
GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser &other) : User(other) {}

//Copy assignment operator
GenreRecommenderUser& GenreRecommenderUser::operator=(const GenreRecommenderUser& other)
{
    if (this != &other) {
        auto hist = other.history;
        delete this;
        modifName(other.getName());

        for (auto show : hist) {
            Watchable *clone = show->clone();
            history.push_back(clone);
        }

    }
    return *this;
}

//Move constructor
GenreRecommenderUser::GenreRecommenderUser(GenreRecommenderUser&& other) : User(other) {}

//Move assignment operator
GenreRecommenderUser& GenreRecommenderUser::operator=(GenreRecommenderUser&& other)
{
    if (this != &other) {
        delete this;
        modifName(other.getName());
        history = std::move(other.get_history());
    }
    return *this;
}


Watchable *GenreRecommenderUser::getRecommendation(Session &s) {

    //If there is a next episode return it
    if (nextEpisode(s)) return s.getContent().at(history.back()->getID());

    std::vector<Watchable *> content = s.getContent();

    //A map that initialy contains every tags that exists in content with recurence 0
    // The initLoveMap just go over the tags in content and if first time it sees then add to the map <"tag", 0>
    std::unordered_map<std::string, int> loveMap = initLoveMap(content);

    //Look at the history and for every tag +1 in the map (loveMap) previously built
    //Note that every tag from every content in history will already be in loveMap
    //Then we just need to iterate over every tag of every content and do +1 in the loveMap
    for (auto &it : history) { // Check every content in the history list
        std::vector<std::string> tags = it->getTags();
        for (const auto &tag : tags) loveMap[tag]++;
    }

    // Convert the loveMap (unordered_map) to loveVec (vector)
    std::vector<std::pair<std::string, int>> loveVec(loveMap.begin(), loveMap.end());

    // Sort the vector according to fame (the most watched tag = front, the least = back)
    // If two tags hase the same frequence then sort then alphabetically
    sort(loveVec.begin(), loveVec.end(), [](const std::pair<std::string, int> &p1,
                                            const std::pair<std::string, int> &p2) {
        if (p1.second == p2.second) return p1.first < p2.first;
        else return p1.second > p2.second;
    });


    // Now we will iterate from all over the loveVec from the most to the least famous tag until we find a content in content that has
    // a tag from the vector and that isn't in the history
    // Note that because we start from the most famous (loveVec.begin()) and we return the first show found we are sure it
    // is indeed the content with the most famous tag available because if not it would have already been chosen
    std::string bestGenre;
    for (const auto &genre : loveVec) { //We go through the vector of tags we built before
        bestGenre = genre.first;
        for (auto show : content) { // We go through all the content to find the next recommendation with the tag
            auto hist = find(history,
                             show->getID()); // Is that show is in the history ? return nullptr if not and the show if so
            if (hist == nullptr) { // Means that the user didnt watched that, good for us!
                std::vector<std::string> tags = show->getTags();
                // Is that show has the famous tag? return the iterator of the tag if found and tags.end() if not found
                auto tag = find(tags.begin(), tags.end(), bestGenre);
                if (tag != tags.end()) { // Means that this show has "bestGenre", good for us!
                    // That means we found a show in content that isn't in history and that contains the bestGenre we are at
                    return show; // The appropriate show
                }
            }
        }
    }
    return nullptr;
}

// Method that go over the tags in content and if first time it sees then add to the map <"tag", 0>
std::unordered_map<std::string, int> GenreRecommenderUser::initLoveMap(std::vector<Watchable *> &content) {
    std::unordered_map<std::string, int> loveMap;
    for (auto it = content.begin(); it != content.end(); ++it) {
        std::vector<std::string> tags = (*it)->getTags();
        for (auto it2 = tags.begin(); it2 != tags.end(); ++it2) {
            if (loveMap.count(*it2) == 0) loveMap.insert({*it2, 0});
        }
    }
    return loveMap;
}

User *GenreRecommenderUser::clone() {
    User *toClone = new GenreRecommenderUser(*this);
    return toClone;
}

