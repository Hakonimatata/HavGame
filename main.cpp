#include "std_lib_facilities.h"
#include "GameWindow.h"

int Win_W = 1920;
int Win_H = 1080;
void config();

int main() {
    config();
    GameWindow G{Win_W, Win_H};
    G.run();

    return 0;
}

void config(){
    map<string, string> configMap{};
    filesystem::path filepath{"konfigurering.txt"};
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