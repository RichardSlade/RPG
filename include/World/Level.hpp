#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "World/LevelBlock.hpp"
#include "World/Wall.hpp"
#include "SceneNode/SceneNode.hpp"

class World;
class Controller;
class Entity;

class Level
{
private:
    typedef std::vector<LevelBlock*> ArrayBlock;
    typedef std::vector<std::vector<LevelBlock*>> LevelArray;

    const int                           mExitWidth;

    LevelArray                          mLevelArray;
    sf::IntRect                         mWorldBounds;
    int                                 mBlockSize;
    int                                 mLevelX;
    int                                 mLevelY;
    int                                 mMidX;

    std::array<Wall::WallData
                , Wall::NumWalls>       mBoundaryWallData;

    std::vector<LevelBlock*>            mLevelExit;

    void                                defineWallData();
    sf::Vector2i                        worldCordsToIndex(sf::Vector2f) const;

    std::vector<LevelBlock*>            getInRangeBlocks(const Entity*
                                                         , float) const;
public:
                                        Level(int
                                              , int
                                              , sf::IntRect);

    // Getters
    std::vector<LevelBlock*>            getBlockTypeInRange(const Entity*
                                                            , float
                                                            , LevelBlock::Type) const;

    std::vector<Entity*>               getEntitiesInRange(const Entity*
                                                           , float radius
                                                           , int type) const;

    LevelBlock*                         getBlock(sf::Vector2i index) const
                                        { return mLevelArray.at(index.y).at(index.x); }

    int                                 getLevelX()
                                        { return mLevelX; }

    std::vector<LevelBlock*>            getLevelExit() const
                                        { return mLevelExit; }

    // Setters
    void                                generateLevel(std::array<SceneNode*, SceneNode::Layers::Num>
                                                      , const Controller&);

    LevelBlock*                         insertEntityIntoLevel(Entity*) const;

    void                                resetColours();
};

#endif // LEVEL_HPP
