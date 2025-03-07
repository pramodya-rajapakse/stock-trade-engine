#include <string>
#include <iostream>

using namespace std;

// Order object for each type of buy and sell order
class Order {
	public:
	string type;
	string ticker;
	int quantity;
	int price;
	Order* next; // pointer to next Order object in sorted list of orders

	/*
	default constructor for Order object
	@param tp - type of order ("Buy" or "Sell")
	@param tkr - ticker type (ex. "APPL")
	@param qty - quantity of the order
	@param prc - price of the order
	@return - None
	*/
	Order(string tp, string tkr, int qty, int prc) {
		type = tp;
		ticker = tkr;
		quantity = qty;
		price = prc;
		next = nullptr;
	}

	/*
	destructor for Order object - frees memory of this Order object, prints message for debugging
	@return - None
	*/
	~Order() {
		cout << "\nRemoved order (" << type << ", " << ticker << ", " << quantity << ", " << price << ")" << endl; 
	}

	/*
	print method - prints order detials, used for debugging
	@return - None
	*/
	void printOrder() {
		cout << "Order: (" << type << ", " << ticker << ", " << quantity << ", " << price << "), "; 

	}
};