#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

class Bullet {
public:
	Texture texture;
	Sprite sprite;
	string direction;

	Bullet() {
		texture.loadFromFile("img/fire.png");
		sprite.setTexture(texture);
		direction = "";
	}

	void setSize(float width, float height) {
		sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height);
	}

	void setPosition(float x, float y) {
		sprite.setPosition(x, y);
	}
};

class BulletNode {
public:
	BulletNode* next;
	Bullet fire;
	int count;
	BulletNode(Bullet fire) {
		next = nullptr;
		this->fire = fire;
		count = 0;
	}
};

class BulletQueue {
public:
	BulletNode* front;
	BulletNode* rear;
	int length;
	BulletQueue() {
		front = nullptr;
		rear = nullptr;
		length = 0;
	}

	void enqueue(Bullet fire) {
		length++;
		if (front == nullptr && rear == nullptr) {
			front = new BulletNode(fire);
			rear = front;
			return;
		}
		int count = rear->count;
		rear->next = new BulletNode(fire);
		rear = rear->next;
		rear->count = count + 1;
		return;
	}

	void dequeue() {
		if (front != nullptr && rear != nullptr) {
			length--;
			BulletNode* temp = front;
			front = front->next;
			if (front == nullptr) {
				rear = nullptr;
			}
			delete temp;
		}
	}

	Bullet peek() {
		return front->fire;
	}

	void deleteNode(BulletNode*& toDelete) {
		if (toDelete == nullptr) return;

		if (toDelete == front) {
			front = toDelete->next;
			if (front == nullptr) {
				rear = nullptr;
			}
		}
		else {
			BulletNode* temp = front;
			while (temp != nullptr && temp->next != toDelete) {
				temp = temp->next;
			}
			if (temp == nullptr)
				return;
			temp->next = toDelete->next;
			if (rear == toDelete) {
				rear = temp;
			}
		}
		delete toDelete;
		toDelete = nullptr;
		length--;
	}


	bool is_empty() {
		if (front == nullptr && rear == nullptr) {
			return true;
		}
		return false;
	}

	int size() {
		return length;
	}

	void clear() {
		while (front) {
			dequeue();
		}
	}
};