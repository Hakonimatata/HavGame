#pragma once
#include "std_lib_facilities.h"
#include "Point.h"
#include "Color.h"
#include "Image.h"
#include "sound&music.h"


struct Config{
    map<string, string> configMap;
    Config();
};

struct FloatPoint{
    double x;
    double y;
    void normalize(){
        double magnitude = sqrt(pow(x,2) + pow(y,2));
        x = x/magnitude;
        y = y/magnitude;
    }
};


class Physics {
    double velocity_y = 0.0;
    double g = -2;
    double dt = 0.5;
    

    public:
        void setVelocity(int v){velocity_y = v;} 
        double getVelocity(){return velocity_y;}
        double getG(){return g;}
        void setG(double d){ g = d;}
        double getDt(){return dt;}
};


class Bird : public Physics, public Image, public Config{
    
    Image image;
    Point currentPosition;
    int jumpPower = 20;
    int moveSpeed = 3;
    int birdHeight = 70;
    int birdWidth = 50;
    bool dead = false;
    Color color;
    int score = 0;

    //medlemsvariabler for bounce funksjonalitet:
    FloatPoint bounceDirection = {0.0, 0.0};
    double bounceVelocity = 0.0;
    double friction = 0.05;

    //out of bounds (oppdateres av config)
    int mapWidth = 1920;
    int mapHeight = 1080;


    bool falling = false; // hindrer at bird faller før man har trykket jump

    public:
        Bird(int x, int y, string image);

        void jump();
        void fall();
        void moveRight();
        void moveLeft();
        void crash();
        void incrementScore();
        void resetScore(){score = 0;}

        void pushImpulse(FloatPoint direction, double velocity);
        void push();


        //get- og setfunksjoner
        Bird* get(){return this;}
        void setJumpPower(int power){jumpPower = power;}
        Point getPosition(){return currentPosition;}
        int getBirdHeight(){return birdHeight;}
        int getBirdWidth(){return birdWidth;}
        void setColor(Color c){color = c;}
        Color getColor(){return color;}
        void setHeight(int h){birdHeight = h;}
        void setWidth(int w){birdWidth = w;}
        void setMoveSpeed(int s){moveSpeed = s;}

        bool hasCrashed(){return dead;}
        void revive(){dead = false;}
        void setCurrentPosition(Point p){currentPosition = p;};
        Image& getImage(){return image;}
        
        int getScore(){return score;}
        bool canFall(){return falling;}
        void setFallStateAtStart(bool v){falling = v;}
        void setBounceVelocity(double d){bounceVelocity = 0;}
};  



class Obsticle : public Image, public Config {
    FloatPoint topLeftPos;
    int topLeftGapPos;
    int pipeWidth = 30;
    int pipeHeight = 200;
    int pipeSpeed = 4;
    int obsticleSpace = 500;

    bool passed = false;

    public:
        Obsticle(int Win_W, int Win_H);

    Point getTopLeft(){return {static_cast<int>(topLeftPos.x),static_cast<int>(topLeftPos.y)};}
    int getPipeWidth(){return pipeWidth;}
    void setPipeWidth(int w){pipeWidth = w;}
    void setPipeHeight(int h){pipeHeight = h;}
    void setTopLeft(int pos){topLeftPos.x = pos;}
    int getGapPos(){return topLeftGapPos;}
    void moveObsticle(){topLeftPos.x-=pipeSpeed;}
    int getPipeHeight(){return pipeHeight;}
    int getObsticleSPace(){return obsticleSpace;}
    void setObsticleSpace(int space){obsticleSpace = space;}
    void setPipeSpeed(double speed){pipeSpeed = speed;}
    double getPipeSpeed(){return pipeSpeed;}

    void passedObsticle(){passed = true;}
    bool isPassedObsticle(){return passed;}
};