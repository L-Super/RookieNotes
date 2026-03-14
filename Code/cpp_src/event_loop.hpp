#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>

class EventLoop {
public:
    using Task = std::function<void()>;

    EventLoop() : running_(false) {
    }

    ~EventLoop() {
        stop();
    }
    
    void start() {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) {
            return;
        }
        worker_ = std::thread([this]() { this->runLoop(); });
    }
    
    void stop() {
        bool expected = true;
        if (!running_.compare_exchange_strong(expected, false)) {
            return;
        }
        
        cv_.notify_all();

        if (worker_.joinable()) {
            worker_.join();
        }
    }

    void post(Task task) { {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.push(std::move(task));
        }
        cv_.notify_one();
    }
    
    bool isRunning() const {
        return running_.load();
    }

private:
    void runLoop() {
        while (running_.load()) {
            Task task; 
            {
                std::unique_lock<std::mutex> lock(mutex_);

                // 如果任务队列为空，就阻塞等待
                cv_.wait(lock, [this]() {
                    return !running_.load() || !tasks_.empty();
                });

                if (!running_.load() && tasks_.empty()) {
                    break; // 收到停止信号并且没有任务了
                }

                if (!tasks_.empty()) {
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
            }

            if (task) {
                try {
                    task();
                } catch (const std::exception &e) {
                    std::cerr << "EventLoop task exception: " << e.what() << std::endl;
                }
            }
        }
    }

private:
    std::atomic<bool> running_;
    std::thread worker_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<Task> tasks_;
};