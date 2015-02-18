#pragma once

#include <SFML/System/Vector2.hpp>


class Tile
{
public:
	Tile()
	{}

	Tile(const int type, const sf::Vector2i position) :
		type(type),
		position(position),
		f(-1),
		g(-1),
		h(-1),
		index(-1)
	{}

	~Tile()
	{}

	int type, f, g, h, index;
	sf::Vector2i position;
};

