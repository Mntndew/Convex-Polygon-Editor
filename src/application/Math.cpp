#include "Math.hpp"
#include <iostream>


namespace math
{
	bool lineIntersectsLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d)
	{
		float q = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
		float D = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);

		if (D == 0)
			return false;

		float r = q/D;
		q = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);
		float s = q/D;

		if (r < 0 || r > 1 || s < 0 || s > 1)
			return false;

		return true;
	}

	bool pointInPolygon(const sf::Vector2f& point, const Polygon& polygon)
	{
		auto bounds = polygon.getBounds();
		if (bounds.contains(point.x, point.y))
		{
			sf::Vector2f _point{point.x, (float)bounds.top - 10};

			int intersections = -1;
			for (int i = 0; i < polygon.getPointCount() - 1; ++i)
			{
				if (lineIntersectsLine(point, _point, polygon.getPoint(i), polygon.getPoint(i + 1)))
					intersections++;
			}

			return (intersections%2 == 0);
		}
		
		return false;
	}

	void normalize(sf::Vector2f& vector)
	{
		float magnitude = math::magnitude<float>(vector);
		vector.x /= magnitude;
		vector.y /= magnitude;
	}

	void projectPolygon(const sf::Vector2f& axis, const Polygon& polygon, float& min, float& max)
	{
		float dotProduct = math::dotProduct<float>(axis, polygon.getPoint(0));
		min = dotProduct;
		max = dotProduct;

		for (int i = 0; i < polygon.getPointCount(); ++i)
		{
			dotProduct = math::dotProduct<float>(axis, polygon.getPoint(i));

			if (dotProduct < min)
				min = dotProduct;
			else if (dotProduct > max)
				max = dotProduct;
		}
	}

	float intervalDistance(const float& minA, const float& maxA, const float& minB, const float& maxB)
	{
		if (minA < minB)
			return minB - maxA;
		else
			return minA - maxB;
	}

	bool polygonIntersectsPolygon(const Polygon& a, const Polygon& b)
	{
		float minIntervalDistance = std::numeric_limits<float>::infinity();
		sf::Vector2f translationAxis, edge, centerA = a.getCenter(), centerB = b.getCenter();

		for (int edgeIndex = 0; edgeIndex < a.getEdgeCount() + b.getEdgeCount(); edgeIndex++)
		{
			if (edgeIndex < a.getEdgeCount())
				edge = a.getEdge(edgeIndex);
			else
				edge = b.getEdge(edgeIndex - a.getEdgeCount());

			sf::Vector2f axis = sf::Vector2f(-edge.y, edge.x);
			math::normalize(axis);

			float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
			math::projectPolygon(axis, a, minA, maxA);
			math::projectPolygon(axis, b, minB, maxB);

			if (math::intervalDistance(minA, maxA, minB, maxB) >= 0)
				return false;
		}

		return true;
	}

	Intersection SAT(const Polygon& a, const Polygon& b, const sf::Vector2f& velocity)
	{
		Intersection result{true, true, {0, 0}};

		float minIntervalDistance = std::numeric_limits<float>::infinity();
		sf::Vector2f translationAxis, edge, centerA = a.getCenter(), centerB = b.getCenter();

		for (int edgeIndex = 0; edgeIndex < a.getEdgeCount() + b.getEdgeCount(); edgeIndex++)
		{
			if (edgeIndex < a.getEdgeCount())
				edge = a.getEdge(edgeIndex);
			else
				edge = b.getEdge(edgeIndex - a.getEdgeCount());

			sf::Vector2f axis = sf::Vector2f(-edge.y, edge.x);
			math::normalize(axis);

			float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
			math::projectPolygon(axis, a, minA, maxA);
			math::projectPolygon(axis, b, minB, maxB);

			if (math::intervalDistance(minA, maxA, minB, maxB) >= 0)
				result.intersect = false;

			float velocityProjection = math::dotProduct<float>(axis, velocity);

			if (velocityProjection < 0)
				minA += velocityProjection;
			else
				maxA += velocityProjection;

			float intervalDistance = math::intervalDistance(minA, maxA, minB, maxB);

			if (intervalDistance >= 0)
				result.willIntersect = false;

			if (!result.intersect && !result.willIntersect)
				break;

			intervalDistance = std::abs(intervalDistance);

			if (intervalDistance < minIntervalDistance)
			{
				minIntervalDistance = intervalDistance;
				translationAxis = axis;

				sf::Vector2f d = sf::Vector2f(centerA.x - centerB.x, centerA.y - centerB.y);
				if (math::dotProduct<float>(d, translationAxis) < 0)
					translationAxis = sf::Vector2f(-translationAxis.x, -translationAxis.y);
			}
		}

		if (result.willIntersect)
			result.minimumTranslationVector = sf::Vector2f(translationAxis.x*minIntervalDistance, translationAxis.y*minIntervalDistance);

//		std::cout << result.minimumTranslationVector.x << ':' << result.minimumTranslationVector.y << std::endl; 

		return result;
	}
}

