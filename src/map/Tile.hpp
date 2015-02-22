#pragma once

#include <SFML/System/Vector2.hpp>


class Tile
{
public:
	Tile()
	{}

	Tile(const int type, const sf::Vector2i position) :
		type(type),
		position(position)
	{}

	~Tile()
	{}

	int type;
	sf::Vector2i position;
};

