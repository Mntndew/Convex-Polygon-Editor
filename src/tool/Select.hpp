#pragma once

#include <memory>

#include "Tool.hpp"
#include "application/Math.hpp"
#include "map/Map.hpp"


class Select : public Tool
{
public:
	Select();
	~Select();

	void applySelection(const Map& map);

private:
	std::vector<std::weak_ptr<math::Vertex>> m_vertices;
};

