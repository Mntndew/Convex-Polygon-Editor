#include "Application.hpp"

Application::Application(int argc, char** argv)
{
	run();
}

Application::~Application()
{

}

void Application::initialize()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 32;

	m_window.create(sf::VideoMode(1600, 900), "Editor", sf::Style::Close, settings);
	m_view.reset(sf::FloatRect(0, 0, 1600, 900));

	m_shape.setFillColor(sf::Color(0, 255, 0, 100));
	m_index = 0;
	m_mouse = false;

	m_running = true;
	m_active = true;
}

void Application::run()
{
	initialize();
	
	sf::Clock dt;
	sf::Time deltaTime;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while(m_running)
	{
		deltaTime += dt.getElapsedTime();
		dt.restart();

		handleEvents();
		render();
		while (deltaTime >= updateTime && m_active)
		{
			update(deltaTime);
			deltaTime -= updateTime;
		}
	}
}

void Application::handleEvents()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_running = false;
		else if (event.type == sf::Event::KeyPressed)
		{
		}
	}
}

void Application::update(sf::Time & p_deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_view.move(0, -10);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_view.move(0, 10);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_view.move(-10, 0);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_view.move(10, 0);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!m_mouse)
		{
			sf::Vector2f mousePosition{sf::Mouse::getPosition(m_window).x + m_view.getCenter().x - 800, sf::Mouse::getPosition(m_window).y + m_view.getCenter().y - 450};
			m_polygon.addPoint(mousePosition);
			m_polygon.constructEdges();
			m_index++;
			m_shape.setPointCount(m_index);
			m_shape.setPoint(m_index-1, mousePosition);

			m_mouse = true;
		}
	}
	else
		m_mouse = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && m_index > 0)
	{
		m_index = 0;
		m_map.addPolygon(m_polygon);
		m_polygons.push_back(m_polygon);
		m_polygon.clear();
		m_shape.setPointCount(0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		m_map.save();
}

void Application::render()
{
	m_window.setView(m_view);
	m_window.clear(sf::Color(46, 46, 46));

	m_window.draw(m_shape);
	m_window.draw(m_map);

	m_window.display();
}

