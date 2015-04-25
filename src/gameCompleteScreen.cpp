/*
*   GameCompleteScreen.cpp
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <SFML/Window/Event.hpp>

#include "App/GameCompleteScreen.hpp"
#include "App/Controller.hpp"
#include "App/GameState.hpp"

GameCompleteScreen::GameCompleteScreen(Controller& cntrl
							, GameState& gameState
							, sf::RenderWindow& window)
: mHostGameState(gameState)
, mWindow(window)
, mBackground(mWindow.getView().getSize())
, mTitleText("Time is Up!"
			  , cntrl.getFont(Controller::Fonts::Sansation)
			  , 128)
, mSheepStatText(""
			  , cntrl.getFont(Controller::Fonts::Sansation)
			  , 64)
, mCurrentMenuSelection(0)
{
	sf::Color bckgrndColour = sf::Color::Black;
    bckgrndColour.a -= 125;

    mBackground.setFillColor(bckgrndColour);
    sf::FloatRect bounds = mBackground.getLocalBounds();
    mBackground.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	bounds = mTitleText.getLocalBounds();
	mTitleText.setOrigin(bounds.width / 2.f
						, bounds.height / 2.f);

    std::vector<std::string> textForPausedMenu;
    textForPausedMenu.push_back("Restart");
    textForPausedMenu.push_back("Main Menu");

    for(unsigned int i = 0; i < MenuText::NumMenuText; i++)
    {
        sf::Text txt(textForPausedMenu.at(i)
                     , cntrl.getFont(Controller::Fonts::Sansation)
                     , 64);

        sf::FloatRect bounds = txt.getLocalBounds();
        txt.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        mMenuText.push_back(txt);
    }
}

void GameCompleteScreen::changeMenuSelection(int change)
{
	mCurrentMenuSelection += change;

	if(mCurrentMenuSelection < 0)
		mCurrentMenuSelection = 1;
	else if(mCurrentMenuSelection > 1)
		mCurrentMenuSelection = 0;
}

void GameCompleteScreen::selectMenuOption(int selection)
{
	switch(selection)
	{
		case 0: mHostGameState.resetGame(); break;
		case 1: mHostGameState.quitGameState(); break;
	}
}

void GameCompleteScreen::draw(sf::RenderTarget& targ
						, sf::RenderStates states) const
{
	targ.draw(mBackground);
	targ.draw(mTitleText);
	targ.draw(mSheepStatText);

	for(const sf::Text& txt : mMenuText)
		targ.draw(txt);
}

void GameCompleteScreen::update(sf::Time dt)
{
	sf::Color textColour(255, 255, 255, 125);

	for(sf::Text& text : mMenuText)
		text.setColor(textColour);

	mMenuText.at(mCurrentMenuSelection).setColor(sf::Color(255, 255, 255, 255));
}

void GameCompleteScreen::handleInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
			mWindow.close();
		else if(event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::Up)
				changeMenuSelection(-1);
			else if(event.key.code == sf::Keyboard::Down)
				changeMenuSelection(1);
			else if(event.key.code == sf::Keyboard::Return)
				selectMenuOption(mCurrentMenuSelection);
		}
	}
}

void GameCompleteScreen::setup(sf::View view
							   , int sheepHerded)
{
	mBackground.setPosition(view.getCenter());

	sf::Vector2f textPos(view.getCenter());

	float fontSize = mTitleText.getCharacterSize();
	textPos.y -= fontSize;
	mTitleText.setPosition(textPos);
	textPos.y += fontSize;

	mSheepStatText.setString("Sheep Herded " + std::to_string(sheepHerded));
	sf::FloatRect bounds = mSheepStatText.getLocalBounds();
	mSheepStatText.setOrigin(bounds.width / 2.f
							, 0.f);

	fontSize = mSheepStatText.getCharacterSize();
	mSheepStatText.setPosition(textPos);
	textPos.y += fontSize;

	int inc = mMenuText.at(0).getCharacterSize();

	textPos.y += inc;

	for(sf::Text& text : mMenuText)
	{
		text.setPosition(textPos);
		textPos.y += inc;
	}
}



