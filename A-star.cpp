#include "A-star.h"

int main(int argc, char* argv[])
{
	Astar astar;
	return 0;
}

Astar::Astar() : 
	m_window(sf::RenderWindow(sf::VideoMode(640, 640), "", sf::Style::Close)),
	m_running(true),
	m_active(true)
{
	run();
}

Astar::~Astar()
{

}

void Astar::run()
{
	Initialize();

	sf::Clock dt;
	sf::Time deltaTime;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while(m_running)
	{
		deltaTime += dt.getElapsedTime();
		dt.restart();

		HandleEvents();
		Render();
		while (deltaTime >= updateTime && m_active)
		{
			Update(deltaTime);
			deltaTime -= updateTime;
		}
	}
}

void Astar::Initialize()
{
	m_tiles.resize(10, std::vector<Tile>(10, Tile()));

	for (unsigned int x = 0, y = 0; x < 10; ++x)
	{
		for (y = 0; y < 10; ++y)
		{
			m_tiles[x][y].m_x = x;
			m_tiles[x][y].m_y = y;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		m_tiles[0][i].m_tileType = TileType::Wall;
		m_tiles[i][0].m_tileType = TileType::Wall;
		m_tiles[9][i].m_tileType = TileType::Wall;
		m_tiles[i][9].m_tileType = TileType::Wall;

		if (i <= 5)
		{
			m_tiles[5][i].m_tileType = TileType::Wall;
		}
	}

	m_pathfinder = Pathfinder(&m_tiles);
}

void Astar::HandleEvents()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				m_running = false;
			}
		}
	}
}

void Astar::Update(sf::Time & p_deltaTime)
{
	m_path = m_pathfinder.GetPath(sf::Vector2<int>(1, 1), sf::Vector2<int>(sf::Mouse::getPosition(m_window)/64));

	if (!m_path.empty())
	{
		std::cout << "";
	}
}

void Astar::Render()
{
	m_window.clear(sf::Color(46, 46, 46));

	for (unsigned int x = 0, y = 0; x < 10; ++x)
	{
		for (y = 0; y < 10; ++y)
		{
			sf::RectangleShape tile = sf::RectangleShape(sf::Vector2<float>(64, 64));
			tile.setPosition(sf::Vector2<float>(x*64, y*64));

			switch (m_tiles[x][y].m_tileType)
			{
			case TileType::Floor:
				break;

			case TileType::Wall:
				tile.setFillColor(sf::Color::Black);
				m_window.draw(tile);
				break;

			default:
				tile.setFillColor(sf::Color::Red);
				m_window.draw(tile);
				break;
			}
		}
	}

	for (unsigned int i = 0; i < m_path.size(); ++i)
	{
		sf::RectangleShape node = sf::RectangleShape(sf::Vector2<float>(64, 64));
		node.setPosition(sf::Vector2<float>(m_path[i].x*64, m_path[i].y*64));

		node.setFillColor(sf::Color::Magenta);
		m_window.draw(node);
	}

	m_window.display();
}
