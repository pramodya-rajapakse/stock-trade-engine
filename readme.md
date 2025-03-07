# Onymos Inc SWE Interview
### Pramodya Rajapakse - Stock Trading Engine

## Run Options
In this replit console, the code can be compiled with `g++ main.cpp`.
There are 5 run options:
- `./a.out add` : This option will add 2 Buy and 2 Sell order of the same ticker type, showing how they will be inserted in sorted order for their respective lists.
- `./a.out match` : This option will add 3 Buy and 3 Sell orders of the same ticker type, and then call matchOrder() to show how buy-sell will be matched and removed.
- `./a.out multithread-short` : This option creates 2 threads that calls addOrder() for the same ticker, showing how they will always be inserted in the correct order regardless of the order of thread execution.
- `./a.out multithread-long` : This option scales up to ~30 threads, with addOrder() and matchOrder() calls to show synchronization with many threads. 
- `./a.out simulate` : This option simulates random trading. It will run continuously, adding random orders and periodically calling matchOrder(). You can stop the program by entering `CTRL + C` in the shell.


## Solution Overview
Working top-down, the top-level object is a single `StockBook`, which contains an array of 1024 entries (for each possible ticker), each being an `OrdersList` object. The `OrdersList` is a pair of lists of buying orders and selling orders for each unique ticker. Each list a `SortedList` object, which is essentially a linked list of `Order` objects, sorted based on the order type. A buying list is sorted in descending order, and the selling list is sorted in ascending order. Each `Order` object contains the order details, type (Buy or Sell), ticker (ex. APPL), quantity, price, as well as a pointer to the next `Order`.

The `addOrder()` function adds a new `Order` object to the corresponding list based on the ticker and the type. Since inserting into the correct list requires potentially iterating over the entire list, the time complexity for this function is linear in terms of the number of `Orders` in the corresponding ticker's list of that type (Buy or Sell).

The `matchOrder()` function of the `StockBook` class calls a `matchOrder()` function for each `OrdersList` object for a single ticker. The function essentially keeps pointers at the start of the buy and sell lists for the ticker. It will iterate through the list as long as the largest Buy order (by price) is greater than the smallest Sell order (by price), for that ticker. If the match is valid, then then a comparison occurs based on the quantity. If the quantities are equal, both Orders are removed. Otherwise, the `Order` with the larger quantity subtracts the smaller `Order's` quantity, and the `Order` with the samller quantity is removed. Since this algorithm will at most run through each order in every list once, and removal takes constant time, this function runs in O(n) time  with n being the total number of orders.

## Multithreading Implementation
Synchronization is achieved by including a mutex in the OrdersList object. The `addOrder()` and `matchOrder()` functions of the `OrdersList` are wrapped with a `mutex` `lock()` and `unlock()`, allowing only a single thread to read/write the `OrdersList` object at once. I chose this implementation because adding or matching orders will only affect a single ticker type at once, so threads could concurrently add or match orders in different tickers without leading to race conditions.

## Additional Notes
I was slightly unsure of how the `matchOrder()` function should operate in terms of which Buy order to match with the smallest Sell order. At first I considered matching the Buy order with the smallest price larger/equal than the smallest Sell order price, as that could potentially result in more matches. However, contextually, I think it makes more sense for the largest Buy order price to match first, since they would likely get priority for buying at the highest price (that would result in the most profit for the one selling). 

The simulation option only uses two ticker types to make tracking changes easier, but there is support for up to 1024 unique tickers in the implementation.
