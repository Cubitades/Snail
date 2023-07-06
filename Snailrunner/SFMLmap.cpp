#include "SFMLmap.h"

SFMLmap::SFMLmap(int windowHeight, int windowWidth, int lineWidth)
{
    whiteArea.setSize(sf::Vector2f(windowWidth*0.8, windowHeight*0.6));
	whiteArea.setFillColor(sf::Color(255, 255, 255)); //white
	whiteArea.setPosition((windowWidth*0.1), (windowHeight*0.2));

	//Draw Black Area - dependent on white area
	blackArea.setSize(sf::Vector2f(whiteArea.getSize().x - lineWidth*2, whiteArea.getSize().y - lineWidth*2));
	blackArea.setFillColor(sf::Color(0, 0, 0)); //black
	blackArea.setPosition(whiteArea.getPosition().x + lineWidth, whiteArea.getPosition().y + lineWidth);

	//Draw Gray Area - depentend on white area
	grayArea.setSize(sf::Vector2f(whiteArea.getSize().x/4, lineWidth));
	grayArea.setFillColor(sf::Color(150, 150, 150));
	grayArea.setPosition((windowWidth/2) - (whiteArea.getSize().x/4)/2, whiteArea.getPosition().y + whiteArea.getSize().y - lineWidth);
}

SFMLmap::~SFMLmap(){};