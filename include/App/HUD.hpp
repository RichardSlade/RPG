#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Time.hpp>

class World;

class HUD : public sf::Drawable
{
public:
    enum DataType
    {
        Name,
        Enemy,
        Time,
        NumDataType
    };

private:
    const World*                            mWorld;
    sf::RectangleShape                      mHUDBackground;
    std::vector<sf::Text>                   mHUDText;

    virtual void                            draw(sf::RenderTarget&
                                                 , sf::RenderStates) const;

public:
                                            HUD(World*
                                                , const sf::Font&
                                                , sf::FloatRect
                                                , std::string = "");

    virtual                                 ~HUD(){};

    void                                    update();

    // Setters
    void                                    setHUDPosition(sf::FloatRect);
};

#endif // HUD_HPP
