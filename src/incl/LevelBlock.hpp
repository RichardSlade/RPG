#ifndef LEVELBLOCK_HPP
#define LEVELBLOCK_HPP

#include <list>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "SceneNode.hpp"
#include "Scenery.hpp"

class MovingEntity;

class LevelBlock : public SceneNode
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
    static const sf::Color      mBrown;
    static const sf::Color      mYellow;
    static const sf::Color      mGreen;

    Type                        mType;
    sf::Sprite                  mBackground;
    Scenery::SceneryPtr         mScenery;
    std::list<MovingEntity*>    mEntitiesInBlock;

    sf::CircleShape             mOriginCircle;

    sf::Vector2f                mBlockCorner;
    sf::Vector2i                mBlockIndex;
    float                       mSize;
    float                       mRadius;

    virtual void                drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

    void                        setBackgroundColour();

public:
                                LevelBlock(const sf::Texture&
                                           , sf::Vector2f
                                           , sf::Vector2i
                                           , float
                                           , float);

                                LevelBlock(const sf::Texture&
                                           , sf::Vector2f);

    virtual                     ~LevelBlock(){};


    Scenery*                    getScenery()
                                { return mScenery.get(); }

    std::list<MovingEntity*>    getEntities()
                                { return mEntitiesInBlock; }

    float                       getRadius() const
                                { return mRadius; }

    float                       getSize() const
                                { return mSize; }

    sf::Vector2i                getIndex()
                                { return mBlockIndex; }

    sf::Vector2f                getMiddle()
                                { return getWorldPosition() + mOriginCircle.getPosition(); }

    LevelBlock::Type            getType() const
                                { return mType; }

    const sf::Sprite&           getBackground() const
                                { return mBackground; }

    // Setters
    LevelBlock*                 insertEntity(MovingEntity*);
    void                        deleteEntity(MovingEntity*);

    void                        addScenery(Scenery::SceneryPtr scenery)
                                { mScenery = std::move(scenery); }

    void                        setType(LevelBlock::Type newType)
                                { mType = newType; }

    void                        setColour()
                                { mBackground.setColor(sf::Color::Red); }

    void                        resetColour()
                                { mBackground.setColor(sf::Color::White); }
};

#endif // LEVELBLOCK_HPP
