#include <iostream>
#include <thread> // used to simulate multithreading
#include <string>
#include <cstring>
#include <vector> // used to store multiple threads
#include <chrono> // chrono and random used to artifically add delay time
#include <random>
#include "StockBook.cpp"

using namespace std;

// thread run function -> call matchOrder()
void matchOrders(StockBook *stockbook) {
	stockbook->matchOrder();
}

// thread run function -> single call to addOrder()
void add(StockBook *stockbook, string type, string ticker, int quantity, int price) {
	// add random delay between 100 - 1000 ms, easier to track changes
	// found through cpp documentation
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(100, 1000);
	this_thread::sleep_for(chrono::milliseconds(distrib(gen)));
	stockbook->addOrder(type, ticker, quantity, price);
}

// thread run function -> many calls to addOrder(), mix between 2 tickers, different parameters
void addOrders(StockBook *stockbook, int numOrders) {
	// add random delay between 100 - 1000 ms, easier to track changes
	// found through cpp documentation
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(100, 1000);
	this_thread::sleep_for(chrono::milliseconds(distrib(gen)));
	for (int i = 0; i < numOrders; ++i) {
			if (i % 2 == 0) {
				stockbook->addOrder("Buy", "APPL", 100 + i, 100 + i);
			} 
			else if (i % 3 == 0) {
				stockbook->addOrder("Buy", "GOOGL", 55 + i, 55 + i);
			}
			else if (i % 5 == 0) {
				stockbook->addOrder("Sell", "GOOGL", 23 + i, 23 + i);
			}
			else {
				stockbook->addOrder("Sell", "APPL", 50 + i, 50 + i);
			}
	}
}

int main(int argc, char *argv[]) {

	StockBook stockbook;

	if (argc < 2 or argc >= 3) {
		cout << "Please run the program again with a correct run option. Details can be found in readme.md" << endl;
		return 0;
	}

	if (strcmp(argv[1], "add") == 0) {
		cout << "\nAdd option - adding 4 orders to show correct sorting\n" << endl;
		
		stockbook.addOrder("Buy", "APPL", 100, 100);
		stockbook.addOrder("Buy", "APPL", 100, 300);
		stockbook.addOrder("Sell", "APPL", 100, 70);
		stockbook.addOrder("Sell", "APPL", 100, 50);

		stockbook.printStockBook();
	}

	else if (strcmp(argv[1], "match") == 0) {
		cout << "\nMatch option - adding 4 orders, and then calling matchOrder()\n" << endl;

		stockbook.addOrder("Buy", "APPL", 100, 100);
		stockbook.addOrder("Buy", "APPL", 100, 300);
		stockbook.addOrder("Buy", "APPL", 100, 80);
		stockbook.addOrder("Sell", "APPL", 50, 70);
		stockbook.addOrder("Sell", "APPL", 100, 50);
		stockbook.addOrder("Sell", "APPL", 100, 90);

		cout << "\nStockbook BEFORE matching\n" << endl;
		stockbook.printStockBook();

		stockbook.matchOrder();
		cout << "\nStockbook AFTER matching\n" << endl;
		stockbook.printStockBook();
	}

	else if (strcmp(argv[1], "multithread-short") == 0) {
		cout << "\nMultithread short - 2 threads adding orders for same tickers\n" << endl;
		thread t1(add, &stockbook, "Buy", "APPL", 100, 50);
		thread t2(add, &stockbook, "Buy", "APPL", 100, 100);

		t1.join();
		t2.join();

		stockbook.printStockBook();
	}

	else if (strcmp(argv[1], "multithread-long") == 0) {
		cout << "\nMultithread long - 30 threads adding ~100 orders across 2 tickers, calling matchOrder() ~10 times interspersed between\n" << endl;
		vector<thread> threads;
		for (int i = 0; i < 30; ++i) {
			if (i % 3 == 0) {
				threads.emplace_back(matchOrders, &stockbook);
			}
			else {
				threads.emplace_back(addOrders, &stockbook, 50);
			}
				
		}

		for (auto& thread : threads) {
				thread.join();
		}

		stockbook.printStockBook();
	}

	else if (strcmp(argv[1], "simulate") == 0) {
		cout << "\nSimulate - randomly add orders with random parameters, and call matchOrder() occasionally\nTYPE 'CTRL + C' in the shell to quit\n" << endl;
		srand(time(0));
		while (true) {
			if (rand() % 10 < 8) {
				// add new addOrder thread
				string type = (rand() % 2 == 0) ? "Buy" : "Sell";
				string ticker = (rand() % 2 == 0) ? "APPL" : "GOOGL";
				int quantity = rand() % 100 + 1;
				int price = rand() % 150 + 1;
				thread t1(add, &stockbook, type, ticker, quantity, price);
				t1.join();
			}
			else {
				// add new matchOrder thread
				thread t1(matchOrders, &stockbook);
				t1.join();
			}
		}
	}

	else {
		cout << "Please run the program again with a correct run option. Details can be found in readme.md" << endl;
	}


	cout << "\nmain finished.\n";


	return 0;

}
