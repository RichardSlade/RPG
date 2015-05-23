#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

/*
*   Controller.hpp
*
*   Class uses to oversee application states changes
*   from menu to game and back again.
*   Additionally holds global resources (textures and fonts)
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <array>
#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include "App/AppState.hpp"
#include "App/Params.hpp"

class Controller : sf::NonCopyable
{
public:
   typedef std::unique_ptr<sf::Texture> upTexture;
   typedef std::unique_ptr<sf::Font> upFont;

    static const sf::Time           mFPS;

    enum Textures
    {
        Adventurer,
        Enemy,
        Grass,
        Wall,
        Corner,
        Exit,
        Brick,
        Stone,
//        Ground,
        NumTextures
    };

    enum Fonts
    {
        Sansation,
//        Smoke,
//        Enemyy,
        NumFonts
    };

private:
    Params                          mParams;

    const float                     mWindowX;
    const float                     mWindowY;

    sf::RenderWindow                mWindow;
    sf::Clock                       mClock;
//    sf::Time                        mCountDown;
//    sf::Time                        mTimeSinceLastUpdate;
    sf::Vector2f                    mResetViewCenter;

    sf::Text                        mStatisticsText;
    sf::Time				            mStatisticsUpdateTime;
   std::size_t				            mStatisticsNumFrames;

//    sf::RenderTexture               mBackgroundTexture;
    std::vector<Controller::upTexture>        mTextures;
    std::vector<Controller::upFont>           mFonts;

    AppState::StateType             mAppStateType;
    std::unique_ptr<AppState>       mCurrentAppState;

    std::string                     mUserName;
    bool                            mChangeState;


    void                            loadMedia();
    const sf::Texture     createBackgroundTexture();
    void                            changeAppState();

   void	                          updateStatistics(sf::Time dt);

public:
                                    Controller();

    void                            run();

    // Getters
    const sf::Texture&              getTexture(Textures type) const
                                    { return *mTextures.at(type).get(); }

    const sf::Font&                 getFont(Controller::Fonts type) const
                                    { return *mFonts.at(type).get(); }

    const Params&                   getParams() const
                                    { return mParams; }

    // Setters
    void                            resetView();

    void                            changeState()
                                    { mChangeState = true; }

    void                            setUserName(std::string name)
                                    { mUserName = name; }
};

#endif // CONTROLLER_HPP
