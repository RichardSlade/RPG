#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "Node/SceneNode.hpp"

class SpriteNode : public SceneNode
{
protected:
   sf::Sprite                 mSprite;

   virtual void               updateCurrent(sf::Time dt) {};
   virtual void               drawCurrent(sf::RenderTarget&, sf::RenderStates) const;

public:
                              SpriteNode(const sf::Texture& t
                                       , sf::Vector2f pos
                                       , sf::IntRect rect
                                       , bool resetOrigin = false)
                              : mSprite(t, rect)
                              {
                                 if(resetOrigin)
                                 {
                                    sf::FloatRect bounds = mSprite.getLocalBounds();
                                    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                                 }

                                 mSprite.setPosition(pos);
                              };

                              SpriteNode(const sf::Texture& t
                                       , sf::Vector2f pos
                                       , float scale)
                              : mSprite(t)
                              {
                                 mSprite.scale(scale, scale);
                                 sf::FloatRect bounds = mSprite.getLocalBounds();
                                 mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                                 mSprite.setPosition(pos);
                              };

   virtual                    ~SpriteNode(){};

    // Getters
    sf::FloatRect               getGlobalBounds() {return mSprite.getGlobalBounds();}

    sf::FloatRect               getLocalBounds() {return mSprite.getLocalBounds();}

};

#endif // SPRITENODE_HPP

