#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <limits>
#include <string>

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

const float MAXFLOAT = (std::numeric_limits<float>::max)();
const float MINFLOAT = (std::numeric_limits<float>::min)();

const float PI = 3.14159265359;

const float PixelPerMeter = 40.f;
const float PhysicsScale = 1.f / PixelPerMeter;

float magVec(sf::Vector2f vec);

sf::Vector2f normVec(sf::Vector2f vec);

sf::Vector2f normVec(float x, float y);

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

sf::Vector2f meterToPixel(b2Vec2 meter);

sf::Vector2f meterToPixel(sf::Vector2f meter);

float meterToPixel(float meter);

sf::Vector2f pixelToMeter(sf::Vector2f meter);

float pixelToMeter(float meter);

sf::Vector2f convertVec(b2Vec2 vec);

b2Vec2 convertVec(sf::Vector2f vec);

float degreeToRadian(float degree);

float radianToDegree(float radian);

#endif // UTILITY_HPP
