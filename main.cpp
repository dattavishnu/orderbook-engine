#include "orderbook.h"
#include <iostream>

int main() {
    order_book ob;

    auto o1 = std::make_shared<order>(1, 100, side::buy, 10, order_type::good_till_cancel);
    auto o2 = std::make_shared<order>(2, 99, side::sell, 5, order_type::good_till_cancel);

    auto trades1 = ob.add_order(o1);
    auto trades2 = ob.add_order(o2);

    for (auto& t : trades2) {
        std::cout << "Trade: Buy " << t.get_bid_trade().orderid_
            << " & Sell " << t.get_ask_info().orderid_
            << " Qty = " << t.get_ask_info().quantity_
            << " Price = " << t.get_ask_info().price_ << "\n";
    }

    return 0;
}

