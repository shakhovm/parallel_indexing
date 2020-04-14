#ifndef CONC_QUEUE_H
#define CONC_QUEUE_H


#include <deque>
#include <mutex>
#include <condition_variable>

template <typename T>
class synch_queue {
private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv;
    size_t counter = 0;
    size_t mul = -1;
public:
    synch_queue() = default;
    ~synch_queue() = default;
    void push(const T& value) {
        {
            std::lock_guard<std::mutex> lg{mutex_};
            queue_.push_back(value);
        }
        cv.notify_one();
    }

    void push(T&& value) {
        {
            std::lock_guard<std::mutex> lg{mutex_};
            queue_.push_back(value);
        }
        cv.notify_one();
    }

    template <typename ...Args>
    void emplace(Args&&... args) {
        {
            std::lock_guard<std::mutex> lg{mutex_};
            queue_.emplace_back(args...);
        }
        cv.notify_one();
    }

    void set_mul(size_t value) {
        std::lock_guard<std::mutex> lg{mutex_};
        mul = value;
    }

    void increase_value() {
        std::lock_guard<std::mutex> lg{mutex_};
        counter++;
    }

    size_t get_counter() {
        std::lock_guard<std::mutex> lg{mutex_};
        return counter;
    }

    size_t get_mul() {
        std::lock_guard<std::mutex> lg{mutex_};
        return mul;
    }

    T pop() {
        std::unique_lock<std::mutex> ul{mutex_};
        cv.wait(ul, [this]{return !queue_.empty(); });
        T elem = queue_.front();
        queue_.pop_front();
        return elem;
    }

    size_t size() {
        return queue_.size();
    }
};

#endif

