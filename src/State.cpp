#include "State.h"
#include <iostream>

State::State(sf::RenderWindow* window, std::stack<State*>* stateStack)
{
	window_ = window;
	hasQuit_ = false;
	stateStack_ = stateStack;
}

State::~State()
{
}

const bool& State::getQuit() const
{
	return hasQuit_;
}

void State::setQuit()
{
	hasQuit_ = true;
}

void State::checkQuit()
{
	static bool quitFlag = false; //static variable to keep track of the key pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !quitFlag) {
		setQuit();
	}
	//Check for the flag. Can't set quit to the state unless the flag is false again (the escape key is not pressed)
	quitFlag = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
}

