#ifndef LEVEL_HPP
#define LEVEL_HPP

//#include "World/LevelBlock.hpp"
#include "World/WallBlock.hpp"
#include "SceneNode/SceneNode.hpp"

class World;
class Controller;
class Entity;

class Level
{
private:
    typedef std::vector<LevelBlock::BlockPtr> ArrayBlock;
    typedef std::vector<ArrayBlock> LevelArray;

//    const int                           mExitWidth;

    LevelArray                          mLevelArray;
    sf::IntRect                         mWorldBounds;
    int                                 mBlockSize;
    int                                 mLevelX;
    int                                 mLevelY;
//    int                                 mMidX;

//    std::array<Wall::WallData
//                , Wall::NumWalls>       mBoundaryWallData;
//
//    std::vector<LevelBlock*>            mLevelExit;


   void                                 createWallBlock(int row, int column);
//    void                                defineWallData();
    sf::Vector2i                        worldCordsToIndex(sf::Vector2f) const;

    std::vector<LevelBlock*>            getInRangeBlocks(const Entity*
                                                         , float) const;
public:
                                        Level(int blockSize
//                                              , int exitWidth
                                              , sf::FloatRect worldBounds);

    // Getters
    std::vector<LevelBlock*>            getBlockTypeInRange(const Entity*
                                                            , float
                                                            , LevelBlock::Type) const;

    std::vector<Entity*>               getEntitiesInRange(const Entity*
                                                           , float radius
                                                           , int type) const;

    LevelBlock*                         getBlock(sf::Vector2i index) const
                                        { return mLevelArray.at(index.y).at(index.x).get(); }

    LevelBlock*                         getBlock(float x, float y) const
                                        { return mLevelArray.at(y).at(x).get(); }

    int                                 getLevelX()
                                        { return mLevelX; }

//    std::vector<LevelBlock*>            getLevelExit() const
//                                        { return mLevelExit; }

    // Setters
    void                                generateLevel(std::array<SceneNode*, SceneNode::Layers::Num>
                                                      , const Controller&);

    LevelBlock*                         insertEntityIntoLevel(Entity*) const;

//    void                                resetColours();
};

#endif // LEVEL_HPP
