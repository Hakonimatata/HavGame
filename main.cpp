#include "std_lib_facilities.h"
#include "GameWindow.h"

int Win_W = 1920;
int Win_H = 1080;
void configWindow();

int main() {
    configWindow();
    GameWindow G{Win_W, Win_H};
    G.run();

    return 0;
}

void configWindow(){ // kan fikse dette!!
    map<string, string> configMap{};
    filesystem::path filepath{"configure_game.txt"};
    if (filesystem::exists(filepath)){
        ifstream ifs{filepath};
        string variable, value = "";
        while(getline(ifs, variable, ',')){
            getline(ifs, value);
            configMap.insert(make_pair(variable, value));
        }
    }

    if (configMap.find("Win_W") != configMap.end()){
        Win_W = stoi(configMap.at("Win_W"));
    }
    if (configMap.find("Win_H") != configMap.end()){
        Win_H = stoi(configMap.at("Win_H"));
    }
}