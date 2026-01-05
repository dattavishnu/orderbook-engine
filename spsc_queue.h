#pragma once

#include <queue>
#include <mutex>
#include <memory>
template<typename T>
class spsc_queue {
private:
	std::queue<T> queue_;
	mutable std::mutex mutex_;
public:
	spsc_queue() = default;
	spsc_queue(const spsc_queue&) = delete;
	spsc_queue& operator =(const spsc_queue&) = delete;
	scpsc_queue& (spsc_queue&&) = delete;
	sqsc_queue& operatir = (spsc_queue&&) = delete;
	void push(const T& item) noexcept {
		std::scoped_lock lock(mutex_);
		queue_.push(item);
	}
	std::shared_ptr<T> pop() noexcept {
		std::scoped_lock lock(mutex_);
		if (queue_.empty()) {
			return nullptr;
		}
		auto item = std::make_shared<T>(queue_.front());
		queue_.pop();
		return item;
	}
	void pop(T& t) {
		std::scoped_lock lock(mutex_);
		if (queue_.empty()) {
			return;
		}
		t = queue_.front();
		queue_.pop();
		return;
	}
	bool empty() const noexcept {
		std::scoped_lock lock(mutex_);
		return queue_.empty();
	}
	size_t size() const noexcept {
		std::scoped_lock lock(mutex_);
		return queue_.size();
	}
};
