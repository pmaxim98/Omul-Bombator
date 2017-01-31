#ifndef MATHUTILS_H
#define MATHUTILS_H

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <random>

namespace Math
{
	constexpr float PI = 3.14159265f;
	constexpr double PI_d = 3.141592653589;

	static std::mt19937 mt_rand;

	OMUL_BOMBATOR_LIB_API void initRandomGenerator();

	OMUL_BOMBATOR_LIB_API int genRandom(int ls, int rs);
	OMUL_BOMBATOR_LIB_API std::pair<bool, sf::Vector2f> solveQuadEc(float a, float b, float c);
	OMUL_BOMBATOR_LIB_API bool checkLineIntersection(sf::Vector2<double> seg1A, sf::Vector2<double> seg1B, sf::Vector2<double> seg2A, sf::Vector2<double> seg2B, sf::Vector2<double> & inter);
	OMUL_BOMBATOR_LIB_API bool checkSegToCircleIntersection(const sf::Vector2f & B, const sf::Vector2f & E, const sf::Vector2f & C, float r, sf::Vector2f & intersectionPoint);
	OMUL_BOMBATOR_LIB_API double getAngleInDegrees(sf::Vector2<double> O, sf::Vector2<double> A, sf::Vector2<double> B);
	OMUL_BOMBATOR_LIB_API float toDegs(float rads);
	OMUL_BOMBATOR_LIB_API float toRads(float degs);
	OMUL_BOMBATOR_LIB_API bool segmentIsAxis(sf::Vector2f A, sf::Vector2f B);
	OMUL_BOMBATOR_LIB_API int getQuadrant(sf::Vector2f source, sf::Vector2f point);
	OMUL_BOMBATOR_LIB_API int getQuadrant(float rads);
	OMUL_BOMBATOR_LIB_API bool lexico_compare(const sf::Vector2f & p1, const sf::Vector2f & p2);
	OMUL_BOMBATOR_LIB_API bool points_are_equal(const sf::Vector2f & p1, const sf::Vector2f & p2);
	OMUL_BOMBATOR_LIB_API void remove_duplicates(std::vector<sf::Vector2f> & points);
	OMUL_BOMBATOR_LIB_API sf::IntRect getRectAtFrame(sf::Uint16 frame, sf::Uint16 textureWidth, sf::Uint16 tileSize);
	OMUL_BOMBATOR_LIB_API sf::IntRect getRectAtFrame(sf::Uint16 frame, sf::Uint16 textureWidth, sf::Vector2<sf::Uint16> tileSize);

	template<typename T>
	T dist(sf::Vector2<T> p1, sf::Vector2<T> p2) {
		return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	}

	template<typename T>
	T dist(T x1, T y1, T x2, T y2)
	{
		return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	template<typename T>
	T sqrDist(sf::Vector2<T> p1, sf::Vector2<T> p2)
	{
		return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	}

	template<typename T>
	T sqrDist(T x1, T y1, T x2, T y2)
	{
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}

	template<typename T>
	inline T lerp(const T & lhs, const T & rhs, float t) {
		return (1.0f - t) * lhs + t * rhs;
	}

	OMUL_BOMBATOR_LIB_API sf::Color operator*(const sf::Color & lhs, float rhs);
	OMUL_BOMBATOR_LIB_API sf::Color operator*(float lhs, const sf::Color & rhs);
}

#endif