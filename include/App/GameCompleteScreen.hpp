#ifndef GAMECOMPLETESCREEN_HPP
#define GAMECOMPLETESCREEN_HPP

/*
*	GameCompleteScreen.hpp
*
*	Class used to display information at
*	level completion
*
*	@Author Richard Slade
*	@Date 12/2014
*/

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

class GameState;
class Controller;

class GameCompleteScreen : public sf::Drawable
{
private:
	enum MenuText
    {
        NextLevel,
        Quit,
        NumMenuText
    };

	GameState&				   	mHostGameState;
	sf::RenderWindow&		   	mWindow;
	sf::RectangleShape         	mBackground;
	sf::Text                   	mTitleText;
	sf::Text 					mSheepStatText;
	std::vector<sf::Text>      	mMenuText;

	int 						mCurrentMenuSelection;

	void						changeMenuSelection(int);
	void 						selectMenuOption(int);

public:
								GameCompleteScreen(Controller&
												  , GameState&
												  , sf::RenderWindow&);

	virtual 					~GameCompleteScreen(){};

	virtual void				draw(sf::RenderTarget&
									, sf::RenderStates) const;

	void 						update(sf::Time);
	void 						handleInput();

	// Setters
	void						setup(sf::View, int);
};

#endif // GAMECOMPLETESCREEN_HPP

