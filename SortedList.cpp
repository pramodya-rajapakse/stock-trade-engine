#include <iostream>
#include <string>
#include "Order.cpp"

// SortList object - linked list of Order object, sorted in ascending or descending order
class SortedList {
	public:
	bool isBuyingList; // true = buying list (desc), false = selling list (asc)
	Order* head; // ptr to head Order of list

	/*
	default constructor for SortedList object
	@return - None
	*/
	SortedList() {
		isBuyingList = true;
		head = nullptr;
	}

	/*
	constructor for SortedList object
	@param buying - type of list (buying or selling)
	@return - None
	*/
	SortedList(bool buying) {
		isBuyingList = buying;
		head = nullptr;
	}

	/*
	addOrder - adds new Order object into list, inserted in sorted order based on type
	@param tp - type of order ("Buy" or "Sell")
	@param tkr - ticker type (ex. "APPL")
	@param qty - quantity of the order
	@param prc - price of the order
	@return - None
	*/
	void addOrder(string tp, string tkr, int qty, int prc) {
		Order* order = new Order(tp, tkr, qty, prc); // create new order
		cout << "\nAdding";
		order->printOrder();
		Order* currNode = this->head;
		Order* prevNode = nullptr;

		// if this is a selling list -> insert in ascending order
		if (!this->isBuyingList) {
			while (currNode != nullptr && currNode->price < prc) {
				prevNode = currNode;
				currNode = currNode->next;
			}
		}
		// if this is a buying list -> insert in descending order
		else {
			while (currNode != nullptr && currNode->price > prc) {
				prevNode = currNode;
				currNode = currNode->next;
			}
		}

		// insert new order in this spot, update head if its at the front
		if (prevNode == nullptr) {
			order->next = currNode;
			this->head = order;
		}
		else {
			prevNode->next = order;
			order->next = currNode;
		}

	}

	/*
	removeOrder - remove Order object from this SortedList
	@param prevOrder - pointer to Order directly behind Order to remove
	@param order - pointer to the Order object in this list that should be removed
	@return - None
	*/
	void removeOrder(Order* prevOrder, Order* order) {
		if (prevOrder != nullptr) {
			prevOrder->next = order->next;
		}
		else {
			this->head = order->next; // order was the head -> set new head
			order->next = nullptr;
		}

		delete order; // call Order destructr
	}

	/*
	printList - prints SortedList object, used for debugging and verifying
	@return - None
	*/
	void printList() {
		// only prints list if there are any orders in it
		if (this->head != nullptr) {
			cout << "PRINTING SORTED LIST..." << endl;
			Order* currNode = this->head;
			// traverse each Order in list
			while (currNode != nullptr) {
				currNode->printOrder();
				currNode = currNode->next;
			}
			cout << "\n" << endl;
		}
	}

};