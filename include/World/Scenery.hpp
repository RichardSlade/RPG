#ifndef SCENERY_HPP
#define SCENERY_HPP

#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "SceneNode/SceneNode.hpp"

class LevelBlock;

class Scenery
{
public:
    typedef std::unique_ptr<Scenery> SceneryPtr;

protected:
    const LevelBlock*           mHostBlock;
    sf::Sprite                  mSprite;

public:
                                Scenery(LevelBlock* host
                                        , const sf::Texture& texture)
                                : mHostBlock(host)
                                , mSprite(texture)
                                {
                                    sf::FloatRect bounds = mSprite.getLocalBounds();
                                    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

                                    mSprite.move(20.f, 20.f);
                                };

    virtual                     ~Scenery(){};

    // Getters
    virtual void                getSceneryData() const {};

    const sf::Sprite&           getSprite()
                                { return mSprite; }

    // Setters
    void                        rotateSprite(float angle)
                                {
                                    mSprite.rotate(angle);
                                }
};

#endif // SCENERY_HPP
