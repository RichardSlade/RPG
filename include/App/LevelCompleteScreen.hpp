#ifndef LEVELCOMPLETESCREEN_HPP
#define LEVELCOMPLETESCREEN_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

class GameState;
class Controller;

class LevelCompleteScreen : public sf::Drawable
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
	std::vector<sf::Text>      	mMenuText;

	int 						mCurrentMenuSelection;

	void						changeMenuSelection(int);
	void 						selectMenuOption(int);

public:
								LevelCompleteScreen(Controller&
											, GameState&
											, sf::RenderWindow&);

	virtual 					~LevelCompleteScreen(){};

	virtual void				draw(sf::RenderTarget&
									, sf::RenderStates) const;

	void 						update(sf::Time);
	void 						handleInput();

	// Setters
	void						setup(sf::View);
};

#endif // LEVELCOMPLETESCREEN_HPP
