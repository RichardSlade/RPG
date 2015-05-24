#ifndef SCENERY_HPP
#define SCENERY_HPP

#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "App/Utility.hpp"
#include "SceneNode/SceneNode.hpp"
//#include "SceneNode/SpriteNode.hpp"
#include "Physics/PhysicsBody.hpp"

class LevelBlock;

class Scenery : public SceneNode, public PhysicsBody
{
public:
    typedef std::unique_ptr<Scenery> upScenery;

    enum Type
    {
      Wall,
      All
    };

protected:
  Scenery::Type                 mSceneryType;
//    const LevelBlock*           mHostBlock;
    sf::Sprite                  mSprite;

    virtual void                updateCurrent(sf::Time dt)
                                {
                                  if(mBodyType == b2BodyType::b2_dynamicBody)
                                  {
                                    sf::Transformable::setPosition(meterToPixel(getWorldPosition()));
                                    sf::Transformable::setRotation(radianToDegree(getBodyRotation()));
                                  }
                                }

    virtual void                drawCurrent(sf::RenderTarget& target,
                                              sf::RenderStates states) const
                                {
                                  target.draw(mSprite, states);
                                }

public:
                                Scenery(const sf::Texture& texture,
                                        sf::Vector2f pos,
                                        sf::IntRect spriteRect,
                                        sf::Vector2f worldSize,
                                        b2Body* body,
                                        b2BodyType bodyType,
                                        Scenery::Type type)
                                : PhysicsBody(body,
                                              bodyType,
                                              pos,
                                              worldSize)
                                , mSceneryType(type)
                                , mSprite(texture,
                                          spriteRect)
                                {
                                  sf::FloatRect bounds = mSprite.getLocalBounds();
                                  mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

                                  sf::Transformable::setPosition(meterToPixel(pos));

//                                  mSprite.setPosition(meterToPixel(pos));

//                                  mBodyBounds = mSprite.getLocalBounds();
                                };

    virtual                     ~Scenery(){};

    Scenery::Type               getSceneryType() {return mSceneryType;}

    // Getters
//    virtual void                getSceneryData() const {};

//    const sf::Sprite&           getSprite()
//                                { return mSprite; }

    // Setters
//    void                        rotateSprite(float angle) {mSprite.rotate(angle);}
};

#endif // SCENERY_HPP
