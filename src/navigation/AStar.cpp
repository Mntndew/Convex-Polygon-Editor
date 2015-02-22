#include "AStar.hpp"


AStar::AStar(Map& map, const sf::Vector2i& source, const sf::Vector2i& destination) :
	m_map(map),
	m_source(source),
	m_destination(destination)
{
	search();
}

void AStar::search()
{
	m_open.push_back(std::shared_ptr<AStar::Node>(new Node(m_source, std::weak_ptr<AStar::Node>())));

	calculateHeuristic(m_open[0]);
	m_open[0]->f = m_open[0]->h;

	while (!m_open.empty())
	{
		std::shared_ptr<AStar::Node> selectedNode = m_open.back();
		m_open.pop_back();
		m_closed.insert(std::make_pair<Key, std::shared_ptr<AStar::Node>&>(Key({selectedNode->position.x, selectedNode->position.y}), selectedNode));

		if (checkAdjacentNodes(selectedNode))
		{
			constructPath();
			return;
		}

		std::sort(m_open.begin(), m_open.end(), compare);
	}
}

void AStar::constructPath()
{
	auto node = m_closed.find({m_destination.x, m_destination.y})->second;
	int index = 0;

	while (auto parent = node->parent.lock())
	{
		m_map.tiles[node->position.x][node->position.y].type = 6;
		node = parent;
	}
}

bool AStar::checkAdjacentNodes(const std::shared_ptr<AStar::Node> node)
{
	for (int i = 0; i < 9; ++i)
	{
		if (!(i%3 == 1 && i/3 == 1))
		{
			sf::Vector2i position(node->position.x - 1 + i%3, node->position.y - 1 + i/3);

			if (m_map.tiles[position.x][position.y].type != 1 && !illegalDiagonal(node->position, position))
			{

				if (m_closed.find({position.x, position.y}) != m_closed.end())
					continue;

				std::shared_ptr<AStar::Node> adjacent;

				if (position == m_destination)
				{
					adjacent.reset(new Node(position, node));
					m_closed.insert(std::make_pair<Key, std::shared_ptr<AStar::Node>&>(Key({adjacent->position.x, adjacent->position.y}), adjacent));
					return true;
				}

				adjacent = getOpenNode(position);

				if (adjacent == nullptr)
				{
					adjacent.reset(new Node(position, node));
					calculateHeuristic(adjacent);
					recalculate(adjacent, node);
					m_open.push_back(adjacent);
				}
				else
				{
					int g = node->g + ((position.x == node->position.x || position.y == node->position.y) ? 10:14);

					if (g < adjacent->g)
					{
						adjacent->parent = node;
						adjacent->g = g;
						adjacent->f = adjacent->g + adjacent->h;
					}
				}
			}
		}
	}

	return false;
}

void AStar::recalculate(std::shared_ptr<AStar::Node> node, const std::shared_ptr<AStar::Node> parent)
{
	node->g = parent->g + ((node->position.x == parent->position.x || node->position.y == parent->position.y) ? 1:std::sqrt(2));
	node->f = node->g + node->h;
}

void AStar::calculateHeuristic(std::shared_ptr<AStar::Node> node)
{
	node->h = .05f*(std::pow(m_destination.x - node->position.x, 1) + std::pow(m_destination.y - node->position.y, 1));
}

bool AStar::illegalDiagonal(const sf::Vector2i& a, const sf::Vector2i& b) const
{
	int x = a.x > b.x ? a.x:b.x;
	int y = a.y > b.y ? a.y:b.y;
	
	if ((x == a.x && y == a.y) || (x == b.x && y == b.y)) // Max coordinates belong to the same node.
		return (m_map.tiles[x - 1][y].type == 1 || m_map.tiles[x][y - 1].type == 1);
	else
		return (m_map.tiles[x - 1][y - 1].type == 1 || m_map.tiles[x][y].type == 1);
}

bool AStar::inOpen(const sf::Vector2i& position) const
{
	for (int i = 0; i < m_open.size(); ++i)
	{
		if (m_open[i]->position == position)
			return true;
	}

	return false;
}

std::shared_ptr<AStar::Node> AStar::getOpenNode(const sf::Vector2i& position)
{
	for (int i = 0; i < m_open.size(); ++i)
	{
		if (m_open[i]->position == position)
			return m_open[i];
	}

	return nullptr;
}

