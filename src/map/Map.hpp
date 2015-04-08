#pragma once

#include <map>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "Quadtree.hpp"


class Map : public sf::Drawable
{
public:
	Map();
	~Map();

	void addPolygon(const math::Polygon& polygon);
	void removePolygon(std::shared_ptr<math::Polygon> polygon);

	std::shared_ptr<math::Polygon> getPolygon(const sf::Vector2f& position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void save() const;

private:
	unsigned int m_index;
	std::shared_ptr<Quadtree> m_quadtree;
	std::map<unsigned int, sf::ConvexShape> m_polygons;
};

