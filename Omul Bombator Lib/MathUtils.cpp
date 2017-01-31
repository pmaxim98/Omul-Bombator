#include "MathUtils.h"

#include <chrono>
#include <algorithm>
#include <utility>
#include <iostream>

namespace Math
{
	sf::Color operator*(const sf::Color & lhs, float rhs)
	{
		sf::Color temp;
		temp.r = static_cast<sf::Uint8>(lhs.r * rhs);
		temp.g = static_cast<sf::Uint8>(lhs.g * rhs);
		temp.b = static_cast<sf::Uint8>(lhs.b * rhs);
		temp.a = static_cast<sf::Uint8>(lhs.a * rhs);
		return temp;
	}

	sf::Color operator*(float lhs, const sf::Color & rhs)
	{
		sf::Color temp;
		temp.r = static_cast<sf::Uint8>(rhs.r * lhs);
		temp.g = static_cast<sf::Uint8>(rhs.g * lhs);
		temp.b = static_cast<sf::Uint8>(rhs.b * lhs);
		temp.a = static_cast<sf::Uint8>(rhs.a * lhs);
		return temp;
	}

	void initRandomGenerator() 
	{
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		mt_rand.seed(seed);
	}

	int genRandom(int ls, int rs)
	{ 
		std::uniform_int_distribution<int> dist(ls, rs);
		return dist(mt_rand);
	};

	bool checkLineIntersection(sf::Vector2<double> seg1A, sf::Vector2<double> seg1B, sf::Vector2<double> seg2A, sf::Vector2<double> seg2B, sf::Vector2<double> & inter)
	{
		double s1_x, s1_y, s2_x, s2_y, den, t1, t2;

		s1_x = seg1B.x - seg1A.x;     s1_y = seg1B.y - seg1A.y;
		s2_x = seg2B.x - seg2A.x;     s2_y = seg2B.y - seg2A.y;

		den = -s2_x * s1_y + s1_x * s2_y;
		if(den < 0.00001f) return false;

		t1 = seg1A.x - seg2A.x;
		t2 = seg1A.y - seg2A.y;

		double s, t;
		s = (-s1_y * t1 + s1_x * t2) / den;
		t = ( s2_x * t2 - s2_y * t1) / den;

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
			inter.x = seg1A.x + (t * s1_x);
			inter.y = seg1A.y + (t * s1_y);
			return true;
		}

		return false;
	};

	std::pair<bool, sf::Vector2f> solveQuadEc(float a, float b, float c)
	{
		const float delta = b * b - 4 * a * c;

		if(delta < 0.0f)
			return {false, sf::Vector2f(0.0f, 0.0f)};

		if(delta > 0.0f) {
			const float sdelta = sqrt(delta);
			return {true, sf::Vector2f((-b - sdelta) / (2 * a), (-b + sdelta) / (2 * a))};
		}

		const float res = -b / (2 * a);
		return {true, sf::Vector2f(res, res)};
	}

	bool checkSegToCircleIntersection(const sf::Vector2f & B, const sf::Vector2f & E, const sf::Vector2f & C, float r, sf::Vector2f & intersectionPoint)
	{
		const sf::Vector2f end(E - B);
		const sf::Vector2f center(C - B);
		const sf::Vector2f beg(0.0f, 0.0f);

		const float m = end.y / end.x;
		const float a = m * m + 1;
		const float b = -2 * (center.x + m * center.y);
		const float c = center.x * center.x + center.y * center.y - r * r;

		const auto resY = solveQuadEc(a, b, c);

		if(resY.first) {
			intersectionPoint.x = resY.second.x;
			intersectionPoint.y = intersectionPoint.x * m;

			if((intersectionPoint.x <= 0 && intersectionPoint.x < end.x) || (intersectionPoint.x > 0 && intersectionPoint.x > end.x) ||
				(intersectionPoint.y <= 0 && intersectionPoint.y < end.y) || (intersectionPoint.y > 0 && intersectionPoint.y > end.y))
				return false;

			sf::Vector2f secondIntersectionPoint(resY.second.y, resY.second.y * m);
			if(sqrDist(intersectionPoint, beg) > sqrDist(secondIntersectionPoint, beg))
				intersectionPoint = secondIntersectionPoint;

			return true;
		}

		return false;
	}

	double getAngleInDegrees(sf::Vector2<double> O, sf::Vector2<double> A, sf::Vector2<double> B)
	{
		double xAO = A.x - O.x;
		double xBO = B.x - O.x;
		double yAO = A.y - O.y;
		double yBO = B.y - O.y;
		double m12 = xAO * xAO + yAO * yAO;
		double m13 = xBO * xBO + yBO * yBO;
		double theta = std::acos((xAO * xBO + yAO * yBO) / std::sqrt(m12 * m13));
		return theta * 180.f / PI;
	}

	float toDegs(float rads)
	{
		return rads * 180.f / PI;
	}

	float toRads(float degrees)
	{
		return degrees * PI / 180.f;
	}

	bool segmentIsAxis(sf::Vector2f A, sf::Vector2f B)
	{
		return (std::abs(A.x - B.x) < 1.f) || (std::abs(A.y - B.y) < 1.f);
	}

	int getQuadrant(float rads)
	{
		if(rads < 0.f) 
			return static_cast<int>(2 * rads / PI + 5);
		return static_cast<int>(2 * rads / PI + 1);
	}

	int getQuadrant(sf::Vector2f source, sf::Vector2f point)
	{
		if(point.x >= source.x && point.y <= source.y)
			return 1;
		else if(point.x <= source.x && point.y <= source.y)
			return 2;
		else if(point.x <= source.x && point.y >= source.y)
			return 3;
		return 4;
	}

	bool lexico_compare(const sf::Vector2f & p1, const sf::Vector2f & p2) {
		if(p1.x < p2.x) { return true; }
		if(p1.x > p2.x) { return false; }
		return (p1.y < p2.y);
	}

	bool points_are_equal(const sf::Vector2f & p1, const sf::Vector2f & p2) {
		return (std::abs(p1.x - p2.x) < 0.001f && std::abs(p1.y - p2.y) < 0.001f);
	}

	void remove_duplicates(std::vector<sf::Vector2f> & points) {
		std::sort(points.begin(), points.end(), lexico_compare);
		points.erase(std::unique(points.begin(), points.end(), points_are_equal), points.end());
	}

	sf::IntRect getRectAtFrame(sf::Uint16 frame, sf::Uint16 textureWidth, sf::Uint16 tileSize)
	{
		sf::Uint16 tilesPerRow = textureWidth / tileSize;
		return sf::IntRect(sf::Vector2i(frame % tilesPerRow * tileSize, frame / tilesPerRow * tileSize), sf::Vector2i(tileSize, tileSize));
	}

	sf::IntRect getRectAtFrame(sf::Uint16 frame, sf::Uint16 textureWidth, sf::Vector2<sf::Uint16> tileSize)
	{
		sf::Uint16 tilesPerRow = textureWidth / tileSize.x;
		return sf::IntRect(sf::Vector2i(frame % tilesPerRow * tileSize.x, frame / tilesPerRow * tileSize.y), sf::Vector2i(tileSize.x, tileSize.y));
	}
}