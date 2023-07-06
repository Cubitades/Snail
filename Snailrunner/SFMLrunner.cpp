#include "SFMLrunner.h"

SFMLrunner::SFMLrunner(int posX, int posY)
{
    //draw chassis
    chassis.setSize(sf::Vector2f(chassisWidth, chassisHeight));
    chassis.setFillColor(sf::Color::Red);
    chassis.setOrigin(chassisWidth/2, chassisHeight/2);
    //chassis.setPosition(posX, posY);

    //draw ControllerBoard
    controllerboard.setSize(sf::Vector2f(controllerWidth, controllerHeight));
    controllerboard.setFillColor(sf::Color(50, 50, 50));
    controllerboard.setOrigin(controllerWidth/2, controllerHeight/2);
    controllerboard.setPosition(chassis.getOrigin().x - chassisWidth/2, (chassis.getOrigin().y-chassisHeight/2)+10);

    //draw right wheel
    rightwheel.setSize(sf::Vector2f(wheelWidth, wheelHeight));
    rightwheel.setFillColor(sf::Color(100, 100, 100));
    rightwheel.setOrigin(wheelWidth/2, wheelHeight/2);
    rightwheel.setPosition((chassis.getOrigin().x-chassisWidth/2)+30, (chassis.getOrigin().y-chassisHeight/2)+10);

    //draw left wheel
    leftwheel.setSize(sf::Vector2f(wheelWidth, wheelHeight));
    leftwheel.setFillColor(sf::Color(100, 100, 100));
    leftwheel.setOrigin(wheelWidth/2, wheelHeight/2);
    leftwheel.setPosition((chassis.getOrigin().x-chassisWidth/2)-30, (chassis.getOrigin().y-chassisHeight/2)+10);

    //draw right lamp
    leftlamp.setSize(sf::Vector2f(lampWidth, lampHeight));
    leftlamp.setFillColor(sf::Color::Yellow);
    leftlamp.setOrigin(lampWidth/2, lampHeight/2);
    leftlamp.setPosition((chassis.getOrigin().x-chassisWidth/2)-30, (chassis.getOrigin().y-chassisHeight/2)+10);
    
    //draw left lamp
    rightlamp.setSize(sf::Vector2f(lampWidth, lampHeight));
    rightlamp.setFillColor(sf::Color::Yellow);
    rightlamp.setOrigin(lampWidth/2, lampHeight/2);
    rightlamp.setPosition((chassis.getOrigin().x-chassisWidth/2)-30, (chassis.getOrigin().y-chassisHeight/2)+10);
}

SFMLrunner::~SFMLrunner(){};