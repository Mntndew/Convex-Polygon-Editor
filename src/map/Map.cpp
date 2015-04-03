#include "Map.hpp"


Map::Map() :
	m_index(0)
{
	m_quadtree.reset(new Quadtree(sf::Vector2f(0, 0), sf::Vector2f(2048, 2048), false, 0));
}

Map::~Map()
{
}

void Map::addPolygon(const math::Polygon& polygon)
{
	m_quadtree->insert(std::shared_ptr<math::Polygon>(new math::Polygon(polygon)));

	sf::ConvexShape shape;
	shape.setPointCount(polygon.getPointCount());

	for (int i = 0; i < shape.getPointCount(); ++i)
		shape.setPoint(i, polygon.getPoint(i));
	
	m_polygons[m_index] = shape;
	m_index++;
}

void Map::removePolygon(std::shared_ptr<math::Polygon> polygon)
{
	m_quadtree->remove(polygon);
}

std::shared_ptr<math::Polygon> Map::getPolygon(const sf::Vector2f& position)
{
	return m_quadtree->getPolygon(position);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = m_polygons.begin(); it != m_polygons.end(); ++it)
		target.draw(it->second, states);

	m_quadtree->draw(target, states);
}

