//
// Created by Leo on 25-1-16.
//
// Future 类封装了 std::future，并提供了 then 方法，用于在 future 完成后执行回调函数。
// 该类的目的是实现无阻塞异步操作。
#pragma once
#include <chrono>
#include <functional>
#include <future>
#include <thread>

template <typename T>
class Future {
 public:
  explicit Future(std::future<T>&& future) noexcept : future_(std::move(future)) {}

  template <typename F>
  auto then(F&& func) -> Future<decltype(func(std::declval<T>()))> {
    using ResultType = decltype(func(std::declval<T>()));
    std::promise<ResultType> promise;
    auto newFuture = promise.get_future();

    std::thread([p = std::move(promise), f = std::forward<F>(func), fut = std::move(future_)]() mutable {
      try {
        if constexpr (std::is_void_v<ResultType>) {
          f(fut.get());
          p.set_value();
        } else {
          p.set_value(f(fut.get()));
        }
      } catch (...) {
        p.set_exception(std::current_exception());
      }
    }).detach();

    return Future<ResultType>(std::move(newFuture));
  }

  T get() { return future_.get(); }

  void wait() const { future_.wait(); }

  [[nodiscard]] bool valid() const { return future_.valid(); }

  template <class Rep, class Period>
  std::future_status wait_for(const std::chrono::duration<Rep, Period>& rel_time) const {
    return future_.wait_for(rel_time);
  }

  template <class Clock, class Duration>
  std::future_status wait_until(const std::chrono::time_point<Clock, Duration>& abs_time) const {
    return future_.wait_until(abs_time);
  }

 private:
  std::future<T> future_;
};