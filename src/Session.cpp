#include "../include/Session.h"

//Constructor
Session::Session(const std::string &configFilePath) : content(), actionsLog(), userMap(), activeUser(), action_in(), action() {
    userMap.clear();
    action.clear();

    //read the json file
    std::ifstream stream(configFilePath);
    nlohmann::json json;
    stream >> json;
    nlohmann::json movies = json["movies"];
    nlohmann::json series = json["tv_series"];
    stream.close();

    long id = 1;

    for (auto & movie : movies) {
        std::vector<std::string> tags;
        std::string name = movie["name"];
        int length = movie["length"];

        tags.reserve(movie["tags"].size());
        for (auto & tag : movie["tags"]) {
            tags.push_back(tag);
        }

        Watchable *newMovie = new Movie(id, name, length, tags);
        content.push_back(newMovie);
        id++;
    }

    for (auto & show : series) {
        std::vector<std::string> tags;
        std::string name = show["name"];
        int length = show["episode_length"];

        tags.reserve(show["tags"].size());
        for (auto & tag : show["tags"]) {
            tags.push_back(tag);
        }
        for (unsigned int S = 0; S < show["seasons"].size(); S++) {
            for (unsigned int E = 1; E <= show["seasons"][S]; E++) {
                Watchable *episode = new Episode(id, name, length, S + 1, E, tags);
                content.push_back(episode);
                id++;
            }
        }
    }
}

//Destructor
Session::~Session() {
    for(auto it : content) delete it;
    content.clear();
    for(auto it : actionsLog) delete it;
    actionsLog.clear();

    std::vector<std::string> usersName;
    for(const auto& it : userMap) usersName.push_back(it.first);
    for (const std::string& name : usersName)
    {
        delete userMap.at(name);
        userMap.erase(name);
    }
}

//Copy constructor
Session::Session(const Session &other) :    content(), actionsLog(), userMap(), activeUser(),
                                            action_in(other.action_in), action(other.action),
                                            StringToAction(other.StringToAction) {
    std::vector<Watchable *> otherContent = other.content;
    for (auto show : otherContent) content.push_back(show->clone());
    std::vector<BaseAction *> otherActions = other.actionsLog;
    for (auto act : otherActions) actionsLog.push_back(act->clone());
    std::unordered_map<std::string,User*> otherUsers = other.userMap;
    for (auto user : otherUsers)
    {
        User* cloneUser = user.second->clone();
        userMap.insert({cloneUser->getName(), cloneUser});
    }
    activeUser = userMap.at(other.getActiveUser()->getName());
}

//Copy assignment operator
Session& Session::operator=(const Session &other) {
    if (this != &other){
        std::vector<Watchable *> otherContent = other.content;
        std::vector<BaseAction *> otherActions = other.actionsLog;
        std::unordered_map<std::string,User*> otherUsers = other.userMap;
        delete this;
        content = std::vector<Watchable*>();

        for (auto show : otherContent)
        {
            Watchable* clone = show->clone();
            content.push_back(clone);
        }
        for (auto act : otherActions) actionsLog.push_back(act->clone());
        for (auto user : otherUsers)
        {
            User* cloneUser = user.second->clone();
            userMap.insert({cloneUser->getName(), cloneUser});
        }
        activeUser = userMap.at(other.getActiveUser()->getName());
        action = other.action;
        action_in = other.action_in;
        StringToAction = other.StringToAction;
    }
    return *this;
}

//Move constructor
Session::Session(Session &&other) : content(std::move(other.content)), actionsLog(std::move(other.actionsLog)),
                                    userMap(std::move(other.userMap)), activeUser(std::move(other.activeUser)),
                                    action_in(std::move(other.action_in)), action(std::move(other.action)),
                                    StringToAction(std::move(other.StringToAction)) {}

//Move assignment operator
Session &Session::operator=(Session &&other) {
    if(this != &other)
    {
        delete this;

        content = std::move(other.content);
        actionsLog = std::move(other.actionsLog);
        userMap = std::move(other.userMap);
        activeUser = std::move(other.activeUser);
        action = std::move(other.action);
        action_in = std::move(other.action_in);
        StringToAction = std::move(other.StringToAction);
    }
    return *this;
}


// Methods

void Session::start() {

    bool run = true;
    std::cout << "\nSPLFLIX is now on!" << std::endl;

    std::string name_default = "default";
    if (userMap.count(name_default) == 0) {
        User *defaultUser = new LengthRecommenderUser(name_default);
        userMap.insert({name_default, defaultUser});
        activeUser = defaultUser;
    }

    while (run) {

        getline(std::cin, action_in); // same as cin except it read an entire line !
        action = split(action_in);

        if (action.empty()) {
            std::cout << "No action entered" << std::endl;
        } else {
            ActionType act = ActionType::null;
            if (StringToAction.count(action[0]) > 0) { act = StringToAction.at(action[0]); }
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
}

std::vector<Watchable *> Session::getContent() const { return content; }

std::vector<BaseAction *> Session::getActionsLog() const { return actionsLog; }

std::unordered_map<std::string, User *> Session::getUserMap() const { return userMap; }

User *Session::getActiveUser() const { return activeUser; }

std::vector<std::string> Session::getAction() const { return action; }

//std::string Session::getActionIn() const { return action_in; }

std::unordered_map<std::string, ActionType> Session::getStringToAction() const { return StringToAction; }

std::vector<std::string> Session::split(std::string string) const {
    std::istringstream ss(string);
    std::vector<std::string> results(std::istream_iterator<std::string>{ss},
                                     std::istream_iterator<std::string>());
    return results;
}

void Session::changeActiveUser(User &other) { activeUser = &other; }

void Session::addUser(User &user) {
    const std::string name = user.getName();
    userMap.insert({name, &user});
}

void Session::deleteUser(const std::string& toDelete) {
    delete userMap.at(toDelete);
    userMap.erase(toDelete);
}

void Session::changeMovie(ActionType type, long id) {
    if (type != ActionType::watch_A) return;
    action[1] = std::to_string(id);
}
