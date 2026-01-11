#include "pch.h"
#include "../../CMakeProject2/orderbook.h"
#include <iostream>
// AI GENERATED CODE 
TEST(OrderBookTest, AddBuyOrderNoMatch) {
    order_book ob;

    auto o1 = std::make_shared<order>(
        1, 100, side::buy, 10, order_type::good_till_cancel);

    auto trades = ob.add_order(o1);

    EXPECT_TRUE(trades.empty());

    auto l2 = ob.get_l2_snapshot(5);
    ASSERT_EQ(l2.bids_.size(), 1);
    EXPECT_EQ(l2.bids_[0].amount_, 100);
    EXPECT_EQ(l2.bids_[0].quantity_, 10);
}

TEST(OrderBookTest, SimpleMatchFullFill) {
    order_book ob;

    auto buy = std::make_shared<order>(
        1, 100, side::buy, 10, order_type::good_till_cancel);

    auto sell = std::make_shared<order>(
        2, 100, side::sell, 10, order_type::good_till_cancel);

    ob.add_order(buy);
    auto trades = ob.add_order(sell);

    ASSERT_EQ(trades.size(), 1);

    EXPECT_EQ(trades[0].get_bid_trade().orderid_, 1);
    EXPECT_EQ(trades[0].get_ask_info().orderid_, 2);
    EXPECT_EQ(trades[0].get_bid_trade().quantity_, 10);

    auto l2 = ob.get_l2_snapshot(5);
    EXPECT_TRUE(l2.bids_.empty());
    EXPECT_TRUE(l2.asks_.empty());
}

TEST(OrderBookTest, PartialFillLeavesRestingOrder) {
    order_book ob;

    auto buy = std::make_shared<order>(
        1, 100, side::buy, 20, order_type::good_till_cancel);

    auto sell = std::make_shared<order>(
        2, 100, side::sell, 10, order_type::good_till_cancel);

    ob.add_order(buy);
    auto trades = ob.add_order(sell);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].get_bid_trade().quantity_, 10);

    auto l2 = ob.get_l2_snapshot(5);
    ASSERT_EQ(l2.bids_.size(), 1);
    EXPECT_EQ(l2.bids_[0].quantity_, 10);
}

TEST(OrderBookTest, FIFOAtSamePrice) {
    order_book ob;

    auto buy1 = std::make_shared<order>(
        1, 100, side::buy, 5, order_type::good_till_cancel);

    auto buy2 = std::make_shared<order>(
        2, 100, side::buy, 5, order_type::good_till_cancel);

    auto sell = std::make_shared<order>(
        3, 100, side::sell, 7, order_type::good_till_cancel);

    ob.add_order(buy1);
    ob.add_order(buy2);

    auto trades = ob.add_order(sell);

    ASSERT_EQ(trades.size(), 2);

    EXPECT_EQ(trades[0].get_bid_trade().orderid_, 1);
    EXPECT_EQ(trades[0].get_bid_trade().quantity_, 5);

    EXPECT_EQ(trades[1].get_bid_trade().orderid_, 2);
    EXPECT_EQ(trades[1].get_bid_trade().quantity_, 2);
}

TEST(OrderBookTest, FillAndKillSuccess) {
    order_book ob;

    auto sell = std::make_shared<order>(
        1, 100, side::sell, 10, order_type::good_till_cancel);

    auto fak_buy = std::make_shared<order>(
        2, 100, side::buy, 10, order_type::fill_and_kill);

    ob.add_order(sell);
    auto trades = ob.add_order(fak_buy);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].get_bid_trade().quantity_, 10);

    auto l2 = ob.get_l2_snapshot(5);
    EXPECT_TRUE(l2.bids_.empty());
    EXPECT_TRUE(l2.asks_.empty());
}
TEST(OrderBookTest, FillAndKillPartialFillThenCancelRemainder) {
    order_book ob;

    // Only 5 available
    auto sell = std::make_shared<order>(
        1, 100, side::sell, 5, order_type::good_till_cancel);

    auto fak_buy = std::make_shared<order>(
        2, 100, side::buy, 10, order_type::fill_and_kill);

    ob.add_order(sell);
    auto trades = ob.add_order(fak_buy);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].get_bid_trade().orderid_, 2);
    EXPECT_EQ(trades[0].get_bid_trade().quantity_, 5);

    // Remainder must NOT rest on book
    auto l2 = ob.get_l2_snapshot(5);
    EXPECT_TRUE(l2.bids_.empty());
    EXPECT_TRUE(l2.asks_.empty());
}

TEST(OrderBookTest, CancelOrderRemovesFromBook) {
    order_book ob;

    auto buy = std::make_shared<order>(
        1, 100, side::buy, 10, order_type::good_till_cancel);

    ob.add_order(buy);
    ob.cancel_order(1);

    auto l2 = ob.get_l2_snapshot(5);
    EXPECT_TRUE(l2.bids_.empty());
}

TEST(OrderBookTest, DuplicateOrderIdThrows) {
    order_book ob;

    auto o1 = std::make_shared<order>(
        1, 100, side::buy, 10, order_type::good_till_cancel);

    auto o2 = std::make_shared<order>(
        1, 101, side::buy, 10, order_type::good_till_cancel);

    ob.add_order(o1);
    EXPECT_THROW(ob.add_order(o2), std::logic_error);
}

TEST(OrderBookTest, L2SnapshotAggregatesQuantity) {
    order_book ob;

    ob.add_order(std::make_shared<order>(
        1, 100, side::buy, 10, order_type::good_till_cancel));

    ob.add_order(std::make_shared<order>(
        2, 100, side::buy, 20, order_type::good_till_cancel));

    auto l2 = ob.get_l2_snapshot(5);

    ASSERT_EQ(l2.bids_.size(), 1);
    EXPECT_EQ(l2.bids_[0].quantity_, 30);
}