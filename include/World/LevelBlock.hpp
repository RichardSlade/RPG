#ifndef LEVELBLOCK_HPP
#define LEVELBLOCK_HPP

#include <list>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

//#include "SceneNode/SceneNode.hpp"
//#include "World/Scenery.hpp"

class Entity;

class LevelBlock// : public SceneNode
{
public:
    typedef std::unique_ptr<LevelBlock> BlockPtr;

    enum Type
    {
        ExitBlock,
        GrassBlock,
        ObstacleBlock,
        WallBlock,
        CornerBlock,
        NumBlockTypes
    };

private:
//    static const sf::Color      mBrown;
//    static const sf::Color      mYellow;
//    static const sf::Color      mGreen;
//    static const sf::Color      mGray;
//    static const sf::Color      mDarkGray;

    Type                        mType;
//    sf::Sprite                  mBackground;
//    Scenery::SceneryPtr         mScenery;
    std::list<Entity*>          mEntitiesInBlock;

//    sf::CircleShape             mOriginCircle;
    sf::FloatRect               mBounds;

   sf::Vector2f                  mCorner;
    sf::Vector2f                mCenter;
    sf::Vector2i                mIndex;
    float                       mSize;
    float                       mRadius;

//    virtual void                drawCurrent(sf::RenderTarget&
//                                            , sf::RenderStates) const;

    void                        setBackgroundColour();

public:
                                 LevelBlock(LevelBlock::Type type,
                                           sf::Vector2f pos,
                                           sf::Vector2i index,
                                           float size)
                                 : mType(type)
                                 , mBounds(pos.x, pos.y, size, size)
                                 , mCorner(pos)
                                 , mCenter(pos.x + (size / 2.f), pos.y + (size / 2.f))
                                 , mIndex(index)
                                 , mSize(size)
                                 , mRadius(std::sqrt((size * size) + (size * size)) / 2.f)
                                 {};

//                                           , float radius);

//                                LevelBlock(const sf::Texture&
//                                           , sf::Vector2f);

   virtual                       ~LevelBlock(){};

//    virtual void                 getData() {};


   LevelBlock*                   insertEntity(Entity* entity)
                                 {
                                    mEntitiesInBlock.push_back(entity);
                                    return this;
                                 }

   void                          deleteEntity(Entity* entity)
                                 {
                                    auto it = find (mEntitiesInBlock.begin(), mEntitiesInBlock.end(), entity);
                                    assert(it != mEntitiesInBlock.end());
                                    mEntitiesInBlock.erase(it);
                                 }

//    Scenery*                    getScenery()
//                                { return mScenery.get(); }

    std::list<Entity*>           getEntities()
                                { return mEntitiesInBlock; }

    sf::Vector2f                getCorner()
                                { return mCorner; }

    sf::Vector2f                getCenter()
                                { return mCenter; }

    float                       getRadius() const
                                { return mRadius; }

    float                       getSize() const
                                { return mSize; }

    sf::Vector2i                getIndex()
                                { return mIndex; }


    LevelBlock::Type            getType() const
                                { return mType; }

//    const sf::Sprite&           getBackground() const
//                                { return mBackground; }

   sf::FloatRect                getBounds()
                                 {return mBounds; }

    // Setters
//    LevelBlock*                 insertEntity(Entity*);
//    void                        deleteEntity(Entity*);

//    void                        addScenery(Scenery::SceneryPtr scenery)
//                                { mScenery = std::move(scenery); }
//
    void                        setType(LevelBlock::Type newType)
                                { mType = newType; }
//
//    void                        setColour()
//                                { mBackground.setColor(sf::Color::Red); }
//
//    void                        resetColour()
//                                { mBackground.setColor(sf::Color::White); }
};

#endif // LEVELBLOCK_HPP
