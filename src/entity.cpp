#include "World/Level.hpp"
#include "Entity/Entity.hpp"

Entity::Entity(EntityType type
             , Level* level
             , const sf::Texture& texture
             , const sf::Font& font
             , float scale);
: mType(type)
, mCurrentBlock(nullptr)
, mSprite(texture)
, mCurrentTarget(nullptr)
, mText("", font, 12)
, mLevel(level)
{
    mSprite.scale(scale, scale);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mRadius = std::max(bounds.width, bounds.height);

    setPosition(startPos);

    float theta = randomClamped() * (2.f * SteeringBehaviour::mPI);
    rotate(theta * (180 / SteeringBehaviour::mPI));
    mHeading = sf::Vector2f(std::sin(theta), -std::cos(theta));

    mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

std::vector<Entity*> Entity::getNeighbours(float radius
                                           , int type) const
{
    return mLevel->getEntitiesInRange(const_cast<Entity*>(this)
                                      , radius
                                      , type);
}

std::vector<LevelBlock*> MovingEntity::getBlockTypeInRange(LevelBlock::Type blockType, float radius) const
{
    return mLevel->getBlockTypeInRange(const_cast<MovingEntity*>(this), radius, blockType);
}

LevelBlock* Entity::getLevelBlock(sf::Vector2i index)
{
    return mLevel->getBlock(index);
}

//std::vector<LevelBlock*> Entity::getLevelExit()
//{
//    return mLevel->getLevelExit();
//}
