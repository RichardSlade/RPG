#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable
                , private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode>      upScNode;

    enum Layers
    {
        Background,
        Foreground,
        HUD,
        Num
    };

protected:
    SceneNode*                              mParent;
    std::vector<upScNode>                   mChildren;

    bool                                    mToRemove;

    virtual void                            updateCurrent(sf::Time){};
    virtual void                            drawCurrent(sf::RenderTarget&, sf::RenderStates) const {};
    virtual void                            draw(sf::RenderTarget&, sf::RenderStates) const;

    void                                    drawChildren(sf::RenderTarget&, sf::RenderStates) const;
    void                                    updateChildren(sf::Time);

public:
                                            SceneNode();
    virtual                                 ~SceneNode(){};

    void                                    update(sf::Time);

    void                                    addChild(upScNode);
    SceneNode::upScNode                     deleteChild(const SceneNode&);
    void                                    removeDeletedNodes();

    sf::Transform			                getWorldTransform() const;
    sf::Vector2f			                getWorldPosition() const;

    // Getters
    bool                                    getToRemove() const
                                            { return mToRemove; }
};

#endif // SCENENODE_HPP
