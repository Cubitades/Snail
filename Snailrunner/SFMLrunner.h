#pragma once
#include <SFML/Graphics.hpp>

class SFMLrunner : public sf::Drawable, public sf::Transformable
{
public:
    SFMLrunner(int posX, int posY);
    ~SFMLrunner();

private:

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

        // draw the shapes
        target.draw(chassis, states);
        target.draw(controllerboard, states);
        target.draw(rightwheel, states);
        target.draw(leftwheel, states);
        target.draw(rightlamp, states);
        target.draw(leftlamp, states);
    }

    sf::RectangleShape chassis;
    int chassisWidth = 50;
    int chassisHeight = 70;

    sf::RectangleShape controllerboard;
    int controllerWidth = 30;
    int controllerHeight = 30;

    sf::RectangleShape rightwheel;
    sf::RectangleShape leftwheel;
    int wheelWidth = 10;
    int wheelHeight = 30;

    sf::RectangleShape rightlamp;
    sf::RectangleShape leftlamp;
    int lampWidth = 10;
    int lampHeight = 10;

};