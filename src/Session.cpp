#include "../include/Session.h"

//Variables in the class:
//std::vector<Watchable*> content;
//std::vector<BaseAction*> actionsLog;
//std::unordered_map<std::string, User*> userMap;
//User* activeUser;

//Constructor
Session::Session(const std::string &configFilePath) {
    userMap.clear();
    activeUser = NULL;
    action_in = "";
    action.clear();

    //read the json file
    std::ifstream i(configFilePath);
    nlohmann::json j;
    i >> j;
    nlohmann::json movies = j["movies"];
    nlohmann::json series = j["tv_series"];

    long id = 1;

    for (int i = 0; i < movies.size(); i++) {
        std::vector<std::string> tags;
        std::string name = movies[i]["name"];
        int length = movies[i]["length"];

        for (int j = 0; j < movies[i]["tags"].size(); j++) {
            tags.push_back(movies[i]["tags"][j]);
        }

        Watchable *movie = new Movie(id, name, length, tags);
        content.push_back(movie);
        id++;
    }

    for (int i = 0; i < series.size(); i++) {
        std::vector<std::string> tags;
        std::string name = series[i]["name"];
        int length = series[i]["episode_length"];

        for (int j = 0; j < series[i]["tags"].size(); j++) {
            tags.push_back(series[i]["tags"][j]);
        }
        for (int S = 0; S < series[i]["seasons"].size(); S++) {
            for (int E = 1; E <= series[i]["seasons"][S]; E++) {
                Watchable *episode = new Episode(id, name, length, S + 1, E, tags);
                content.push_back(episode);
                id++;
            }
        }
    }
}

//Destructor
Session::~Session() {
//    delete activeUser;
    for(auto it : content) delete it;
    content.clear();
    for(auto it : actionsLog) delete it;
    actionsLog.clear();

    std::vector<std::string> usersName;
    for(auto it : userMap) usersName.push_back(it.first);
    for (std::string name : usersName)
    {
        delete userMap.at(name);
        userMap.erase(name);
    }
}

//Copy constructor
Session::Session(const Session &other) : content(other.getContent()), actionsLog(other.getActionsLog()),
                                         userMap(other.getUserMap()),
                                         activeUser(other.getActiveUser()) {}

//Copy assignment operator
Session &Session::operator=(Session &other) {
    return *this;
}

//Move constructor
Session::Session(Session &&other) {

}

//Move assignment operator
Session &Session::operator=(Session &&other) {
    getAction();
    return *this;
}

void Session::start() {

    bool run = true;
    std::cout << "\nSPLFLIX is now on!" << std::endl;

    std::string name_default = "default";
    User *defaultUser = new LengthRecommenderUser(name_default);
    userMap.insert({name_default, defaultUser});
    activeUser = defaultUser;

    while (run) {

		getline(std::cin, action_in); // same as cin except it read an entire line !
        action = split(action_in);

        if (action.empty()) {
            std::cout << "No action entered" << std::endl;
        } else {
            ActionType act = ActionType::null;
            if (getStringToAction().count(action[0]) > 0) { act = getStringToAction().at(action[0]); }
            BaseAction *baseAction;
            bool watching = false;

            switch (act) {
                case (createUser_A):
                    baseAction = new CreateUser();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (changeActiveUser_A):
                    baseAction = new ChangeActiveUser();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (deleteUser_A):
                    baseAction = new DeleteUser();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (duplicateUser_A):
                    baseAction = new DuplicateUser();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (watch_A):
                    watching = true;
                    while (watching) {
                        baseAction = new Watch();
                        baseAction->act(*this);
                        actionsLog.push_back(baseAction);
                        if (baseAction->getStatus() == ActionStatus::ERROR) { break; }
                        std::cout << "Watching " + activeUser->get_history().back()->toStringShort() << std::endl;
                        std::cout << "We recommend watching ";
                        Watchable* recommendation = activeUser->getRecommendation(*this);
                        if ( recommendation != nullptr) {
                            std::cout << recommendation->toStringShort() + ", continue watching? [y/n]" << std::endl;
                            std::string response;
                            getline(std::cin, response);
                            if (response == "n") { watching = false; }
                            else if (response == "y") { changeMovie(act, recommendation->getID()); }
                            else {
                                std::cout << "Non valid entry, watching aborted" << std::endl;
                                watching = false;
                            }
                        } else { std::cout << ": No content recommended" << std::endl; watching = false;}
                    }
                    break;
                case (content_A):
                    baseAction = new PrintContentList();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (watchHistory_A):
                    baseAction = new PrintWatchHistory();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (log_A):
                    baseAction = new PrintActionsLog();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    break;
                case (exit_A):
                    baseAction = new Exit();
                    baseAction->act(*this);
                    actionsLog.push_back(baseAction);
                    run = false;
                    break;
                case (null):
                    std::cout << "Action invalid" << std::endl;
            }
        }
    }
    return;
}


//getters
std::vector<Watchable *> Session::getContent() const { return content; }

std::vector<BaseAction *> Session::getActionsLog() const { return actionsLog; }

std::unordered_map<std::string, User *> Session::getUserMap() const { return userMap; }

User *Session::getActiveUser() const { return activeUser; }

std::vector<std::string> Session::getAction() { return action; }

std::unordered_map<std::string, ActionType> Session::getStringToAction() const { return StringToAction; }

std::vector<std::string> Session::split(std::string action_in) const {
    std::istringstream ss(action_in);
    std::vector<std::string> results(std::istream_iterator<std::string>{ss},
                                     std::istream_iterator<std::string>());
    return results;
}

void Session::changeActiveUser(User &other) { activeUser = &other; }

void Session::addUser(User &user) {
    const std::string name = user.getName();
    userMap.insert({name, &user});
}

void Session::deleteUser(std::string toDelete) {
    delete userMap.at(toDelete);
    userMap.erase(toDelete);
}

void Session::changeMovie(ActionType type, long id) {
    if (type != ActionType::watch_A) return;
    action[1] = std::to_string(id);
}
