#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <limits>
#include <string>

#include <SFML/System/Vector2.hpp>

const float MAXFLOAT = (std::numeric_limits<float>::max)();
const float MINFLOAT = (std::numeric_limits<float>::min)();

float magVec(sf::Vector2f vec);

sf::Vector2f normVec(sf::Vector2f vec);

void truncateVec(sf::Vector2f& vec
                 , float maxSpeed);

int signVec(sf::Vector2f
            , sf::Vector2f);

float dotVec(const sf::Vector2f vec1
             , const sf::Vector2f vec2);

float randomClamped();

float rangedClamped(int
                    , int);

float rangedClamped(float
                    , float);

void clampRotation(float&
                   , float
                   , float);

bool lineIntersection2D(sf::Vector2f
                        , sf::Vector2f
                        , sf::Vector2f
                        , sf::Vector2f
                        , float&
                        , sf::Vector2f&);

#endif // UTILITY_HPP
