#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Inventory.h"
using namespace std;
using namespace sf;
class Player {
public:
	Inventory inventory;
	Texture texture;
	Sprite sprite;
	Texture shieldTexture;
	Sprite shield;
	bool isShield;
	bool hasKey;
	int score;

	Player(string path) {
		score = 0;
		hasKey = false;
		texture.loadFromFile(path);
		sprite.setTexture(texture);
		isShield = false;
		shieldTexture.loadFromFile("img/shield.png");
		shield.setTexture(shieldTexture);
	}

	void setSize(float width, float height) {
		sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height);
		shield.setScale(((width + 5) / shield.getLocalBounds().width), (height + 5) / shield.getLocalBounds().height);
	}

	void setPosition(float x, float y) {
		sprite.setPosition(x, y);
		if (isShield) {
			shield.setPosition(x + 8, y);
		}
	}


};