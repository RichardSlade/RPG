#include <SFML/Window/Event.hpp>

#include "App/MenuState.hpp"
#include "App/Controller.hpp"
#include "World/World.hpp"

const int MenuState::mMaxTextNum = 4;
const sf::Color MenuState::mSelectedTextColour = sf::Color::White;
const sf::Color MenuState::mOtherTextColour = sf::Color(255, 255, 255, 125);

MenuState::MenuState(Controller& cntrl
                     , sf::RenderWindow& window)
: mView(window.getView())
, mViewCenter(mView.getCenter())
, mMenuTextPos(mViewCenter.x
               , mViewCenter.y + 64)
, mController(cntrl)
, mWindow(window)
, mBackground(cntrl.getTexture(Controller::Textures::GameBackground)
                    , mWindow.getViewport(mView))
, mTitleText("RPG"
             , mController.getFont(Controller::Fonts::Sansation)
             , 128)
, mCurrentMenu(MenuState::MenuType::Main)
, mNewMenu(mCurrentMenu)
, mPreviousMenu(mCurrentMenu)
, mCurrentMenuSelection(0)
{
    mTitleText.setColor(mSelectedTextColour);

    float fontSize = mTitleText.getCharacterSize();

    sf::FloatRect bounds = mTitleText.getLocalBounds();
    mTitleText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mTitleText.setPosition(mViewCenter.x, mViewCenter.y - fontSize);

    for(size_t i = 0; i < mMaxTextNum; i++)
    {
        mMenuText.push_back(sf::Text(""
                                     , mController.getFont(Controller::Fonts::Sansation)
                                     , 64));
        mMenuText.at(i).setColor(mOtherTextColour);
    }

    changeMenu(mCurrentMenu);
}

void MenuState::handleMenuInput(int selectionChange)
{
    int maxMenu = getMaxMenuSelection(mCurrentMenu) - 1;

    mCurrentMenuSelection += selectionChange;

    if(mCurrentMenuSelection < 0)
        mCurrentMenuSelection = maxMenu;
    else if(mCurrentMenuSelection > maxMenu)
        mCurrentMenuSelection = 0;
}

void MenuState::changeMenu(MenuType newMenuType)
{
    for(sf::Text& txt : mMenuText)
        txt.setString("");

    switch(newMenuType)
    {
        case MenuState::MenuType::Main:
        {
            mMenuText.at(0).setString("Play");
            mMenuText.at(1).setString("Quit");
            break;
        }
        case MenuState::MenuType::Help:
        {
            mMenuText.at(0).setString("Back");
            break;
        }
        case MenuState::MenuType::Quit:
        {
            mWindow.close();
            break;
        }
        case MenuState::MenuType::Start:
        {
            mController.changeState();
            break;
        }
        default: break;
    }

    float inc = mMenuText.at(0).getCharacterSize();
    sf::Vector2f textPos = mMenuTextPos;

    for(sf::Text& txt : mMenuText)
    {
        sf::FloatRect bounds = txt.getLocalBounds();
        txt.setOrigin(bounds.width / 2.f, 0.f);
        txt.setPosition(textPos);

        textPos.y += inc;
    }

    mPreviousMenu = mCurrentMenu;
    mCurrentMenu = newMenuType;
    mCurrentMenuSelection = 0;
}

int MenuState::getMaxMenuSelection(MenuType menuType)
{
    switch(menuType)
    {
        case MenuState::MenuType::Main: return 2;
        case MenuState::MenuType::Help: return 1;
        default: return 0;
    }
}

MenuState::MenuType MenuState::getSelectionMenuType(int menuSelection)
{
    switch(mCurrentMenu)
    {
        case MenuState::MenuType::Main:
        {
            switch(menuSelection)
            {
                case 0: return MenuState::MenuType::Start;
//                case 1: return MenuState::MenuType::Help;
                case 1: return MenuState::MenuType::Quit;
                default: return MenuState::MenuType::Main;
            }
        }
        default: return MenuState::MenuType::Main;
    }
}

void MenuState::lockedUpdate(sf::Time dt)
{
    if(mCurrentMenu != mNewMenu)
        changeMenu(mNewMenu);

    for(sf::Text& txt : mMenuText)
        txt.setColor(mOtherTextColour);

    mMenuText.at(mCurrentMenuSelection).setColor(mSelectedTextColour);
}

void MenuState::handleInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            mWindow.close();
            continue;
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::Escape)
            {
                if(mCurrentMenu == MenuType::Main)
                {
                    mPreviousMenu = MenuType::Main;
                }
                else
                    mNewMenu = mPreviousMenu;
            }
            if(event.key.code == sf::Keyboard::Up)
                handleMenuInput(-1);
            else if(event.key.code == sf::Keyboard::Down)
                handleMenuInput(1);
            else if(event.key.code == sf::Keyboard::Return)
                mNewMenu = getSelectionMenuType(mCurrentMenuSelection);
        }
    }
}

void MenuState::display()
{
//    mWindow.clear(sf::Color(25, 25, 25));

    mWindow.draw(mBackground);
    mWindow.draw(mTitleText);

    for(sf::Text& txt : mMenuText)
        mWindow.draw(txt);

//    mWindow.display();
}

