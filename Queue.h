#pragma once
template<class T>
class Node_Q {
public:
	Node_Q<T>* next;
	T row;
	T col;
	int count;
	Node_Q(T row, T col) {
		this->row = row;
		this->col = col;
		next = nullptr;
		count = 0;
	}
};

template<class T>
class Queue {
public:
	Node_Q<T>* front;
	Node_Q<T>* rear;
	int length;

	Queue() {
		front = nullptr;
		rear = nullptr;
		length = 0;
	}

	void enqueue(T row, T col) {
		length++;
		if (front == nullptr && rear == nullptr) {
			front = new Node_Q<T>(row, col);
			rear = front;
			return;
		}
		int count = rear->count;
		rear->next = new Node_Q<T>(row, col);
		rear = rear->next;
		rear->count = count + 1;
		return;
	}

	void dequeue() {
		if (front != nullptr && rear != nullptr) {
			length--;
			Node_Q<T>* temp = front;
			front = front->next;
			if (front == nullptr) {
				rear = nullptr;
			}
			delete temp;
		}
	}

	T peek() {
		return front->data;
	}

	T* getData(int ind) {
		Node_Q<T>* curr = front;
		while (curr->next != nullptr) {
			if (curr->count == ind) {
				break;
			}
			curr = curr->next;
		}
		T* data = new T[2];
		data[0] = curr->row;
		data[1] = curr->col;
		return data;
	}

	void reverse(Node_Q<T>* front, Node_Q<T>* rear) {
		Node_Q<T>* temp1 = front;
		Node_Q<T>* temp2 = nullptr;
		while (temp1 != temp2) {
			while (temp1->next != temp2) {
				temp1 = temp1->next;
			}
			enqueue(temp1->row, temp1->col);
			temp2 = temp1;
			temp1 = front;
		}
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