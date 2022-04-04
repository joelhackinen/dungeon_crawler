#include "game.h"


Game::Game()
{
	window_ = new sf::RenderWindow(sf::VideoMode(), "application", sf::Style::Fullscreen);
	window_->setFramerateLimit(60);
	window_->setVerticalSyncEnabled(true);
	dt_ = -1;

	stateStack_.push(new MenuState(window_, &stateStack_));;
}

Game::~Game() {
	delete window_;
	while (!stateStack_.empty()) {
		delete stateStack_.top();
		stateStack_.pop();
	}
}

void Game::run() {
	while (window_->isOpen()) {
		updateDt();
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (window_->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window_->close();
		}
	}
}

void Game::update() {
	if (!stateStack_.empty()) {
		stateStack_.top()->update(dt_);
		if (stateStack_.top()->getQuit()) {
			stateStack_.top()->endState();
			delete stateStack_.top();
			stateStack_.pop();
		}
	}
	else {
		std::cout << "Ending application!" << std::endl;
		window_->close();
	}
}

void Game::render() {
	window_->clear();

	if (!stateStack_.empty()) {
		stateStack_.top()->render();
	}

	window_->display();
}


void Game::updateDt()
{
	dt_ = this->clock_.restart().asSeconds();
}
