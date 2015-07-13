#include <cmath>
#include <cwctype>

#include "TextField.hpp"

GUI::Widget::TextField::TextField(
	const std::string& str,
	float x,
	float y,
	float width,
	float height
) : Widget(x, y, width, height) {
	text.setString(stringFromUtf8(str));
	cursorIndex = text.getString().getSize();

	text.setCharacterSize(fontSize());
	text.setColor(Color::text);
	text.setFont(font);
}

void GUI::Widget::TextField::setText(const std::string& str) {
	text.setString(stringFromUtf8(str));
	cursorIndex = text.getString().getSize();
}

std::string GUI::Widget::TextField::getText() const {
	return utf8FromString(text.getString());
}

bool GUI::Widget::TextField::handleEvent(
	const sf::Event& e,
	const sf::RenderWindow& window
) {
	if (e.type == sf::Event::TextEntered) {
		if (std::iscntrl(e.text.unicode)) {
			return false;
		}

		if (
			textWidth(e.text.unicode) + text.getLocalBounds().width >
			maxTextWidth()
		) {
			return true;
		}

		sf::String tmp(text.getString());
		tmp.insert(cursorIndex, e.text.unicode);
		text.setString(tmp);
		++cursorIndex;

		return true;
	}

	if (e.type == sf::Event::KeyPressed) {
		switch (e.key.code) {
			case sf::Keyboard::BackSpace: {
				if (cursorIndex == 0) {
					return true;
				}

				--cursorIndex;

				sf::String tmp(text.getString());
				tmp.erase(cursorIndex);
				text.setString(tmp);

				return true;
			}
			case sf::Keyboard::Left:
				if (cursorIndex != 0) {
					--cursorIndex;
				}
				return true;
			case sf::Keyboard::Right:
				if (cursorIndex != text.getString().getSize()) {
					++cursorIndex;
				}
				return true;
			default:
				return false;
		}
	}

	if (e.type == sf::Event::MouseButtonPressed) {
		if (e.mouseButton.button != sf::Mouse::Left) {
			return false;
		}

		sf::Vector2f mousePosition = window.mapPixelToCoords(
			sf::Vector2i(e.mouseButton.x, e.mouseButton.y)
		);

		if (!position.contains(mousePosition)) {
			return false;
		}

		cursorIndex = getIndex(mousePosition.x - position.left);

		return true;
	}

	return false;
}

unsigned int GUI::Widget::TextField::getIndex(const float& xPosition) const {
	if (xPosition < textStartPos().x) {
		return 0;
	} else if (xPosition > text.getLocalBounds().width + textStartPos().x) {
		return text.getString().getSize();
	}

        int a = 0;
        int b = text.getString().getSize() - 1;
	while (a <= b) {
		unsigned int current = (a + b) / 2;

		float charWidth = textWidth({text.getString()[current]});
		float charPos = text.findCharacterPos(current).x;

		if (std::abs(charWidth / 2 + charPos - xPosition) < charWidth) {
			if (charWidth / 2 + charPos - xPosition < 0) {
				return current + 2;
			}
			return current + 1;
		}

		if (charPos > xPosition) {
			b = current - 1;
		}
		if (charPos < xPosition) {
			a = current + 1;
		}
	}

	return a + 1;
}

unsigned int GUI::Widget::TextField::fontSize() const {
	return position.height * 0.8f - borderSize() * 2;
}

unsigned int GUI::Widget::TextField::borderSize() const {
	return (position.height < 60 ? (position.height < 40 ? 2 : 4) : 6);
}

sf::Vector2f GUI::Widget::TextField::textStartPos() const {
	return sf::Vector2f(
		borderSize() + characterSize().x / 2,
		position.height / 2 - characterSize().y / 2 - borderSize() / 2
	);
}

float GUI::Widget::TextField::maxTextWidth() const {
	return (
		position.width -
		borderSize() * 2 -
		textStartPos().x -
		characterSize().x / 2
	);
}

float GUI::Widget::TextField::textWidth(const sf::String& str) const {
	return sf::Text(str, font, fontSize()).getLocalBounds().width;
}

sf::Vector2f GUI::Widget::TextField::characterSize() const {
	sf::Text tmp("j", font, fontSize());

	sf::Vector2f size;
	size.x = tmp.getLocalBounds().width;

	tmp.setString(stringFromUtf8(u8"|_ÖÄ%£*"));
	size.y = tmp.getLocalBounds().height;

	return size;
}

void GUI::Widget::TextField::draw(sf::RenderWindow& window) {
	sf::RectangleShape background(
		sf::Vector2f(
			position.width - borderSize() * 2,
			position.height - borderSize() * 2
		)
	);
	background.setPosition(
		position.left + borderSize(),
		position.top + borderSize()
	);
	background.setFillColor(Color::background);
	background.setOutlineColor(Color::border);
	background.setOutlineThickness(borderSize());
	window.draw(background);

	text.setPosition(textStartPos() + sf::Vector2f(position.left, position.top));
	window.draw(text);

	sf::Vector2f cursorSize;
	if (cursorIndex == text.getString().getSize()) {
		cursorSize.x = characterSize().x;
	} else {
		cursorSize.x = textWidth({text.getString()[cursorIndex]});
	}
	cursorSize.y = position.height / 30;

	sf::RectangleShape cursor(cursorSize);
	cursor.setPosition(text.findCharacterPos(cursorIndex));
	cursor.move(0, fontSize() * 1.25f);
	cursor.setFillColor(Color::backgroundHover);
	window.draw(cursor);
}
