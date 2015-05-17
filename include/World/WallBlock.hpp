#ifndef WALLBLOCK_HPP
#define WALLBLOCK_HPP

//#include <memory>

//#include "Scenery.hpp"
#include "World/LevelBlock.hpp"

class WallBlock : public LevelBlock //public Scenery
{
public:
//    typedef std::unique_ptr<Wall> WallPtr;

   enum EdgeData
   {
      PointA,
      PointB,
      Normal,
      NumData
   };

   typedef std::array<sf::Vector2f, WallBlock::EdgeData::NumData> Edge;

//    typedef std::pair<PointPair, sf::Vector2f> WallData;

//    enum WallType
//    {
//        LeftWall,
//        RightWall,
//        TopWall,
//        TopLeftWall,
//        TopRightWall,
//        BottomWall,
//        NumWalls
//    };

//   enum EdgeType
//   {
//      Top,
//      Bottom,
//      Left,
//      Right,
//   };



private:
//    sf::Vector2f                    mPointA;
//    sf::Vector2f                    mPointB;
//    sf::Vector2f                    mNormal;

    std::vector<WallBlock::Edge>           mEdges;

//    WallData                        retrieveWallData();

public:

                                    WallBlock(LevelBlock::Type type,
                                             sf::Vector2f pos,
                                             sf::Vector2i index,
                                             float size,
                                             std::vector<WallBlock::Edge> edges)
                                    : LevelBlock(type,
                                                 pos,
                                                 index,
                                                 size)
                                    , mEdges(edges)
                                    {};

    virtual                         ~WallBlock(){};

    // Getters
    std::vector<WallBlock::Edge>   getData()
                                    { return mEdges; };
};

#endif // WALLBLOCK_HPP
