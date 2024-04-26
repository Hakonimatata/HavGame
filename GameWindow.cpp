#include "GameWindow.h"


GameWindow::GameWindow(int W, int H) : 
    Win_W{W},
    Win_H{H},
    AnimationWindow{0, 0, W, H, "Flappy Bird"},
    background{"sky.jpg"},
    quitBtn{{W - 100, 0}, 100, 40, "Quit"},
    onePlayer{{W/2-100, 0}, 100, 40, "1 Player"},
    twoPlayer{{W/2, 0}, 100, 40, "2 Player"},
    threePlayer{{W/2+100, 0}, 100, 40, "3 Player"}
{
    quitBtn.setCallback(bind(&GameWindow::cb_quitBtn, this));
    add(quitBtn);

    onePlayer.setCallback(bind(&GameWindow::cb_onePlayer, this));
    add(onePlayer);
    twoPlayer.setCallback(bind(&GameWindow::cb_twoPlayer, this));
    add(twoPlayer);
    threePlayer.setCallback(bind(&GameWindow::cb_threePlayer, this));
    add(threePlayer);
}


void GameWindow::drawBird(Bird& bird){
    if (bird.getPosition().y > Win_H){
        bird.crash();
    }
    
    if (!bird.hasCrashed()){
        // Kommenter ut under for å endre til en boks
        // draw_rectangle(bird.getPosition(), bird.getBirdWidth(), bird.getBirdHeight(), bird.getColor());
        
        //tegn bilde av bird:
        draw_image({bird.getPosition().x - imageShift, bird.getPosition().y - imageShift}, bird.getImage(), bird.getBirdWidth() + imageShift, bird.getBirdHeight() + imageShift);
    }
    
    

}

void GameWindow::run(){
    fillObsticleVector();
    fillBirdsVector();

    while(!should_close()){
        drawBackground();

        //ittere over obsticles for å bevege hver av dem:
        for(auto& obsticle : obsticles){
            drawObsticle(obsticle);
            obsticle.moveObsticle();
        }
        
        handleInput(); // håndtere input fra tastatur
        drawScore();

        //gå over hver bird og sjekk om de krasjer med hver obsticle
        for(auto& bird : birds){
            drawBird(bird);
            
            if (!bird.hasCrashed()){
                if(bird.canFall()){bird.fall();}
            }

            for(auto& obsticle : obsticles){
                if (checkCollition(obsticle, bird)){
                    bird.crash();
                }
                //teller score:
                if(bird.getPosition().x > obsticle.getTopLeft().x + obsticle.getPipeWidth() && !obsticle.isPassedObsticle() && !bird.hasCrashed()){
                    bird.incrementScore();
                    obsticle.passedObsticle();
                }
            }
        }
        
        next_frame();
    }
}


void GameWindow::handleInput(){
    // Player 1:
    static bool lastUpKeyState = false;
    static bool lastRightKeyState = false;
    static bool lastLeftKeyState = false;
    static bool lastResetKeyState = false;

    bool currentUpKeyState = is_key_down(KeyboardKey::UP);
    bool currentRightKeyState = is_key_down(KeyboardKey::RIGHT);
    bool currentLeftKeyState = is_key_down(KeyboardKey::LEFT);
    bool currentResetKeyState = is_key_down(KeyboardKey::R);
    
    
    if(currentUpKeyState && !lastUpKeyState) {
        birds.at(0).enableFall();
        birds.at(0).jump();
    }   

    if(currentRightKeyState && !lastRightKeyState && !birds.at(0).hasCrashed()) {
        birds.at(0).moveRight();
    }  

    if(currentLeftKeyState && !lastLeftKeyState && !birds.at(0).hasCrashed()) {
        birds.at(0).moveLeft();
    }

    if(currentResetKeyState && !lastResetKeyState) {
        restartGame();
    }

    lastUpKeyState = currentUpKeyState;
    lastResetKeyState = currentResetKeyState;
    //---------------------------------------
    //bird 2:
    if(numberOfPLayers >= 2){

        static bool lastWKeyState = false;
        static bool lastDKeyState = false;
        static bool lastAKeyState = false;

        bool currentWKeyState = is_key_down(KeyboardKey::W);
        bool currentDKeyState = is_key_down(KeyboardKey::D);
        bool currentAKeyState = is_key_down(KeyboardKey::A);

        if(currentWKeyState && !lastWKeyState) {
            birds.at(1).enableFall();
            birds.at(1).jump();
        }   

        if(currentDKeyState && !lastDKeyState && !birds.at(1).hasCrashed()) {
            birds.at(1).moveRight();
        }  

        if(currentAKeyState && !lastAKeyState && !birds.at(1).hasCrashed()) {
            birds.at(1).moveLeft();
        }

        lastWKeyState = currentWKeyState;
    }
    //---------------------------------------
    //bird 3:
    if(numberOfPLayers == 3){

        static bool lastIKeyState = false;
        static bool lastLKeyState = false;
        static bool lastJKeyState = false;

        bool currentIKeyState = is_key_down(KeyboardKey::I);
        bool currentLKeyState = is_key_down(KeyboardKey::L);
        bool currentJKeyState = is_key_down(KeyboardKey::J);

        if(currentIKeyState && !lastIKeyState) {
            birds.at(2).enableFall();
            birds.at(2).jump();
        }   

        if(currentLKeyState && !lastLKeyState && !birds.at(2).hasCrashed()) {
            birds.at(2).moveRight();
        }  

        if(currentJKeyState && !lastJKeyState && !birds.at(2).hasCrashed()) {
            birds.at(2).moveLeft();
        }

        lastIKeyState = currentIKeyState;
    }      
}


void GameWindow::drawObsticle(Obsticle& obsticle){
    Point topLeft = obsticle.getTopLeft();
    int width = obsticle.getPipeWidth();
    int gapPos = obsticle.getGapPos();
    int pipeHeight = obsticle.getPipeHeight();

    draw_rectangle(topLeft, width, gapPos, Color::green);
    draw_rectangle({topLeft.x+5, topLeft.y}, width-2*5, gapPos, Color::light_green);
    draw_rectangle({topLeft.x-5, gapPos-20}, width+2*5, 20, Color::green);

    draw_rectangle({topLeft.x, gapPos + pipeHeight}, width, Win_H - pipeHeight - gapPos, Color::green);
    draw_rectangle({topLeft.x+5, gapPos + pipeHeight}, width-2*5, Win_H - pipeHeight - gapPos, Color::light_green);
    draw_rectangle({topLeft.x-5, gapPos + pipeHeight}, width+2*5, 20, Color::green);

    //flytter obsticle bakerst når den går ut av skjermen:
    if(topLeft.x < -obsticle.getObsticleSPace()){
        obsticles.erase(obsticles.begin());
        Obsticle newObsticle{Win_W, Win_H};
        Point backPos = obsticles.back().getTopLeft();
        newObsticle.setTopLeft(backPos.x + obsticle.getObsticleSPace());
        obsticles.push_back(newObsticle);
    }
}



void GameWindow::fillObsticleVector(){
    int space = obsticle.getObsticleSPace();
    int numObsticles = Win_W/space + 1;
    
    for(int i = 0; i <= numObsticles; i++){
        Obsticle newObsticle{Win_W, Win_H};
        newObsticle.setTopLeft(Win_W + i*space);
        obsticles.push_back(newObsticle);
    }
}

bool GameWindow::checkCollition(Obsticle& obsticle, Bird& bird){

    int pipe_x = obsticle.getTopLeft().x;
    int gap_y = obsticle.getGapPos();
    int bird_x = bird.getPosition().x;
    int bird_y = bird.getPosition().y;

    if(bird_x + bird.getBirdWidth() > pipe_x && bird_x < (pipe_x + obsticle.getPipeWidth()) //mellom to vegger?
        && (bird_y < gap_y || bird_y + bird.getBirdHeight() > gap_y + obsticle.getPipeHeight())) // under eller over gap?
    {
        return true;
    }
    else{return false;}
} 

void GameWindow::gameOver(){
    gameEnd = true;
}


void GameWindow::fillBirdsVector(){
    Bird bird1{birdStartPosition.x, birdStartPosition.y, "havard.png"};
    bird1.setColor(Color::red);
    birds.push_back(bird1);

    if(numberOfPLayers >= 2){
        Bird bird2{birdStartPosition.x + 90, birdStartPosition.y, "havard_2.png"};
        bird2.setColor(Color::blue);
        birds.push_back(bird2);
    }

    if(numberOfPLayers == 3){
        Bird bird3{birdStartPosition.x + 90*2, birdStartPosition.y, "havard_3.png"};
        bird3.setColor(Color::green);
        birds.push_back(bird3);
    }

}

void GameWindow::restartGame(){
    int mellomrom = 0;
    for(auto& bird : birds){
        bird.disableFall();
        bird.revive();
        bird.setVelocity(0.0);
        bird.resetScore();
        bird.setCurrentPosition({birdStartPosition.x + mellomrom, birdStartPosition.y});
        mellomrom += 90;
        
        //lager ny obsticles
        obsticles = {};
        fillObsticleVector();
    }
}

void GameWindow::drawBackground(){
    draw_image({0, 0}, background, Win_W, Win_H);
}

void GameWindow::drawScore(){
    draw_text({10, 10}, "Player 1:  " + to_string(birds.at(0).getScore()), Color::white, 30, Font::arial);
    if(numberOfPLayers >= 2){
       draw_text({200, 10}, "Player 2:  " + to_string(birds.at(1).getScore()), Color::white, 30, Font::arial);
    }
    if(numberOfPLayers == 3){
       draw_text({390, 10}, "Player 3:  " + to_string(birds.at(2).getScore()), Color::white, 30, Font::arial);
    }
}

//Hei
//Dette er en endring