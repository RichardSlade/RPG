#include "World/Scenery.hpp"
#include "World/QuadTree.hpp"

Scenery::Scenery(const sf::Texture& texture,
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

void Scenery::addToQuadTree(QuadTree* quadTree)
{
  // Add children
  SceneNode::addToQuadTree(quadTree);

  quadTree->insert(this);
}

void Scenery::updateCurrent(sf::Time dt)
{
  if(mBodyType == b2BodyType::b2_dynamicBody)
  {
    sf::Transformable::setPosition(meterToPixel(getWorldPosition()));
    sf::Transformable::setRotation(radianToDegree(getBodyRotation()));
  }
}

void Scenery::drawCurrent(sf::RenderTarget& target,
                          sf::RenderStates states) const
{
  target.draw(mSprite, states);
}
