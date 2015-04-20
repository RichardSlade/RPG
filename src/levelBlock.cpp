#include <iostream>
#include <cassert>

#include "App/Utility.hpp"
#include "World/LevelBlock.hpp"

const sf::Color LevelBlock::mBrown = sf::Color(150, 70, 0, 255);
const sf::Color LevelBlock::mYellow = sf::Color(200, 200, 0, 255);
const sf::Color LevelBlock::mGreen = sf::Color(0, 150, 0, 255);

LevelBlock::LevelBlock(const sf::Texture& background
                       , sf::Vector2f pos
                       , sf::Vector2i index
                       , float size
                       , float rad)
    : mType(LevelBlock::Type::GrassBlock)
    , mBackground(background)
    , mScenery(nullptr)
    , mOriginCircle(5.f)
    , mBlockCorner(pos)
    , mBlockIndex(index)
    , mSize(size)
    , mRadius(rad)
{
    sf::FloatRect bounds = mOriginCircle.getLocalBounds();
    mOriginCircle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mOriginCircle.setFillColor(sf::Color::Yellow);
    mOriginCircle.setPosition(mSize / 2.f, mSize / 2.f);

    setPosition(pos);
    setBackgroundColour();
}

LevelBlock::LevelBlock(const sf::Texture& texture
                       , sf::Vector2f pos)
    : mBackground(texture)
{
    mBackground.setPosition(pos);
    setBackgroundColour();
}

void LevelBlock::drawCurrent(sf::RenderTarget& target
                             , sf::RenderStates states) const
{
    target.draw(mBackground, states);

    if(mScenery)
        target.draw(mScenery->getSprite(), states);
}

void LevelBlock::setBackgroundColour()
{
    const float MaxGrass = 100.f;
    float grass = rangedClamped(MaxGrass / 2.f, MaxGrass);

    sf::Color bottomColour, topColour, backgroundColour;
    float max, min;

    if(grass < MaxGrass / 2.f)
    {
        bottomColour = mBrown;
        topColour = mYellow;
        max = MaxGrass / 2.f;
        min = 0.f;
    }
    else
    {
        bottomColour = mYellow;
        topColour = mGreen;
        max = MaxGrass;
        min = MaxGrass / 2.f;
    }

    float topPercentage = (grass - min) / max;
    float botPercentage = 1.f - topPercentage;

    backgroundColour.r = (topColour.r * topPercentage) + (bottomColour.r * botPercentage);
    backgroundColour.g = (topColour.g * topPercentage) + (bottomColour.g * botPercentage);
    backgroundColour.b = 0.f;

    mBackground.setColor(backgroundColour);
}

LevelBlock* LevelBlock::insertEntity(Entity* entity)
{
    mEntitiesInBlock.push_back(entity);
    return this;
}

void LevelBlock::deleteEntity(Entity* entity)
{
    auto it = find (mEntitiesInBlock.begin(), mEntitiesInBlock.end(), entity);

    assert(it != mEntitiesInBlock.end());

    mEntitiesInBlock.erase(it);
}
