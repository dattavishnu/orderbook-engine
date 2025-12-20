
// CMakeProject2.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include <map>
#include <list>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <format>
#include <mutex>

enum class order_type { good_till_cancel, fill_and_kill };
enum class side { buy, sell };

using price = std::int32_t;
using quantity = std::int32_t;
using order_id = std::uint64_t;

struct level_info {
    price amount_;
    quantity quantity_;
};
using order_level_infos = std::vector<level_info>;

class order_level_info {
public:
    order_level_info(order_level_infos bids, order_level_infos asks)
        : bids_(bids), asks_(asks) {
    }
private:
    order_level_infos bids_;
    order_level_infos asks_;
};

class order {
public:
    order(order_id order_id, price price, side side,
        quantity quantity, order_type type)
        : order_id_(order_id), price_(price), side_(side),
        initial_quantity_(quantity), remaining_quantity_(quantity), order_type_(type) {
    }

    quantity get_remaining_quantity() const { return remaining_quantity_; }
    side get_side() const { return side_; }
    price get_price() const { return price_; }
    order_type get_order_type() const { return order_type_; }
    order_id get_order_id() const { return order_id_; }
    quantity get_initial_quantity() const { return initial_quantity_; }

    bool is_filled() const { return remaining_quantity_ == 0; }

    void fill(quantity q) {
        if (q > remaining_quantity_)
            throw std::logic_error(std::format("order {} overfilled", order_id_));
        remaining_quantity_ -= q;
    }

private:
    order_id order_id_;
    price price_;
    side side_;
    order_type order_type_;
    quantity initial_quantity_;
    quantity remaining_quantity_;
};

using order_ptr = std::shared_ptr<order>;
using order_ptrs = std::list<order_ptr>;

struct trade_info {
    order_id orderid_;
    price price_;
    quantity quantity_;
};

class trade {
public:
    trade(trade_info bid_trade, trade_info ask_trade)
        : bid_trade_(bid_trade), ask_trade_(ask_trade) {
    }

    const trade_info& get_bid_trade() const { return bid_trade_; }
    const trade_info& get_ask_info() const { return ask_trade_; }

private:
    trade_info bid_trade_;
    trade_info ask_trade_;
};

using trades = std::vector<trade>;

class order_book {
private:
    struct order_entry {
        order_ptr order_;
        order_ptrs::iterator location_;
    };

    std::map<price, order_ptrs, std::greater<price>> bids_;
    std::map<price, order_ptrs, std::less<price>> asks_;
    std::unordered_map<order_id, order_entry> orders_;

    std::mutex ordersMutex_;

    bool can_match(side s, price p) const {
        if (s == side::buy) {
            if (asks_.empty()) return false;
            return asks_.begin()->first <= p;
        }
        else {
            if (bids_.empty()) return false;
            return bids_.begin()->first >= p;
        }
    }

    trades match_orders() {
        trades result;

        while (!bids_.empty() && !asks_.empty()) {
            auto [bid_price, bid_list] = *bids_.begin();
            auto [ask_price, ask_list] = *asks_.begin();

            if (bid_price < ask_price)
                break;

            while (!bid_list.empty() && !ask_list.empty()) {
                auto bid = bid_list.front();
                auto ask = ask_list.front();

                quantity qty = std::min(bid->get_remaining_quantity(),
                    ask->get_remaining_quantity());

                bid->fill(qty);
                ask->fill(qty);

                result.emplace_back(
                    trade_info{ bid->get_order_id(), bid->get_price(), qty },
                    trade_info{ ask->get_order_id(), ask->get_price(), qty }
                );

                if (bid->is_filled()) {
                    orders_.erase(bid->get_order_id());
                    bid_list.pop_front();
                }
                if (ask->is_filled()) {
                    orders_.erase(ask->get_order_id());
                    ask_list.pop_front();
                }
            }

            if (bid_list.empty()) bids_.erase(bid_price);
            if (ask_list.empty()) asks_.erase(ask_price);
        }

        return result;
    }

public:

    trades add_order(order_ptr order) {
        std::scoped_lock lock(ordersMutex_);

        if (orders_.contains(order->get_order_id()))
            throw std::logic_error("duplicate order id");

        price p = order->get_price();
        side  s = order->get_side();

        order_ptrs* list_ptr;
        if (s == side::buy)
            list_ptr = &bids_[p];
        else
            list_ptr = &asks_[p];

        list_ptr->push_back(order);
        auto it = std::prev(list_ptr->end());

        orders_[order->get_order_id()] = { order, it };

        if (can_match(s, p))
            return match_orders();

        return {};
    }

    void cancel_order(order_id id) {
        std::scoped_lock lock(ordersMutex_);
        if (!orders_.contains(id)) return;

        auto [ord, it] = orders_[id];
        price p = ord->get_price();

        if (ord->get_side() == side::buy) {
            auto& lst = bids_[p];
            lst.erase(it);
            if (lst.empty()) bids_.erase(p);
        }
        else {
            auto& lst = asks_[p];
            lst.erase(it);
            if (lst.empty()) asks_.erase(p);
        }

        orders_.erase(id);
    }
};
// TODO: Reference additional headers your program requires here.
