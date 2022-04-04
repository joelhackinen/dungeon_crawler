#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* stateStack) : State(window, stateStack)
{
	window_ = window;
	stateStack_ = stateStack;
	isPause_ = false;

	player_.getPlayerShape()->setPosition(sf::Vector2f(map_.rooms_[0]->center_x_ * 50.f, map_.rooms_[0]->center_y_ * 50.f));

	float viewWidth = 800.f;
	gameView_.setSize(viewWidth, viewWidth * (9.f / 16.f));

	//Health bar
	healthBar_.setSize(sf::Vector2f(290.f, 35.f));
	healthBar_.setFillColor(sf::Color::Red);
	healthBar_.setPosition(sf::Vector2f(95.f, 20.f));

	//XP bar
	xpBar_.setSize(sf::Vector2f(290.f, 35.f));
	xpBar_.setFillColor(sf::Color::Magenta);
	xpBar_.setPosition(sf::Vector2f(95.f, 80.f));

	//Health bar background
	backgroundHpBar_.setSize(sf::Vector2f(290.f, 35.f));
	backgroundHpBar_.setFillColor(sf::Color(50, 50, 50, 150));
	backgroundHpBar_.setPosition(sf::Vector2f(95.f, 20.f));

	//XP bar background
	backgroundXpBar_ = backgroundHpBar_;
	backgroundXpBar_.setPosition(sf::Vector2f(95.f, 80.f));

	//Health bar frame
	hpFrameTexture_.loadFromFile("../images/health_ui.png");
	hpFrameTexture_.setSmooth(true);
	hpFrame_.setTexture(hpFrameTexture_);
	hpFrame_.setScale(sf::Vector2f(5.f, 5.f));

	//Text
	font_.loadFromFile("../fonts/VNBAHAB.ttf");
	hpText_.setFont(font_);
	hpText_.setFillColor(sf::Color::Red);
	levelText_.setFont(font_);
	levelText_.setFillColor(sf::Color::Magenta);
	hpText_.setString("100/100");
	levelText_.setString("1");
	hpText_.setPosition(sf::Vector2f(470.f, 21.f));
	levelText_.setPosition(sf::Vector2f(470.f, 81.f));

	//Item slots
	slot1_.setSize(sf::Vector2f(55.f, 55.f));
	slot1_.setFillColor(sf::Color::Transparent);
	slot2_ = slot1_;
	weaponSlot_ = slot1_;
	slot1_.setPosition(sf::Vector2f(765.f, 40.f));
	slot2_.setPosition(sf::Vector2f(840.f, 40.f));
	weaponSlot_.setPosition(sf::Vector2f(650.f, 40.f));

	//Minimap stuffs
	minimapFrameTexture_.loadFromFile("../images/minimapFrame.png");
	minimapFrameTexture_.setSmooth(true);
	minimapFrame_.setTexture(minimapFrameTexture_);
	minimapFrame_.setPosition(sf::Vector2f(1534.f, 0.f));

	//Pause panel stuffs
	pauseText_.setFont(font_);
	pauseText_.setString("- The game is paused. Unpause by pressing P -");
	pauseText_.setFillColor(sf::Color(180, 180, 180));
	pauseText_.setCharacterSize(60);
	pauseText_.setPosition(sf::Vector2f(960.f - pauseText_.getGlobalBounds().width / 2,
		540.f - pauseText_.getGlobalBounds().height / 2.f));

	//Game over stuffs
	panelTexture_.loadFromFile("../images/panel.png");
	buttonTexture_.loadFromFile("../images/small_button.png");
	buttonPressedTexture_.loadFromFile("../images/small_button_press.png");

	panelTexture_.setSmooth(true);
	buttonTexture_.setSmooth(true);
	buttonPressedTexture_.setSmooth(true);

	panel_.setTexture(panelTexture_);
	panel_.setPosition(sf::Vector2f(660.f, 320.f));
	quitButton_.setTexture(buttonTexture_);
	quitButton_.setScale(sf::Vector2f(2.f, 2.f));
	quitButton_.setPosition(sf::Vector2f(1170.f, 345.f));

	summaryText_.setFont(font_);
	summaryText_.setFillColor(sf::Color::Black);
	summaryText_.setCharacterSize(34);
}


GameState::~GameState()
{

}

void GameState::endState()
{
	std::cout << "Ending game state!" << std::endl;
}

void GameState::update(const float& dt)
{
	if (!isPause_ && player_.isAlive() && map_.enemies_.size() != 0) {
		duration_ += dt;
		//Check neighbouring squares
		std::vector<std::shared_ptr<Tile>> nbs = player_.getNeighboringTiles(map_);

		//Check for ESC
		checkQuit();
		player_.update(dt, map_);

		for (size_t i = 0; i < map_.enemies_.size(); i++) {
			map_.enemies_[i]->decideAction(player_, dt, map_);
		}

		//Pausing
		static bool pauseFlag = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !pauseFlag) {
			isPause_ = true;
		}

		//Movements
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player_.move(-1.f, 0.f, dt);
			if (playerCollision(nbs))
				player_.move(1.f, 0.f, dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player_.move(1.f, 0.f, dt);
			if (playerCollision(nbs))
				player_.move(-1.f, 0.f, dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player_.move(0.f, -1.f, dt);
			if (playerCollision(nbs))
				player_.move(0.f, 1.f, dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player_.move(0.f, 1.f, dt);
			if (playerCollision(nbs))
				player_.move(0.f, -1.f, dt);
		}

		//Shooting
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2f mPos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_)) - sf::Vector2f(window_->getSize()) / 2.f;
			sf::Vector2f normDir = mPos / (float)sqrt(pow(mPos.x, 2) + pow(mPos.y, 2));
			player_.shoot(normDir);
		}

		//Dropping weapon or items to the ground using right mouse
		static bool dropFlag = false;

		sf::Vector2f anotherMpos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
		if (slot1_.getGlobalBounds().contains(anotherMpos)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !dropFlag)
				player_.dropItem(map_.groundItems_, "healthpotion");
		}
		else if (slot2_.getGlobalBounds().contains(anotherMpos)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !dropFlag)
				player_.dropItem(map_.groundItems_, "speedpotion");
		}
		else if (weaponSlot_.getGlobalBounds().contains(anotherMpos)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !dropFlag)
				player_.dropWeapon(map_.groundWeapons_);
		}

		//Picking up weapon or items from the ground
		static bool pickFlag = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !pickFlag) {
			sortItemsByDistance();
			if (map_.groundItems_.size() > 0 && player_.getInventory().size() < 4 && map_.groundItems_[0]->playerDistance(player_) < 50.f) {
				if (player_.pickItem(map_.groundItems_[0])) {
					map_.groundItems_.erase(map_.groundItems_.begin());
				}
			}
			if (!player_.hasWeapon() && map_.groundWeapons_.size() > 0 && map_.groundWeapons_[0]->playerDistance(player_) < 50.f) {
				if (player_.pickWeapon(map_.groundWeapons_[0])) {
					map_.groundWeapons_.erase(map_.groundWeapons_.begin());
				}
			}
		}

		//Dropping the weapon using the key Q
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			player_.dropWeapon(map_.groundWeapons_);
		}

		//Using potion
		static bool potionFlag = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !potionFlag)
			player_.useItem("healthpotion");
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !potionFlag)
			player_.useItem("speedpotion");

		//Update the 3 flags for key pressed
		pauseFlag = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
		potionFlag = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
		dropFlag = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		pickFlag = sf::Keyboard::isKeyPressed(sf::Keyboard::E);

		//Update the HP and XP bar 
		healthBar_.setSize(sf::Vector2f(290.f * player_.getHpPercent(), healthBar_.getSize().y));
		xpBar_.setSize(sf::Vector2f(290.f * player_.getXPPercent(), xpBar_.getSize().y));
		gameView_.setCenter(player_.getPlayerShape()->getPosition());

		//Update the health and level to the UI
		std::stringstream hpString;
		std::stringstream levelString;

		hpString << player_.getHp() << "/" << player_.getMaxHp() << std::endl;
		levelString << player_.getLevel() << std::endl;
		hpText_.setString(hpString.str());
		levelText_.setString(levelString.str());

	}
	else if (isPause_) {
		static bool unpauseFlag = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !unpauseFlag) {
			isPause_ = false;
		}
		unpauseFlag = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
	}
	else if (!player_.isAlive()) {
		sf::Vector2f yetAnotherMpos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
		if (quitButton_.getGlobalBounds().contains(yetAnotherMpos)) {
			quitButton_.setTexture(buttonPressedTexture_);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				setQuit();
			}
		}
		else {
			quitButton_.setTexture(buttonTexture_);
		}
	}
	else if (map_.enemies_.size() == 0) {
		sf::Vector2f yetAnotherMpos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
		if (quitButton_.getGlobalBounds().contains(yetAnotherMpos)) {
			quitButton_.setTexture(buttonPressedTexture_);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				setQuit();
			}
		}
		else {
			quitButton_.setTexture(buttonTexture_);
		}
	}
}

void GameState::sortItemsByDistance() {
	sf::Vector2f ppos = player_.getPlayerShape()->getPosition();
	auto cmp_distance =
		[ppos](std::shared_ptr<Item> item1, std::shared_ptr<Item> item2)
	{return sqrt(pow(item1->getPosition().x - ppos.x, 2) + pow(item1->getPosition().y - ppos.y, 2)) < sqrt(pow(item2->getPosition().x - ppos.x, 2) + pow(item2->getPosition().y - ppos.y, 2)); };
	std::sort(map_.groundWeapons_.begin(), map_.groundWeapons_.end(), cmp_distance);
	std::sort(map_.groundItems_.begin(), map_.groundItems_.end(), cmp_distance);
}

bool GameState::playerCollision(std::vector< std::shared_ptr< Tile > >& nbs)
{
	sf::RectangleShape* shape = player_.getPlayerShape();
	auto check = [shape](std::shared_ptr<Tile> tile) { return !tile->isPassable() && tile->sprite_.getGlobalBounds().intersects(shape->getGlobalBounds()); };
	return std::any_of(nbs.begin(), nbs.end(), check);
}

void GameState::render()
{
	window_->setView(gameView_);
	renderMap();
	player_.render(window_);
	window_->setView(window_->getDefaultView()); //get the default view to render the UI
	renderUI();

	//Change the view and render the minimap
	minimapView_ = gameView_;
	minimapView_.setViewport(sf::FloatRect(0.809f, 0.02f, 0.18f, 0.18f));
	minimapView_.setSize(1600, 900);
	window_->setView(minimapView_);
	renderMap();
	window_->draw(*player_.getPlayerShape());

	//Reset the view to default
	window_->setView(window_->getDefaultView());

	//Render the pause panel and its buttons
	if (isPause_)
		window_->draw(pauseText_);

	if (!player_.isAlive()) {
		std::stringstream summary;
		summary << "You Lost! Better luck next time!" << "\n" << std::endl;
		summary << "Your level: " << player_.getLevel() << "\n" << std::endl;
		summary << "Total kills: " << player_.getTotalKill() << "\n" << std::endl;
		summary << "Game duration: " << duration_ << " seconds" << std::endl;
		summaryText_.setString(summary.str());
		summaryText_.setPosition(sf::Vector2f(680.f, 490.f));
		window_->draw(panel_);
		window_->draw(quitButton_);
		window_->draw(summaryText_);
	}

	if (map_.enemies_.size() == 0 && player_.isAlive()) {
		std::stringstream summary;
		summary << "You won! You killed all the monsters!" << "\n" << std::endl;
		summary << "Your level: " << player_.getLevel() << "\n" << std::endl;
		summary << "Total kills: " << player_.getTotalKill() << "\n" << std::endl;
		summary << "Game duration: " << duration_ << " seconds" << std::endl;
		summaryText_.setString(summary.str());
		summaryText_.setPosition(sf::Vector2f(680.f, 490.f));
		window_->draw(panel_);
		window_->draw(quitButton_);
		window_->draw(summaryText_);
	}
}

void GameState::renderMap()
{
	sf::Vector2f playerPos = player_.getPosition();
	//Rendering bounds. Much better performance since game doesn't have to render the whole map all the time
	int left = std::max(0, (int)floor(playerPos.x / 50.f) - 17);
	int right = std::min(map_.getWidth(), (int)floor(playerPos.x / 50.f) + 17);
	int top = std::max(0, (int)floor(playerPos.y / 50.f) - 10);
	int bottom = std::min(map_.getHeight(), (int)floor(playerPos.y / 50.f) + 10);

	for (int j = top; j < bottom; j++) {
		for (int i = left; i < right; i++) {
			window_->draw(map_.tiles_[j][i]->sprite_);
		}
	}
	for (size_t i = 0; i < map_.groundWeapons_.size(); i++) {
		window_->draw(map_.groundWeapons_[i]->sprite_);
	}
	for (size_t i = 0; i < map_.groundItems_.size(); i++) {
		window_->draw(map_.groundItems_[i]->sprite_);
	}
	for (size_t i = 0; i < map_.enemies_.size(); i++) {
		window_->draw(*map_.enemies_[i]->getShape());
	}
}

void GameState::renderUI()
{
	window_->draw(minimapFrame_);
	window_->draw(backgroundHpBar_);
	window_->draw(backgroundXpBar_);
	window_->draw(healthBar_);
	window_->draw(xpBar_);
	window_->draw(hpFrame_);
	window_->draw(hpText_);
	window_->draw(levelText_);

	//Render the hotkeys
	sf::Text hpHotKey;
	sf::Text spHotKey;

	hpHotKey.setFont(this->font_);
	hpHotKey.setFillColor(sf::Color::White);
	hpHotKey.setCharacterSize(18);
	spHotKey = hpHotKey;
	hpHotKey.setPosition(sf::Vector2f(765.f, 40.f));
	spHotKey.setPosition(sf::Vector2f(840.f, 40.f));
	hpHotKey.setString("1");
	spHotKey.setString("2");
	window_->draw(hpHotKey);
	window_->draw(spHotKey);

	//Render weapon slot
	if (player_.hasWeapon()) {
		sf::Sprite weaponSprite = player_.weapon_->sprite_;
		weaponSprite.setScale(1.35f, 1.35f);
		if (player_.weapon_->GetName() == "Bow")
			weaponSprite.setPosition(sf::Vector2f(685.f, 61.f));
		else
			weaponSprite.setPosition(sf::Vector2f(680.f, 67.f));
		weaponSprite.rotate(315.f);
		window_->draw(weaponSprite);
	}

	//Render the potion slot
	int hpPotion = 0;
	int spPotion = 0;
	sf::Text hpPotionCount;
	sf::Text spPotionCount;

	hpPotionCount.setFont(font_);
	hpPotionCount.setFillColor(sf::Color::Black);
	hpPotionCount.setCharacterSize(28);
	spPotionCount = hpPotionCount;

	for (auto& i : player_.getInventory()) {
		sf::Sprite tobeRender = i->sprite_;
		tobeRender.setScale(2.5f, 2.5f);
		if (i->GetName() == "healthpotion") {
			hpPotion++;
			tobeRender.setPosition(sf::Vector2f(791.f, 67.f));
			hpPotionCount.setString(std::to_string(hpPotion));
			hpPotionCount.setPosition(sf::Vector2f(765.f, 63.f));
		}
		else {
			spPotion++;
			tobeRender.setPosition(sf::Vector2f(866.f, 67.f));
			spPotionCount.setString(std::to_string(spPotion));
			spPotionCount.setPosition(sf::Vector2f(840.f, 63.f));
		}
		window_->draw(tobeRender);
	}
	window_->draw(hpPotionCount);
	window_->draw(spPotionCount);

	//Draw the inventory overlay
	window_->draw(slot1_);
	window_->draw(slot2_);
	window_->draw(weaponSlot_);
}
