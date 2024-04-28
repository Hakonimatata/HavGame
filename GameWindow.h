#pragma once
#include "std_lib_facilities.h"
#include "AnimationWindow.h"
#include "Entities.h"
#include <random>
#include "Image.h"
#include "widgets/Button.h"

class GameWindow : public TDT4102::AnimationWindow, public Config {

    int Win_W;
    int Win_H;
    int numberOfPLayers = 1;
    Obsticle obsticle{Win_W, Win_H}; // generisk obstcle for å hente verdier fra
    Point birdStartPosition = {150, Win_H/2};
    vector<Obsticle> obsticles;
    vector<Bird> birds;

    Image background;

    bool gameEnd = false;
    int imageShift = 20; //bildet litt større enn hitboxen

    //knapper:
    Button quitBtn;
    Button onePlayer;
    Button twoPlayer;
    Button threePlayer;

    public:
        GameWindow(int W = 1920, int H = 1080);
        void run();
        void drawBird(Bird& bird);
        void drawObsticle(Obsticle& obsticle);
        void drawBackground();
        void handleInput();
        void fillObsticleVector();
        void fillBirdsVector();
        bool checkCollition(Obsticle& obsticle, Bird& bird);
        bool checkCollision(Bird& bird1, Bird& bird2);
        void bounce(vector<Bird>& birds); //bounce logikk
        void gameOver();
        void restartGame();
        void drawScore();
        void setAllObsticlesSpeed(vector<Obsticle> obs, int speed);

        void cb_quitBtn(){this->close();}
        void cb_onePlayer(){birds.clear(); numberOfPLayers = 1; fillBirdsVector();  restartGame();}
        void cb_twoPlayer(){birds.clear(); numberOfPLayers = 2; fillBirdsVector(); restartGame();}
        void cb_threePlayer(){birds.clear(); numberOfPLayers = 3; fillBirdsVector(); restartGame();}
};

