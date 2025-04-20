#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;
class Treasure {
public:
	int score;
	Sprite sprite;
	Texture texture;
	bool collision;
	Treasure(string path, int s) {
		collision = false;
		score = s;
		texture.loadFromFile(path);
		sprite.setTexture(texture);
	}

	void setSize(float width, float height) {
		sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height);
	}

	void setPosition(float x, float y) {
		sprite.setPosition(x, y);
	}
};