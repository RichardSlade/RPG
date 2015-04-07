#include <SFML/Window/Event.hpp>

#include "incl/PausedScreen.hpp"
#include "incl/Controller.hpp"
#include "incl/GameState.hpp"

PausedScreen::PausedScreen(Controller& cntrl
							, GameState& gameState
							, sf::RenderWindow& window)
: mHostGameState(gameState)
, mWindow(window)
, mBackground(mWindow.getView().getSize())
, mTitleText("Paused"
			  , cntrl.getFont(Controller::Fonts::Sansation)
			  , 128)
, mPaused(false)
, mCurrentMenuSelection(0)
{
	sf::Color bckgrndColour = sf::Color::Black;
    bckgrndColour.a -= 125;

    mBackground.setFillColor(bckgrndColour);
    sf::FloatRect bounds = mBackground.getLocalBounds();
    mBackground.setOrigin(bounds.width / 2.f
						, bounds.height / 2.f);

	bounds = mTitleText.getLocalBounds();
	mTitleText.setOrigin(bounds.width / 2.f
						, bounds.height / 2.f);

    std::vector<std::string> textForPausedMenu;
    textForPausedMenu.push_back("Resume");
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

void PausedScreen::changeMenuSelection(int change)
{
	mCurrentMenuSelection += change;

	if(mCurrentMenuSelection < 0)
		mCurrentMenuSelection = 1;
	else if(mCurrentMenuSelection > 1)
		mCurrentMenuSelection = 0;
}

void PausedScreen::selectMenuOption(int selection)
{
	switch(selection)
	{
		case 0: mPaused = false; break;
		case 1: mHostGameState.quitGameState(); break;
	}
}

void PausedScreen::draw(sf::RenderTarget& targ
						, sf::RenderStates states) const
{
	targ.draw(mBackground);
	targ.draw(mTitleText);

	for(const sf::Text& txt : mMenuText)
		targ.draw(txt);
}

void PausedScreen::update(sf::Time dt)
{
	sf::Color textColour(255, 255, 255, 125);

	for(sf::Text& text : mMenuText)
		text.setColor(textColour);

	mMenuText.at(mCurrentMenuSelection).setColor(sf::Color(255, 255, 255, 255));
}

void PausedScreen::handleInput()
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
			else if(event.key.code == sf::Keyboard::Escape)
				mPaused = false;
		}
	}
}

void PausedScreen::setup(sf::View view)
{
	mPaused = true;
	mBackground.setPosition(view.getCenter());

	sf::Vector2f textPos(view.getCenter());

	float fontSize = mTitleText.getCharacterSize();

	textPos.y -= fontSize;
	mTitleText.setPosition(textPos);

	int inc = mMenuText.at(0).getCharacterSize();

	textPos = view.getCenter();
	textPos.y += inc;

	for(sf::Text& text : mMenuText)
	{
		text.setPosition(textPos);
		textPos.y += inc;
	}
}

