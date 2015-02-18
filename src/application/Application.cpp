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

	m_window.create(sf::VideoMode(1280, 720), "A*", sf::Style::Close, settings);

	m_running = true;
	m_active = true;
	m_space = false;
	m_mouse = false;

	m_map = Map();

	m_aStar = new AStar(m_map, sf::Vector2i(8, 3), sf::Vector2i(8, 13));
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
		{
			m_running = false;
		}
	}
}

void Application::update(sf::Time & p_deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (!m_space)
		{
		}

		m_space = true;
	}
	else
	{
		m_space = false;
	}
}

void Application::render()
{
	m_window.clear(sf::Color(46, 46, 46));
	m_window.draw(m_map);
	m_window.display();
}

