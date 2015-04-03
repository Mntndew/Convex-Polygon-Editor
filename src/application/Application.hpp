#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include "Math.hpp"
#include "../map/Map.hpp"

struct Polygon
{
	math::Polygon polygon;
	sf::ConvexShape shape;
};

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

	bool m_mouse;
	int m_index;

	math::Polygon m_polygon;
	sf::ConvexShape m_shape;

	std::vector<Polygon> m_polygons;

	Map m_map;

	sf::RenderWindow m_window;
	sf::View m_view;
};

