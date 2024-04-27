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
    void normalize();
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

class Bird : public Physics, public Image, public Config {
private:
    // Medlemsvariabler
    Image image;
    Point currentPosition;
    int jumpPower = 20;
    int moveSpeed = 3;
    int birdHeight = 70;
    int birdWidth = 50;
    bool crashed = false;
    Color color;
    int score = 0;
    bool canFall = false;

    // Bounce-funksjonalitet
    FloatPoint bounceDirection = {0.0, 0.0};
    double bounceVelocity = 0.0;
    double friction = 0.05;

    // Out of bounds
    int mapWidth = 1920;
    int mapHeight = 1080;

public:
    // Konstruktør
    Bird(int x, int y, string image);

    // Funksjoner for bevegelse
    void jump();
    void fall();
    void moveRight();
    void moveLeft();
    void crash();

    // Funksjoner for poeng
    void incrementScore();
    void resetScore(){score = 0;};

    // Funksjoner for kollisjon og fall
    void pushImpulse(FloatPoint direction, double velocity);
    void push();
    bool isCrashed() const{return crashed;}
    void setCrashed(bool b){crashed = b;}
    bool getCanFall() const{return canFall;}

    // Get- og set-metoder
    // Posisjon og størrelse
    Point getPosition() const{return currentPosition;}
    void setCurrentPosition(Point p){currentPosition = p;};
    int getBirdHeight() const{return birdHeight;}
    void setHeight(int h){birdHeight = h;}
    int getBirdWidth() const{return birdWidth;}
    void setWidth(int w){birdWidth = w;}

    // Farge (brukes ikke lenger)
    Color getColor() const{return color;}
    void setColor(Color c){color = c;};

    // Score
    int getScore() const{return score;}

    // Andre metoder
    Bird* get(){return this;};
    Image& getImage(){return image;}
    void setJumpPower(int power){jumpPower = power;}
    void setMoveSpeed(int s){moveSpeed = s;}
    void setCanfall(bool v){canFall = v;}
    void setBounceVelocity(double d){bounceVelocity = d;}
};

class Obsticle : public Image, public Config {
private:
    FloatPoint topLeftPos;
    int topLeftGapPos;
    int pipeWidth = 30;
    int pipeHeight = 200;
    int pipeSpeed = 4;
    int obsticleSpace = 500;

    std::unordered_map<const Bird*, bool> passed_by; // Inneholder fugler som har passert obstaklet

public:
    // Konstruktør
    Obsticle(int Win_W, int Win_H);

    // Get- og set-metoder
    Point getTopLeft() const;
    void setTopLeft(int pos);
    int getPipeWidth() const;
    void setPipeWidth(int w);
    int getPipeHeight() const;
    void setPipeHeight(int h);
    int getGapPos() const;
    int getObsticleSpace() const;
    void setObsticleSpace(int space);
    void setPipeSpeed(double speed);
    double getPipeSpeed() const;

    // Bevegelse av hindring
    void moveObsticle();

    // Passering av hindring
    void passedObsticle(const Bird& bird);
    bool isPassedObsticle(const Bird& bird) const;
};