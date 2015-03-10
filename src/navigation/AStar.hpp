#pragma once

#include <cassert>
#include <algorithm>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <utility>

#include "../map/Map.hpp"


class AStar
{
private:
	struct Node
	{
		std::weak_ptr<Node> parent;
		float f, g, h;
		bool open, closed;

		sf::Vector2i position;

		Node(const sf::Vector2i& position, std::weak_ptr<Node> parent) :
			position(position),
			parent(parent),
			open(true),
			closed(false)
		{}

		bool operator==(const Node& node) const
		{
			return node.position == position;
		}
	};

	struct Key
	{
		int x, y;

		bool operator==(const Key& key) const
		{
			return x == key.x && y == key.y;
		}
	};

	struct Hasher
	{
		std::size_t operator()(const Key& key) const
		{
			int hash = 17;
			hash = ((hash + key.x) << 5) - (hash + key.x);
			hash = ((hash + key.y) << 5) - (hash + key.y);
			return hash;
		}
	};

	Map& m_map;

	std::vector<std::weak_ptr<Node>> m_open;
	std::unordered_map<Key, std::shared_ptr<Node>, Hasher> m_nodes;

	short m_reserved;
	sf::Vector2i m_source, m_destination;

	static bool compare(const std::weak_ptr<Node> a, const std::weak_ptr<Node> b)
	{
		auto _a = a.lock();
		auto _b = b.lock();

		if (_a && _b)
			return _a->f > _b->f && _b->open;
		else
			return false;
	}

	float heuristic(const sf::Vector2i& position) const;
	int movementCost(const sf::Vector2i& position, const sf::Vector2i& parentPosition, const int parentMovementCost) const;
	bool illegalDiagonal(const sf::Vector2i& a, const sf::Vector2i& b) const;
	bool checkAdjacentNodes(const std::shared_ptr<Node> node);
	void constructPath();
	void search();

public:
	AStar(Map& map, const sf::Vector2i& source, const sf::Vector2i& destination);
};

