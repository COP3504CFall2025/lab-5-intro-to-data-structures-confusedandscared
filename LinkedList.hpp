#pragma once
#include <iostream>
using namespace std;

template <typename T>
struct Node {

	Node(T data_, Node* prev_, Node* next_) : data(data_), prev(prev_), next(next_) {};

	T data;
	Node* prev;
	Node* next;
};

template <typename T>
class LinkedList {
public:
	// Behaviors
	void printForward() const {
		Node<T>* temp = head;

		while (temp != nullptr) {
			cout << temp->data << endl;
			temp = temp->next;
		}
	};
	void printReverse() const {
		Node<T>* temp = tail;

		while (temp != nullptr) {
			cout << temp->data << endl;
			temp = temp->prev;
		}
	};

	// Accessors
	[[nodiscard]] unsigned int getCount() const {
		return count;
	};
	Node<T>* getHead() {
		return head;
	};
	const Node<T>* getHead() const {
		return head;
	};
	Node<T>* getTail() {
		return tail;
	};
	const Node<T>* getTail() const {
		return tail;
	};

	// Insertion
	void addHead(const T& data) {
		Node<T>* n = new Node<T>(data, nullptr, nullptr);

		if (head == nullptr) {
			head = n;
			tail = n;
		} 
		else {
			n->next = head; // head in front of n
			head->prev = n; // put n before head
			head = n;
		}

		count++;
	};
	void addTail(const T& data) {
		Node<T>* n = new Node<T>(data, nullptr, nullptr);

		if (head == nullptr) {
			head = n;
			tail = n;
		} 
		else {
			n->prev = tail;
			tail->next = n;
			tail = n;
		}

		count++;
	};

	// Removal
	bool removeHead() {
		if (head == nullptr) { // if no list
			return false;
		}

		Node<T>* temp = head;
		
		if (head == tail) { // there's either 1 on list or > 1
			head = nullptr;
			tail = nullptr;
		}
		else {
			head = head->next;
			head->prev = nullptr;
		} 

		delete temp;
		count--;
		return true;
	};
	bool removeTail() {
		if (head == nullptr) { // if no list
			return false;
		}

		Node<T>* temp = tail;
		
		if (head == tail) { // there's either 1 on list or > 1
			head = nullptr;
			tail = nullptr;
		}
		else {
			tail = tail->prev;
			tail->next = nullptr;
		} 

		delete temp;
		count--;
		return true;
	};
	void clear() {
		while (head != nullptr) {
			removeTail();
		}
	};

	// Operators
	LinkedList<T>& operator=(LinkedList<T>&& other) noexcept {
		if (this == &other) return *this;

		this->clear();

		this->head = other.head;
		this->tail = other.tail;
		this->count = other.count;

		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;

		return *this;
	};
	LinkedList<T>& operator=(const LinkedList<T>& rhs) {
		if (this == &rhs) return *this;

		Node<T>* temp = rhs.head;

		this->clear();

		while (temp != nullptr) {
			this->addTail(temp->data);
			temp = temp->next;
		}

		return *this;
	};

	// Construction/Destruction
	LinkedList() {
		head = nullptr;
		tail = nullptr;
		count = 0;
	};
	LinkedList(const LinkedList<T>& list) {
		this->head = nullptr;
		this->tail = nullptr;
		this->count = 0;
	
		Node<T>* temp = list.head;

		while (temp != nullptr) {
			this->addTail(temp->data);
			temp = temp->next;
		}
	};
	LinkedList(LinkedList<T>&& other) noexcept : head(other.head), tail(other.tail), count(other.count) {
		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;
	};
	~LinkedList() {
		clear();
	};

private:
	// Stores pointers to first and last Node<T>s and count
	Node<T>* head;
	Node<T>* tail;
	unsigned int count;

};




