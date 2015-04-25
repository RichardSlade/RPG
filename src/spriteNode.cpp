<<<<<<< HEAD
#include "Node/SpriteNode.hpp"
=======
#include "SceneNode/SpriteNode.hpp"
>>>>>>> working

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
