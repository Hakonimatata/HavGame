#include "Entities.h"

Config::Config(){ // setter opp config mapet
    filesystem::path filepath{"MediaFiles\\configure_game.txt"};
    if (filesystem::exists(filepath)){
        ifstream ifs{filepath};
        string variable, value = "";
        while(getline(ifs, variable, ',')){
            getline(ifs, value);
            configMap.insert(make_pair(variable, value));
        }
    }
}

Bird::Bird(int x, int y, string image) : currentPosition{x,y}, image{image}{
    
    
    //Sjekker configMap:
    if (configMap.find("gravity") != configMap.end()){
        setG(stod(configMap.at("gravity")));
    }
    if (configMap.find("jumpPower") != configMap.end()){
        setJumpPower(stoi(configMap.at("jumpPower")));
    }
    if (configMap.find("moveSpeed") != configMap.end()){
        setMoveSpeed(stoi(configMap.at("moveSpeed")));
    }
    if (configMap.find("birdHeight") != configMap.end()){
        setHeight(stoi(configMap.at("birdHeight")));
    }
    if (configMap.find("birdWidth") != configMap.end()){
        setWidth(stoi(configMap.at("birdWidth")));
    }
    if (configMap.find("Win_W") != configMap.end()){
        mapWidth = stoi(configMap.at("Win_W"));
    }
    if (configMap.find("Win_H") != configMap.end()){
        mapHeight = stoi(configMap.at("Win_H"));
    }
}

void Bird::jump(){
    //sett ny v0 osv 
    if(!dead){
        setVelocity(jumpPower);
        playSound("MediaFiles\\jump_sound.wav");
    }
};

void Bird::crash(){
    pushImpulse({-1,1}, 20.0);
    if(!dead){
        playSound("MediaFiles\\death_sound.wav");
    }
    dead = true; 
}

void Bird::incrementScore(){
    playSound("MediaFiles\\score_sound.wav");
    score++;
    if(score % 10 == 0){
        playSound("MediaFiles\\score_sound_milestone.wav");
    }
}

void Bird::fall(){
    
    if (currentPosition.y < mapHeight){ //begrense rendering
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

Obsticle::Obsticle(int Win_W, int Win_H) : topLeftPos{static_cast<double>(Win_W), 0} 
{
    //genererer tilfeldig gappos
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<int> distribution(20, Win_H-pipeHeight-20);
    topLeftGapPos = distribution(generator);

    //sjekker configMap
    if (configMap.find("pipeWidth") != configMap.end()){
        setPipeWidth(stoi(configMap.at("pipeWidth")));
    }
    if (configMap.find("pipeHeight") != configMap.end()){
        setPipeHeight(stoi(configMap.at("pipeHeight")));
    }
    if (configMap.find("pipeSpeed") != configMap.end()){
        setPipeSpeed(stoi(configMap.at("pipeSpeed")));
    }
    if (configMap.find("obsticleSpace") != configMap.end()){
        setObsticleSpace(stoi(configMap.at("obsticleSpace")));
    }
}


void Bird::pushImpulse(FloatPoint direction, double velocity){
    direction.normalize();
    bounceDirection = direction;
    bounceVelocity = velocity;
}

void Bird::push(){
    if(abs(bounceVelocity) > 0.05){
        currentPosition.x += bounceDirection.x * bounceVelocity;
        currentPosition.y += bounceDirection.y * bounceVelocity;
        bounceVelocity -= bounceVelocity * friction;
    }
}
