#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Tile.hpp"


class Map : public sf::Drawable
{
public:
	std::vector<std::vector<Tile>> tiles;

	Map()
	{
		tiles.resize(16, std::vector<Tile>(16, Tile(0, sf::Vector2i(0, 0))));

		for (int x = 0; x < 16; ++x)
		{
			for (int y = 0; y < 16; ++y)
			{
				tiles[x][y].type = (x == 0 || x == 15 || y == 0 || y == 15 || (y == 8 && (x < 10 && x > 1)) || (y == 5 && (x < 13 && x > 3)) || (y == 10 && (x < 13 && x > 3)));
			}
		}

		tiles[8][3].type = 2;
		tiles[8][13].type = 3;
	}

	~Map()
	{}

	virtual void draw(sf::RenderTarget & target, sf::RenderStates state) const
	{
		
		for (int x = 0; x < 16; ++x)
		{
			for (int y = 0; y < 16; ++y)
			{
				sf::RectangleShape tile(sf::Vector2f(32, 32));
				tile.setPosition(sf::Vector2f(x*32, y*32));

				switch (tiles[x][y].type)
				{
					case 0: // Walkable gorund
						tile.setFillColor(sf::Color(255, 255, 255));
					break;

					case 1: // Wall
						tile.setFillColor(sf::Color(0, 0, 0));
					break;

					case 2: // Source
						tile.setFillColor(sf::Color(0, 0, 255));
					break;
					
					case 3: // Destination
						tile.setFillColor(sf::Color(0, 255, 0));
					break;
					
					case 4: // Open
						tile.setFillColor(sf::Color(255, 0, 255));
					break;

					case 5: // Closed
						tile.setFillColor(sf::Color(255, 0, 0));
					break;

					case 6: // Path
						tile.setFillColor(sf::Color(0, 255, 255));
					break;
				}

				target.draw(tile);
			}
		}
	}
};

