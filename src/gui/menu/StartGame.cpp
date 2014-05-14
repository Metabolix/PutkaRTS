#include <functional>

#include "StartGame.hpp"

#include "gui/GUI.hpp"
#include "gui/widget/Button.hpp"
#include "gui/game/Game.hpp"

GUI::Menu::StartGame::StartGame(std::shared_ptr<Widget> parent_, std::shared_ptr<Connection::Client> client_):
	Menu(parent_),
	client(client_) {

	insert(new GUI::Widget::Button("Ready", 400, 300, 100, 30, std::bind(&Connection::Client::setReadyToInit, client)));
}

void GUI::Menu::StartGame::updateState(sf::RenderWindow& window) {
	Menu::updateState(window);
	client->update();
	if (client->getState() >= Connection::INIT) {
		GUI::currentWidget.reset(new GUI::Game::Game(client, window));
	}
}
