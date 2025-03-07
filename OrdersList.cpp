#include <iostream>
#include <string>
#include <array>
#include <mutex>
#include "SortedList.cpp"

// OrdersList object - each unique ticker has an OrdersList object containing buying and selling order lists
class OrdersList {
	public:
	SortedList buyingList; // list of buying orders for this ticker
	SortedList sellingList; // list of selling orders for this ticker

	// removes default copy constructor -> needed for mutex 
	OrdersList (const OrdersList&) = delete;
	OrdersList& operator= (const OrdersList&) = delete;
	mutex mtx; // mutex for synchronization

	/*
	default constructor for OrdersList object
	@return - None
	*/
	OrdersList() {
		buyingList = SortedList(true);
		sellingList = SortedList(false);
	}

	/*
	addOrder - calls addOrder of SortedList of given type to add new Order
	@param tp - type of order ("Buy" or "Sell")
	@param tkr - ticker type (ex. "APPL")
	@param qty - quantity of the order
	@param prc - price of the order
	@return - None
	*/
	void addOrder(string tp, string tkr, int qty, int prc) {
		mtx.lock(); // lock so no other thread can add/match for this ticker type
		// add new order to relevant sortedlist based on ticker and buying type
		if (tp == "Buy") {
			this->buyingList.addOrder(tp, tkr, qty, prc);
		}
		else {
			this->sellingList.addOrder(tp, tkr, qty, prc);
		}
		mtx.unlock();
	}

	/*
	matchOrder - performs matching for buy-sell orders of this ticker type
	@return - None
	*/
	void matchOrder() {
		mtx.lock(); // lock so no other thread can add/match during process
		Order* prevSellPtr = nullptr;
		Order* currSellPtr = sellingList.head;
		Order* prevBuyPtr = nullptr;
		Order* currBuyPtr = buyingList.head;

		// match smallest sell order with largest buy (if buy price is greater), otherwise no valid match possible
		while (currSellPtr != nullptr && currBuyPtr != nullptr && currBuyPtr->price >= currSellPtr->price) {
			// valid match -> update quantity for order with larger quantity, remove smaller (or both if equal)
			if (currBuyPtr->quantity == currSellPtr->quantity) {
				// match removes both completely
				Order* nextBuyPtr = currBuyPtr->next;
				Order* nextSellPtr = currSellPtr->next;
				this->buyingList.removeOrder(prevBuyPtr, currBuyPtr);
				this->sellingList.removeOrder(prevSellPtr, currSellPtr);
				// increment pointers for both lists
				currBuyPtr = nextBuyPtr;
				currSellPtr = nextSellPtr;
			}
			// if buy order has larger quantity than sell order
			else if (currBuyPtr->quantity > currSellPtr->quantity) {
				Order* nextSellPtr = currSellPtr->next;
				// update quantity for buy order
				currBuyPtr->quantity -= currSellPtr->quantity;
				// remove sell order
				this->sellingList.removeOrder(prevSellPtr, currSellPtr);
				// increment pointer for selling list
				currSellPtr = nextSellPtr;
			}
			// if sell order has larger quantity than buy order
			else {
				Order* nextBuyPtr = currBuyPtr->next;
				// update quantity for sell order
				currSellPtr->quantity -= currBuyPtr->quantity;
				// remove buy order
				this->buyingList.removeOrder(prevBuyPtr, currBuyPtr);
				// increment pointer for buying list
				currBuyPtr = nextBuyPtr;
			}
		}
		mtx.unlock();
	}

	/*
	printOrdersList - call print metho for buying list and selling list for this ticker, used for debuging
	@return - None
	*/
	void printOrdersList() {
		this->buyingList.printList();
		this->sellingList.printList();
	}
};