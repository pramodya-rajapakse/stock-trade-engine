#include <iostream>
#include <string>
#include <array>
#include "OrdersList.cpp"

using namespace std;

const int SIZE = 1024;

/*
common hash function - maps ticker string -> unique index between 0 - 1023
@param word - string ticker word
@return - unique index for this ticker string
*/
int hash_function(string word) {
	int seed = 131; // prime number
	unsigned long hash = 0;
	for(int i = 0; i < word.length(); i++) {
		hash = (hash * seed) + word[i];
	}
	return hash % SIZE; // mod to get valid index
}

// StockBook object - contains array of 1024 OrdersList objects for each ticker type
class StockBook {
	public:
	OrdersList ordersList[SIZE]; // array of OrdersList for each ticker

	/*
	addOrder - gets index into array based on ticker, calls addOrder for that OrdersList object
	@param tp - type of order ("Buy" or "Sell")
	@param tkr - ticker type (ex. "APPL")
	@param qty - quantity of the order
	@param prc - price of the order
	@return - None
	*/
	void addOrder(string tp, string tkr, int qty, int prc) {
		int index = hash_function(tkr);
		
		// add new order to relevant sortedlist based on ticker and buying type
		this->ordersList[index].addOrder(tp, tkr, qty, prc);
	}

	/*
	matchOrder - calls matchOrder() for each OrdersList object
	@return - None
	*/
	void matchOrder() {
		for (int i = 0; i < SIZE; i++) {
			ordersList[i].matchOrder();
		}
	}

	/*
	printStockBook - call print method for each OrdersList object in stockbook
	@return - None
	*/
	void printStockBook() {
		cout << "\nPRINTING FULL STOCKBOOK...\n" << endl;
		for (int i = 0; i < SIZE; i++) {
			ordersList[i].printOrdersList();
		}
	}


};