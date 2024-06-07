#include "std_lib_facilities.h"
#include "GameWindow.h"

int Win_W = 1920;
int Win_H = 1080;
void configWindow();
int setResolution(int &Win_W, int &Win_H);

int main() {
    configWindow();
    setResolution(Win_W, Win_H);
    GameWindow G{Win_W, Win_H};
    G.run();

    return 0;
}

void configWindow(){ // kan fikse dette!!
    map<string, string> configMap{};
    filesystem::path filepath{"MediaFiles\\configure_game.txt"};
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

int setResolution(int &Win_W, int &Win_H){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Get the current display mode of the primary display (display index 0)
    SDL_DisplayMode DM;
    if (SDL_GetCurrentDisplayMode(0, &DM) != 0) {
        std::cerr << "SDL_GetCurrentDisplayMode Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Extract width and height
    Win_W = DM.w;
    Win_H = DM.h;

    // Print the width and height
    std::cout << "Screen width: " << Win_W << ", Screen height: " << Win_H << std::endl;

    // Clean up and quit SDL
    SDL_Quit();
    return 0;
}