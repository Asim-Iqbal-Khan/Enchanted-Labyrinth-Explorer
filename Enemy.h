#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;
class Enemy {
public:
	Texture texture;
	Sprite sprite;

	Enemy(string path = "img/goblin1.png") {
		texture.loadFromFile(path);
		sprite.setTexture(texture);
	}

	Enemy(Enemy& copy) {
		texture = copy.texture;
		sprite = copy.sprite;
	}

	void operator=(Enemy& e) {
		texture = e.texture;
		sprite = e.sprite;
	}

	void setSize(float width, float height) {
		sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height);
	}

	void setPosition(float x, float y) {
		sprite.setPosition(x, y);
	}
};

class EnemyNode {
public:
	EnemyNode* next;
	Enemy* e;
	int count;
	EnemyNode(Enemy* e) {
		next = nullptr;
		this->e = e;
		count = 0;
	}
};

class EnemyQueue {
public:
	EnemyNode* front;
	EnemyNode* rear;
	int length;
	EnemyQueue() {
		front = nullptr;
		rear = nullptr;
		length = 0;
	}

	void enqueue(Enemy* e) {
		length++;
		if (front == nullptr && rear == nullptr) {
			front = new EnemyNode(e);
			rear = front;
			return;
		}
		int count = rear->count;
		rear->next = new EnemyNode(e);
		rear = rear->next;
		rear->count = count + 1;
		return;
	}

	void dequeue() {
		if (front != nullptr && rear != nullptr) {
			length--;
			EnemyNode* temp = front;
			front = front->next;
			if (front == nullptr) {
				rear = nullptr;
			}
			delete temp;
		}
	}

	Enemy* peek() {
		return front->e;
	}

	void deleteNode(EnemyNode*& toDelete) {
		if (toDelete == nullptr)
			return;

		if (toDelete == front) {
			front = toDelete->next;
			if (front == nullptr) {
				rear = nullptr;
			}
		}
		else {
			EnemyNode* curr = front;
			EnemyNode* prev = nullptr;
			while (curr != nullptr) {
				if (curr == toDelete) {
					if (toDelete == rear) {
						rear = prev;
					}
					else {
						prev->next = curr->next;
						curr = curr->next;
					}
					continue;
				}
				prev = curr;
				curr = curr->next;
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