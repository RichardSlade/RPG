#include <iostream>
#include <list>
#include <string>

#include "App/Utility.hpp"
#include "App/Controller.hpp"
#include "World/Level.hpp"
#include "World/World.hpp"
#include "SceneNode/SceneNode.hpp"
#include "Entity/Entity.hpp"

Level::Level(int blockSize
//             , int exitWidth
             , sf::FloatRect worldBounds)
//: mExitWidth(exitWidth)
: mWorldBounds(worldBounds)
, mBlockSize(blockSize)
, mLevelX(mWorldBounds.width / mBlockSize)
, mLevelY(mWorldBounds.height / mBlockSize)
//, mMidX(mLevelX / 2)
{
    // Initialise level array to correct size
    for(int y = 0; y < mLevelY; y++)
    {
        mLevelArray.push_back(ArrayBlock());

        for(int x = 0; x < mLevelX; x++)
        {
            mLevelArray.at(y).push_back(nullptr);
        }
    }

   std::cout << "Level constructor - Level X : " << mLevelX <<std::endl;
   std::cout << "Level constructor - Level Y : " << mLevelY <<std::endl;

//    defineWallData();
}

void Level::createWallBlock(int row, int column)
{
   enum Faces{North, South, East, West, NumFaces};

   LevelBlock* currentBlock = getBlock(column, row);
   std::vector<WallBlock::Edge> edges;

   sf::Vector2f pos = currentBlock->getCorner();

   sf::Vector2f pointA, pointB, normal;

   for(unsigned int i = 0; i < Faces::NumFaces; i++)
   {
      switch(i)
      {
      case Faces::North:
      {
         pointA = pos;
         pointB = sf::Vector2f(pointA.x + mBlockSize, pointA.y);
         normal = normVec(pointB.x / 2.f, pointB.y - 1.f);
         break;
      }
      case Faces::South:
      {
         pointA = sf::Vector2f(pos.x, pos.y + mBlockSize);
         pointB = sf::Vector2f(pointA.x + mBlockSize, pointA.y);
         normal = normVec(pointB.x / 2.f, pointB.y + 1.f);
         break;
      }
      case Faces::East:
      {
         pointA = pos;
         pointB = sf::Vector2f(pointA.x, pointB.y + mBlockSize);
         normal = normVec(pointB.x - 1.f, pointB.y / 2.f);
         break;
      }
      case Faces::West:
      {
         pointA = sf::Vector2f(pos.x + mBlockSize, pos.y);
         pointB = sf::Vector2f(pointA.x, pointA.y + mBlockSize);
         normal = normVec(pointB.x + 1.f, pointB.y / 2.f);
         break;
      }
      default: break;
      }

      WallBlock::Edge edge;

      edge.at(WallBlock::EdgeData::PointA) = pointA;
      edge.at(WallBlock::EdgeData::PointB) = pointB;
      edge.at(WallBlock::EdgeData::Normal) = normal;

      edges.push_back(edge);
   }

//   LevelBlock::BlockPtr wall(new WallBlock(LevelBlock::Type::WallBlock
//                                          , currentBlock->getCorner()
//                                          , sf::Vector2i(column, row)
//                                          , mBlockSize
//                                          , edges));

//   mLevelArray.at(row).at(column).reset(std::move(wall));
   mLevelArray.at(row).at(column).reset(new WallBlock(LevelBlock::Type::WallBlock
                                          , currentBlock->getCorner()
                                          , sf::Vector2i(column, row)
                                          , mBlockSize
                                          , edges));

}

//void Level::defineWallData()
//{
//    sf::Vector2f pointA, pointB, norm;
//    Wall::PointPair points;
//
////    int exitBorder = (mExitWidth - 1) / 2;
//
//    // Left wall
//    pointA = sf::Vector2f(mBlockSize, 0.f);
//    pointB = sf::Vector2f(mBlockSize, mWorldBounds.height);
//    points = Wall::PointPair(pointA, pointB);
//    norm = sf::Vector2f(1.f, 0.f);
//
//    mBoundaryWallData.at(Wall::WallType::LeftWall) = Wall::WallData(points, norm);
//
//    // Right wall
//    pointA = sf::Vector2f(mWorldBounds.width - mBlockSize, 0.f);
//    pointB = sf::Vector2f(mWorldBounds.width - mBlockSize, mWorldBounds.height);
//    points = Wall::PointPair(pointA, pointB);
//    norm = sf::Vector2f(-1.f, 0.f);
//
//    mBoundaryWallData.at(Wall::WallType::RightWall) = Wall::WallData(points, norm);
//
//    // Top wall
//    pointA = sf::Vector2f(0.f, mBlockSize);
//    pointB = sf::Vector2f(mWorldBounds.width, mBlockSize);
//    points = Wall::PointPair(pointA, pointB);
//    norm = sf::Vector2f(0.f, 1.f);
//
//    mBoundaryWallData.at(Wall::WallType::TopWall) = Wall::WallData(points, norm);
//
////    // Top wall left
////    pointA = sf::Vector2f(0.f, mBlockSize);
////    pointB = sf::Vector2f(mBlockSize * (mMidX - exitBorder), mBlockSize);
////    points = Wall::PointPair(pointA, pointB);
////    norm = sf::Vector2f(0.f, 1.f);
////
////    mBoundaryWallData.at(Wall::WallType::TopLeftWall) = Wall::WallData(points, norm);
////
////     // Top wall right
////    pointA = sf::Vector2f(mBlockSize * (mMidX + exitBorder), mBlockSize);
////    pointB = sf::Vector2f(mWorldBounds.width, mBlockSize);
////    points = Wall::PointPair(pointA, pointB);
////    norm = sf::Vector2f(0.f, 1.f);
////
////    mBoundaryWallData.at(Wall::WallType::TopRightWall) = Wall::WallData(points, norm);
//
//    // Bottom wall
//    pointA = sf::Vector2f(0.f, mWorldBounds.height - mBlockSize);
//    pointB = sf::Vector2f(mWorldBounds.width, mWorldBounds.height - mBlockSize);
//    points = Wall::PointPair(pointA, pointB);
//    norm = sf::Vector2f(0.f, -1.f);
//
//    mBoundaryWallData.at(Wall::WallType::BottomWall) = Wall::WallData(points, norm);
//}

sf::Vector2i Level::worldCordsToIndex(sf::Vector2f pos) const
{
    int x = static_cast<int>(pos.x);
    int y = static_cast<int>(pos.y);

    sf::Vector2i index(x, y);
    index /= mBlockSize;

    if(index.x > mLevelX - 1
   || index.y > mLevelY - 1
   || index.x < 0
   || index.y < 0)
    throw std::runtime_error("ERROR: Index out of bounds in worldCordsToIndex() \nX: "
                             + std::to_string(index.x)
                             + " Y: "
                             + std::to_string(index.y)
                             + "\n");

    return index;
}

std::vector<LevelBlock*> Level::getInRangeBlocks(const Entity* entity
                                                 , float radius) const
{
    std::vector<LevelBlock*> inRangeBlocks;

    sf::Vector2i index = worldCordsToIndex(entity->getWorldPosition());

    int x, y, leftStart, bottomStart, right, top;

    int sizeY = mLevelY - 1;
    int sizeX = mLevelX - 1;
    int range = 1;

    leftStart = std::max(0, index.x - range);
    right = std::min(sizeX, index.x + range);
    bottomStart = std::max(0, index.y - range);
    top = std::min(sizeY, index.y + range);

    for(y = bottomStart; y <= top; y++)
    {
        for(x = leftStart; x <= right; x++)
        {
//            LevelBlock* curBlock = mLevelArray.at(y).at(x).get();
            LevelBlock* curBlock = getBlock(sf::Vector2i(x, y));

            float expandedRadius = curBlock->getRadius() + radius;
            sf::Vector2f toBlock = entity->getWorldPosition() - curBlock->getCenter();

            if(magVec(toBlock) < expandedRadius)
                inRangeBlocks.push_back(curBlock);
        }
    }

    return inRangeBlocks;
}

std::vector<LevelBlock*> Level::getBlockTypeInRange(const Entity* entity
                                                    , float radius
                                                    , LevelBlock::Type blockType) const
{
    std::vector<LevelBlock*> inRangeBlocks = getInRangeBlocks(entity
                                                              , radius);

    std::vector<LevelBlock*> returnBlocks;

    for(LevelBlock* lvlBlck : inRangeBlocks)
    {
        if(lvlBlck->getType() == blockType)
            returnBlocks.push_back(lvlBlck);
    }

    return returnBlocks;
}

std::vector<Entity*> Level::getEntitiesInRange(const Entity* entity
                                                , float neighbourhood
                                                , int type) const
{
    assert(entity);

    std::vector<Entity*> neighbourEntities;
    std::vector<LevelBlock*> inRangeBlocks = getInRangeBlocks(entity
                                                              , neighbourhood);

    const sf::Vector2f entityPos = entity->getWorldPosition();

    for(LevelBlock* lvlBlck : inRangeBlocks)
    {
        std::list<Entity*> entitiesInBlock = lvlBlck->getEntities();

        for(Entity* e : entitiesInBlock)
        {
            assert(e);

            if(e->getEntityType() == type
               || type == Entity::Type::AllTypes)
            {
               sf::Vector2f toNeighbour = e->getWorldPosition() - entityPos;

               if(magVec(toNeighbour) <= neighbourhood)
                  neighbourEntities.push_back(e);
            }
        }
    }

    return neighbourEntities;
}

void Level::generateLevel(std::array<SceneNode*, SceneNode::Layers::Num> sceneLayers
                          , const Controller& controller)
{
    int posX = 0;
    int posY = 0;
    sf::Vector2f blockPos;

//    float radius = std::sqrt((mBlockSize * mBlockSize)
//                             + (mBlockSize * mBlockSize)) / 2.f;
    int row, col;

    // Initialise level blocks as grass
    for(row = 0; row < mLevelY; row++)
    {
        for(col = 0; col < mLevelX; col++)
        {
            blockPos = sf::Vector2f(posX, posY);
            LevelBlock::BlockPtr levelBlock = LevelBlock::BlockPtr(new LevelBlock(LevelBlock::Type::GrassBlock
                                                                                 , blockPos
                                                                                  , sf::Vector2i(col, row)
                                                                                  , mBlockSize));
                                                                                  //, radius));



//            levelBlock->setType(LevelBlock::Type::GrassBlock);

//            mLevelArray.at(row).at(col) = levelBlock.get();
            mLevelArray.at(row).at(col) = std::move(levelBlock);
//            sceneLayers.at(SceneNode::Layers::Background)->addChild(std::move(levelBlock));

            posX += mBlockSize;
        }

        posX = 0;
        posY += mBlockSize;
    }


    // Fill in top wall and bottom walls
//    Wall::WallData wallData = mBoundaryWallData.at(Wall::WallType::TopWall);
//    float angleToRotate = 90.f;

    for(row = 0; row < mLevelY; row += mLevelY - 1)
    {
        for(col = 0; col < mLevelX; col++)
        {
            createWallBlock(row, col);


//            LevelBlock::BlockPtr oldBlock = std::move(mLevelArray.at(row).at(col));
//            oldBlock = nullptr;

//            mLevelArray.at(row).at(col) = std::move(wall);
//            wall->rotateSprite(angleToRotate);
//            levelBlock->addScenery(std::move(wall));
        }

//        wallData = mBoundaryWallData.at(Wall::WallType::BottomWall);
//        angleToRotate = -90.f;

    }

//    Wall::WallData wallData = mBoundaryWallData.at(Wall::WallType::TopLeftWall);
//    float angleToRotate = 90.f;
//
//    for(row = 0; row < mLevelY; row += mLevelY - 1)
//    {
//        for(col = 0; col < mLevelX; col++)
//        {
//            LevelBlock* levelBlock = mLevelArray.at(row).at(col);
//            levelBlock->setType(LevelBlock::Type::WallBlock);
//
//            Wall::WallPtr wall(new Wall(levelBlock
//                                   , controller.getTexture(Controller::Textures::Wall)
//                                   , wallData.first.first
//                                   , wallData.first.second
//                                   , wallData.second));
//
//            wall->rotateSprite(angleToRotate);
//            levelBlock->addScenery(std::move(wall));
//        }
//
//        wallData = mBoundaryWallData.at(Wall::WallType::BottomWall);
//        angleToRotate = -90.f;
//
//    }

//    int exitIndex = mLevelX / 2.f;
//    angleToRotate = 90.f;
//    wallData = mBoundaryWallData.at(Wall::WallType::TopRightWall);
//
//    for(col = exitIndex + 2; col < mLevelX; col++)
//    {
//        LevelBlock* levelBlock = mLevelArray.at(0).at(col);
//        levelBlock->setType(LevelBlock::Type::WallBlock);
//
//        Wall::WallPtr wall(new Wall(levelBlock
//                           , controller.getTexture(Controller::Textures::Wall)
//                           , wallData.first.first
//                           , wallData.first.second
//                           , wallData.second));
//
//        wall->rotateSprite(angleToRotate);
//        levelBlock->addScenery(std::move(wall));
//    }

    // Fill in left and right walls
//    wallData = mBoundaryWallData.at(Wall::WallType::LeftWall);
//    angleToRotate = 0.f;

    for(col = 0; col < mLevelX; col += mLevelX - 1)
    {
        for(row = 0; row < mLevelY; row++)
        {
           createWallBlock(row, col);

//            LevelBlock* levelBlock = getBlock(col, row);
//            levelBlock->setType(LevelBlock::Type::WallBlock);
//
//            Wall::WallPtr wall(new Wall(levelBlock
//                                   , controller.getTexture(Controller::Textures::Wall)
//                                   , wallData.first.first
//                                   , wallData.first.second
//                                   , wallData.second));
//
//            wall->rotateSprite(angleToRotate);
//            levelBlock->addScenery(std::move(wall));
        }

//        wallData = mBoundaryWallData.at(Wall::WallType::RightWall);
//        angleToRotate = 180.f;
    }

    // Add corners
//    angleToRotate = 0.f;
//    bool firstLoop = false;
//
//    for(int row = 0; row < mLevelY; row += mLevelY - 1)
//    {
//        for(int col = 0; col < mLevelY; col += mLevelX - 1)
//        {
//            LevelBlock* levelBlock = getBlock(col, row);
//            Scenery::SceneryPtr corner(new Scenery(levelBlock
//                                                   , controller.getTexture(Controller::Textures::Corner)));
//
//            corner->rotateSprite(angleToRotate);
////            levelBlock->addScenery(std::move(corner));
//            levelBlock->setType(LevelBlock::Type::CornerBlock);
//
//            if(!firstLoop)
//                angleToRotate += 90.f;
//            else
//                angleToRotate -= 90.f;
//        }
//
//        firstLoop = true;
//        angleToRotate = -90.f;
//    }

//    int exitBorder = (mExitWidth - 1) / 2;

//    for(col = exitIndex - exitBorder; col < exitIndex + exitBorder; col ++)
//    {
//        LevelBlock* levelBlock = mLevelArray.at(0).at(col);
//            Scenery::SceneryPtr exit(new Scenery(levelBlock
//                                                   , controller.getTexture(Controller::Textures::Exit)));
//
//        levelBlock->addScenery(std::move(exit));
//        levelBlock->setType(LevelBlock::Type::ExitBlock);
//        mLevelExit.push_back(levelBlock);
//    }
}

LevelBlock* Level::insertEntityIntoLevel(Entity* entity) const
{
    sf::Vector2i index = worldCordsToIndex(entity->getWorldPosition());
    return mLevelArray.at(index.y).at(index.x)->insertEntity(entity);
}

//void Level::resetColours()
//{
//    for(int i = 0; i < mLevelY - 1; i++)
//    {
//        for(int j = 0; j < mLevelX - 1; j++)
//        {
//            mLevelArray.at(i).at(j)->resetColour();
//        }
//    }
//}

