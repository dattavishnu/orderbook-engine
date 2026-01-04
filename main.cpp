#include "orderbook.h"
#include <iostream>


int main() {
    order_book ob;
    std::vector<trades> vec;

	// AI GENERATED TEST CASES
    // -------- INITIAL GTC ORDERS --------

    // BUY side (GTC)
    auto o1 = std::make_shared<order>(1, 100, side::buy, 5, order_type::good_till_cancel);
    auto o2 = std::make_shared<order>(2, 101, side::buy, 10, order_type::good_till_cancel);
    auto o3 = std::make_shared<order>(3, 102, side::buy, 15, order_type::good_till_cancel);
    auto o4 = std::make_shared<order>(4, 101, side::buy, 7, order_type::good_till_cancel);
    auto o5 = std::make_shared<order>(5, 99, side::buy, 20, order_type::good_till_cancel);

    // SELL side (GTC)
    auto o6 = std::make_shared<order>(6, 103, side::sell, 5, order_type::good_till_cancel);
    auto o7 = std::make_shared<order>(7, 104, side::sell, 10, order_type::good_till_cancel);
    auto o8 = std::make_shared<order>(8, 105, side::sell, 15, order_type::good_till_cancel);
    auto o9 = std::make_shared<order>(9, 102, side::sell, 8, order_type::good_till_cancel);
    auto o10 = std::make_shared<order>(10, 101, side::sell, 12, order_type::good_till_cancel);

    // -------- FAK / IOC ORDERS --------

    // Fully fills
    auto o11 = std::make_shared<order>(11, 103, side::buy, 5, order_type::fill_and_kill);
    auto o12 = std::make_shared<order>(12, 102, side::sell, 8, order_type::fill_and_kill);

    // Partial fill → cancel remainder
    auto o13 = std::make_shared<order>(13, 101, side::sell, 20, order_type::fill_and_kill);
    auto o14 = std::make_shared<order>(14, 102, side::buy, 30, order_type::fill_and_kill);

    // No match → never enters book
    auto o15 = std::make_shared<order>(15, 90, side::sell, 10, order_type::fill_and_kill);
    auto o16 = std::make_shared<order>(16, 110, side::buy, 10, order_type::fill_and_kill);

    // Edge cases
    auto o17 = std::make_shared<order>(17, 101, side::buy, 7, order_type::fill_and_kill);
    auto o18 = std::make_shared<order>(18, 105, side::buy, 15, order_type::fill_and_kill);
    auto o19 = std::make_shared<order>(19, 99, side::sell, 20, order_type::fill_and_kill);
    auto o20 = std::make_shared<order>(20, 104, side::buy, 10, order_type::fill_and_kill);

    // -------- INSERT ALL --------

    vec.emplace_back(ob.add_order(o1));
    vec.emplace_back(ob.add_order(o2));
    vec.emplace_back(ob.add_order(o3));
    vec.emplace_back(ob.add_order(o4));
    vec.emplace_back(ob.add_order(o5));

    vec.emplace_back(ob.add_order(o6));
    vec.emplace_back(ob.add_order(o7));
    vec.emplace_back(ob.add_order(o8));
    vec.emplace_back(ob.add_order(o9));


    auto info = ob.get_l2_snapshot(10);
    vec.emplace_back(ob.add_order(o10));

    vec.emplace_back(ob.add_order(o11));
    vec.emplace_back(ob.add_order(o12));
    vec.emplace_back(ob.add_order(o13));
    vec.emplace_back(ob.add_order(o14));
    vec.emplace_back(ob.add_order(o15));

    vec.emplace_back(ob.add_order(o16));
    vec.emplace_back(ob.add_order(o17));
    vec.emplace_back(ob.add_order(o18));
    vec.emplace_back(ob.add_order(o19));
    vec.emplace_back(ob.add_order(o20));


     for (auto& tr : vec) {
         for (auto& t : tr) {
             std::cout << "Trade: Buyer id " << t.get_bid_trade().orderid_
                 << " & Seller id " << t.get_ask_info().orderid_
                 << " Qty = " << t.get_ask_info().quantity_
                 << " Price = " << t.get_ask_info().price_ << "\n";
         }
     }
     {
		 std::cout << "Order Book Snapshot:\n";
		 std::cout << "Asks || Bids\n";
         for (auto it1 = info.asks_.begin(), it2 = info.bids_.begin();
             it1 != info.asks_.end() || it2 != info.bids_.end(); ) {
             if (it1 != info.asks_.end()) {
                 std::cout << it1->amount_ << " : " << it1->quantity_;
                 ++it1;
             }
             if (it2 != info.bids_.end()) {
                 std::cout << " || " << it2->amount_ << " : " << it2->quantity_;
                 ++it2;
             }
             std::cout << std::endl;
         }
     }
     
    return 0;
}

