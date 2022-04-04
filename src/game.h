#ifndef GAME_H
#define GAME_H


#include "GameState.h"
#include <stack>
#include "MenuState.h"


class Game {
public:
	Game();

	virtual ~Game();

	void run();

	void processEvents();

	void update();

	void updateDt(); //updating delta time

	void render();

private:
	float dt_; //delta time
	sf::RenderWindow* window_;
	sf::Clock clock_;
	std::stack<State*> stateStack_;

};



#endif
