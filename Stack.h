#pragma once
template<class T>
class Node {
public:
	Node* next;
	T row;
	T col;
	Node(T row, T col) {
		this->row = row;
		this->col = col;
		next = nullptr;
	}
};

template<class T>
class Stack {
public:
	Node<T>* top;
	Stack() {
		top = nullptr;
	}

	void push(T row, T col) {
		Node<T>* temp = new Node<T>(row, col);
		temp->next = top;
		top = temp;
	}

	void pop() {
		if (!isEmpty()) {
			Node<T>* temp = top;
			top = top->next;
			delete temp;
			temp = nullptr;
		}
	}

	T* peek() {
		T* data = new T[2];
		data[0] = top->row;
		data[1] = top->col;
		return data;
	}

	void clear() {
		while (!isEmpty()) {
			pop();
		}
	}

	bool isEmpty() {
		if (top == nullptr) {
			return true;
		}
		return false;
	}

	~Stack() {
		clear();
	}
};
