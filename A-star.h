#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Window\Event.hpp>
#include <iostream>
#include <vector>

#include "Tile.h"
#include "Pathfinder.h"

class Astar
{
public:
	Astar();
	~Astar();
	
private:
	void run();

	void Initialize();

	void HandleEvents();

	void Update(sf::Time & p_deltaTime);

	void Render();

	bool m_running, m_active;

	sf::RenderWindow m_window;

	std::vector<std::vector<Tile>> m_tiles;
	
	std::vector<sf::Vector2<int>> m_path;

	Pathfinder m_pathfinder;
};
