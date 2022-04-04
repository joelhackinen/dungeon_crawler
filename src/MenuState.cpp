#include "MenuState.h"
#include "GameState.h"


MenuState::MenuState(sf::RenderWindow* window, std::stack<State*>* stateStack) : State(window, stateStack)
{
	window_ = window;
	stateStack_ = stateStack;

	//Textures
	menuTexture_.loadFromFile("../images/menu.jpeg");
	buttonTexture_.loadFromFile("../images/menu_button.png");
	buttonPressedTexture_.loadFromFile("../images/menu_button_press.png");

	menuTexture_.setSmooth(true);
	buttonTexture_.setSmooth(true);
	buttonPressedTexture_.setSmooth(true);

	//Sprites
	menuBackground_.setTexture(menuTexture_);
	playButton_.setTexture(buttonTexture_);
	playButton_.setScale(sf::Vector2f(4.f, 4.f));
	playButton_.setPosition(sf::Vector2f(768.f, 476.f));
	quitButton_.setTexture(buttonTexture_);
	quitButton_.setScale(sf::Vector2f(4.f, 4.f));
	quitButton_.setPosition(sf::Vector2f(768.f, 620.f));

	//Text
	font_.loadFromFile("../fonts/VNBAHAB.ttf");
	playText_.setFont(font_);
	playText_.setString("PLAY");
	playText_.setFillColor(sf::Color::Black);
	playText_.setCharacterSize(55);
	playText_.setPosition(sf::Vector2f(960.f - playText_.getGlobalBounds().width / 2,
		526.f - playText_.getGlobalBounds().height / 2.f));
	quitText_.setFont(font_);
	quitText_.setString("QUIT");
	quitText_.setFillColor(sf::Color::Black);
	quitText_.setCharacterSize(55);
	quitText_.setPosition(sf::Vector2f(960.f - quitText_.getGlobalBounds().width / 2,
		670.f - quitText_.getGlobalBounds().height / 2.f));
}

MenuState::~MenuState()
{
}

void MenuState::endState()
{
	std::cout << "Ending menu state!" << std::endl;
}

void MenuState::update(const float& dt)
{
	//Update mouse position on the view 
	mousePos_ = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

	//Check if mouse is pointed to the "Play" button
	if (playButton_.getGlobalBounds().contains(mousePos_)) {
		playButton_.setTexture(buttonPressedTexture_); //set to pressed 
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			stateStack_->push(new GameState(window_, stateStack_)); //Proceed to GameState
			std::cout << "Entering game state!" << std::endl;
		}
	}
	else {
		playButton_.setTexture(buttonTexture_); //reset to normal
	}

	//Check if mouse is pointed to the "Exit" button
	if (quitButton_.getGlobalBounds().contains(mousePos_)) {
		quitButton_.setTexture(buttonPressedTexture_); //set to buttonPressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			setQuit();
		}
	}
	else {
		quitButton_.setTexture(buttonTexture_); //reset to normal
	}

	checkQuit();
}

void MenuState::render()
{
	window_->draw(menuBackground_);
	window_->draw(playButton_);
	window_->draw(playText_);
	window_->draw(quitButton_);
	window_->draw(quitText_);
}
