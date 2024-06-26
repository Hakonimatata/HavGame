#include "GameWindow.h"
#include "sound&music.h"


GameWindow::GameWindow(int W, int H) : 
    Win_W{W},
    Win_H{H},
    AnimationWindow{0, 0, W, H, "Flappy Bird"},
    background{"MediaFiles\\sky.jpg"},
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
    if (bird.getPosition().y > Win_H || bird.getPosition().y < -Win_H/4 || bird.getPosition().x < 0){
        bird.crash();
    }
    
    if (bird.getPosition().y <= Win_H){
        // Kommenter ut under for å endre til en boks
        // draw_rectangle(bird.getPosition(), bird.getBirdWidth(), bird.getBirdHeight(), bird.getColor());
        
        //tegn bilde av bird:
        draw_image({bird.getPosition().x - imageShift, bird.getPosition().y - imageShift}, bird.getImage(), bird.getBirdWidth() + imageShift, bird.getBirdHeight() + imageShift);
    }
}

void GameWindow::run(){
    playSoundLoop("MediaFiles\\background_music.wav"); // evig musikk. Kan ikke endres undervis slik den er nå
    fillObsticleVector();
    fillBirdsVector();

    while(!should_close()){ // spilløkke
        drawBackground();

        //ittere over obsticles for å bevege hver av dem:
        for(auto& obsticle : obsticles){
            drawObsticle(obsticle);
            obsticle.moveObsticle();
        }
        
        handleInput(); // håndtere input fra tastatur
        drawScore();
        bounce(birds); // fjern denne for å skru av bounce mellom birds

        //gå over hver bird og sjekk om de krasjer med hver obsticle
        for(auto& bird : birds){
            drawBird(bird);
            
            if (bird.getCanFall() || bird.isCrashed()){
                bird.fall(); // oppdaterer fall fysikken
                bird.push(); // oppdaterer push fysikken
            }

            for(auto& obsticle : obsticles){
                if (checkCollition(obsticle, bird) && !bird.isCrashed()){
                    bird.crash();
                    bird.pushImpulse({-2,1}, 3*obsticle.getPipeSpeed());
                    playSound("MediaFiles\\punch_sound.wav");
                }
                //teller score:
                if(bird.getPosition().x > obsticle.getTopLeft().x + obsticle.getPipeWidth() && !obsticle.isPassedObsticle(bird) && !bird.isCrashed()){
                    bird.incrementScore();
                    obsticle.passedObsticle(bird);
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
    
    
    if(currentUpKeyState && !lastUpKeyState && !birds.at(0).isCrashed()) {
        birds.at(0).setCanfall(true);
        birds.at(0).jump();
    }   

    if(currentRightKeyState && !lastRightKeyState && !birds.at(0).isCrashed()) {
        birds.at(0).moveRight();
    }  

    if(currentLeftKeyState && !lastLeftKeyState && !birds.at(0).isCrashed()) {
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

        if(currentWKeyState && !lastWKeyState && !birds.at(1).isCrashed()) {
            birds.at(1).setCanfall(true);
            birds.at(1).jump();
        }   

        if(currentDKeyState && !lastDKeyState && !birds.at(1).isCrashed()) {
            birds.at(1).moveRight();
        }  

        if(currentAKeyState && !lastAKeyState && !birds.at(1).isCrashed()) {
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

        if(currentIKeyState && !lastIKeyState && !birds.at(2).isCrashed()) {
            birds.at(2).setCanfall(true);
            birds.at(2).jump();
        }   

        if(currentLKeyState && !lastLKeyState && !birds.at(2).isCrashed()) {
            birds.at(2).moveRight();
        }  

        if(currentJKeyState && !lastJKeyState && !birds.at(2).isCrashed()) {
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

    //Rør topp
    draw_rectangle(topLeft, width, gapPos, Color::green);
    draw_rectangle({topLeft.x+5, topLeft.y}, width-2*5, gapPos, Color::light_green);
    draw_rectangle({topLeft.x-5, gapPos-20}, width+2*5, 20, Color::green);
    //Rør bunn
    draw_rectangle({topLeft.x, gapPos + pipeHeight}, width, Win_H - pipeHeight - gapPos, Color::green);
    draw_rectangle({topLeft.x+5, gapPos + pipeHeight}, width-2*5, Win_H - pipeHeight - gapPos, Color::light_green);
    draw_rectangle({topLeft.x-5, gapPos + pipeHeight}, width+2*5, 20, Color::green);

    //flytter obsticle bakerst når den går ut av skjermen:
    if(topLeft.x < -obsticle.getObsticleSpace()){
        Obsticle newObsticle{Win_W, Win_H};
        Point backPos = obsticles.back().getTopLeft();
        newObsticle.setTopLeft(backPos.x + obsticle.getObsticleSpace());
        newObsticle.setPipeSpeed(obsticles.at(0).getPipeSpeed());
        obsticles.push_back(newObsticle);
        obsticles.erase(obsticles.begin());
    }
}



void GameWindow::fillObsticleVector(){
    int space = obsticle.getObsticleSpace();
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

    // Sjekk om fuglen kolliderer med røret eller gapet
    bool between_pipes = (bird_x + bird.getBirdWidth() > pipe_x) && (bird_x < pipe_x + obsticle.getPipeWidth());
    bool above_gap = bird_y < gap_y;
    bool below_gap = bird_y + bird.getBirdHeight() > gap_y + obsticle.getPipeHeight();

    return between_pipes && (above_gap || below_gap);
} 


void GameWindow::gameOver(){
    gameEnd = true;
}

void GameWindow::fillBirdsVector() {
    const vector<string> imageFilenames = {"MediaFiles\\havard.png", "MediaFiles\\havard_2.png", "MediaFiles\\havard_3.png"};

    for (int i = 0; i < numberOfPLayers; ++i) {
        int offsetX = i * 90;
        Bird bird{birdStartPosition.x + offsetX, birdStartPosition.y, imageFilenames.at(i)};
        birds.push_back(bird);
    }
}

void GameWindow::restartGame(){
    int mellomrom = 0;
    for(auto& bird : birds){
        bird.setCanfall(false);
        bird.setCrashed(false);
        bird.setVelocity(0.0);
        bird.setBounceVelocity(0.0);
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

void GameWindow::drawScore() {
    for (int i = 0; i < numberOfPLayers; ++i) {
        draw_text({10 + i * 200, 10}, "Player " + to_string(i + 1) + ":  " + to_string(birds.at(i).getScore()), Color::white, 30, Font::courier_bold);
    }
}

bool GameWindow::checkCollision(Bird& bird1, Bird& bird2){
    bool x_overlap = (bird1.getPosition().x < bird2.getPosition().x + bird2.getBirdWidth()) && (bird1.getPosition().x + bird1.getBirdWidth() > bird2.getPosition().x);
    bool y_overlap = (bird1.getPosition().y < bird2.getPosition().y + bird2.getBirdHeight()) && (bird1.getPosition().y + bird1.getBirdHeight() > bird2.getPosition().y);

    return x_overlap && y_overlap;
}

void GameWindow::bounce(vector<Bird>& birds){
    
    for(auto& bird : birds){
        for(auto& other : birds){
            if((other.get() != bird.get()) && checkCollision(bird, other) && !bird.isCrashed()){
                FloatPoint direction = {static_cast<double>(bird.getPosition().x - other.getPosition().x), static_cast<double>(bird.getPosition().y - other.getPosition().y)};
                bird.pushImpulse(direction, 10.0); // 10 er hvor kraftig bouncen er
            }
        }
    }   
}

void GameWindow::setAllObsticlesSpeed(vector<Obsticle> obs, int speed){
    for(auto& obsticle : obs){
        obsticle.setPipeSpeed(speed);
    }
}