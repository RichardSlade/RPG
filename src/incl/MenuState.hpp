#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "AppState.hpp"

class Controller;

class MenuState : public AppState
{
private:
    enum MenuType
    {
        Main,
        Help,
        Quit,
        Start,
        NumMenuType
    };

    static const int            mMaxTextNum;
    static const sf::Color      mSelectedTextColour;
    static const sf::Color      mOtherTextColour;
    static const sf::Time       mUnderscoreResetTime;

    sf::View                    mView;
    sf::Vector2f                mViewCenter;

    const sf::Vector2f          mMenuTextPos;

    Controller&                 mController;
    sf::RenderWindow&           mWindow;

    sf::Sprite                  mBackground;
    sf::Text                    mTitleText;

    std::vector<sf::Text>       mMenuText;

    MenuType                    mCurrentMenu;
    MenuType                    mNewMenu;
    MenuType                    mPreviousMenu;

    int                         mCurrentMenuSelection;

    void                        handleMenuInput(int);
    void                        changeMenu(MenuType);
    int                         getMaxMenuSelection(MenuType);

    MenuType                    getSelectionMenuType(int);

public:
                                MenuState(Controller&
                                      , sf::RenderWindow&);

    virtual                     ~MenuState(){};

    virtual void                update(sf::Time);
    virtual void                handleInput();
    virtual void                display();
};

#endif // MENUSTATE_HPP

