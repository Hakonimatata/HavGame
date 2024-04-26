#include "Entities.h"

Config::Config(){ // setter opp config mapet
    filesystem::path filepath{"configure_game.txt"};
    if (filesystem::exists(filepath)){
        ifstream ifs{filepath};
        string variable, value = "";
        while(getline(ifs, variable, ',')){
            getline(ifs, value);
            configMap.insert(make_pair(variable, value));
        }
    }
}

Bird::Bird(int x, int y, string image) : currentPosition{x,y}, image{image}{}

void Bird::jump(){
    //sett ny v0 osv 
    setVelocity(jumpPower);
};

void Bird::fall(){
    
    if (currentPosition.y < 4000){ //begrense rendering
        double velocity_y = getVelocity();
        double g = getG();
        double dt = getDt();

        velocity_y += g * dt;
        setVelocity(velocity_y);
        
        currentPosition.y -= velocity_y*dt;
    }
}

void Bird::moveRight(){
    currentPosition.x += moveSpeed;
}

void Bird::moveLeft(){
    currentPosition.x -= moveSpeed;
}