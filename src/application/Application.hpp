#pragma once

#include <iostream>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include "../map/Map.hpp"
#include "../navigation/A*.hpp"

class Application
{
public:
	Application(int argc, char** argv);
	~Application();
	
private:
	void run();
	void initialize();
	void handleEvents();
	void update(sf::Time & deltaTime);
	void render();

	bool m_running, m_active;

	sf::RenderWindow m_window;

	Map m_map;

	bool m_space, m_mouse;

	AStar* m_aStar;
};

