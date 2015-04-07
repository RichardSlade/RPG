#ifndef PAUSEDSCREEN_HPP
#define PAUSEDSCREEN_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

class GameState;
class Controller;

class PausedScreen : public sf::Drawable
{
private:
	enum MenuText
    {
        Resume,
        Quit,
        NumMenuText
    };

	GameState&				   	mHostGameState;
	sf::RenderWindow&		   	mWindow;
	sf::RectangleShape         	mBackground;
	sf::Text                   	mTitleText;
	std::vector<sf::Text>      	mMenuText;

	bool						mPaused;
	int 						mCurrentMenuSelection;

	void						changeMenuSelection(int);
	void 						selectMenuOption(int);

public:
								PausedScreen(Controller&
											, GameState&
											, sf::RenderWindow&);

	virtual 					~PausedScreen(){};

	virtual void				draw(sf::RenderTarget&
									, sf::RenderStates) const;

	void 						update(sf::Time);
	void 						handleInput();

	// Getters
	bool						isPaused()
								{ return mPaused; }
	// Setters
	void						setup(sf::View);
};

#endif // PAUSEDSCREEN_HPP
