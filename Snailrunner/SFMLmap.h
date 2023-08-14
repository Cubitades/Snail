#pragma once
#include <SFML/Graphics.hpp>

class SFMLmap : public sf::Drawable, public sf::Transformable
{
public:
	SFMLmap(int windowHeight, int windowWidth);
	~SFMLmap();

private:

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		// apply the entity's transform -- combine it with the one that was passed by the caller
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

		// draw the vertex array
		target.draw(whiteArea, states);
		target.draw(blackArea, states);
		target.draw(grayArea, states);
		//target.draw(wheel, states);
	}

	sf::RectangleShape whiteArea;
	sf::RectangleShape blackArea;
	sf::RectangleShape grayArea;
};