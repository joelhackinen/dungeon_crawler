#ifndef STATE_H
#define STATE_H


#include "SFML/Graphics.hpp"
#include <stack>


class State
{
public:
	State(sf::RenderWindow* window, std::stack<State*>* stateStack);

	virtual ~State();

	virtual void update(const float& dt) = 0;

	virtual void render() = 0;

	const bool& getQuit() const;

	void setQuit();

	virtual void checkQuit();

	virtual void endState() = 0;

private:
	bool hasQuit_;
	sf::RenderWindow* window_;
	std::stack<State*>* stateStack_; //point to the stateStack in game.h
};

#endif
