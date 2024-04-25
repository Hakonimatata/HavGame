#include "Entities.h"

void Bird::jump(){
    //sett ny v0 osv 
    setVelocity(jumpPower);
};

void Bird::fall(){
    
    if (currentPosition.y < 4000){//begrense rendering
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


