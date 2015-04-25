#ifndef WALL_HPP
#define WALL_HPP

#include <memory>

#include "Scenery.hpp"

class Wall : public Scenery
{
public:
    typedef std::unique_ptr<Wall> WallPtr;

    typedef std::pair<sf::Vector2f, sf::Vector2f> PointPair;
    typedef std::pair<PointPair, sf::Vector2f> WallData;

    enum WallType
    {
        LeftWall,
        RightWall,
        TopWall,
        TopLeftWall,
        TopRightWall,
        BottomWall,
        NumWalls
    };

private:
    sf::Vector2f                    mPointA;
    sf::Vector2f                    mPointB;
    sf::Vector2f                    mNormal;

    WallData                        retrieveWallData();

public:

                                    Wall(LevelBlock*
                                         , const sf::Texture&
                                         , sf::Vector2f
                                         , sf::Vector2f
                                         , sf::Vector2f);

    virtual                         ~Wall(){};

    // Getters
    virtual WallData                getSceneryData()
                                    { return retrieveWallData(); };
};

#endif // WALL_HPP
