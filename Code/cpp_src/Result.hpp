//
// Created by Leo on 24-11-27.
//
/// Result type, similar to Rust Result
/// example
/// Result<int, std::string> divide(int numerator, int denominator) {
///     if (denominator == 0) {
///         return Result<int, std::string>::Err("Division by zero");
///     }
///     return Result<int, std::string>::Ok(numerator / denominator);
/// }
///
/// int main() {
///     // success
///     auto success_result = divide(10, 2);
///     success_result.match(
///         [](int value) { std::cout << "Success: " << value << std::endl; },
///         [](const std::string& err) { std::cout << "Error: " << err << std::endl; }
///     );
///     std::println("is_ok:{} is_err:{} unwrap:{}", success_result.is_ok(), success_result.is_err(),
///     success_result.unwrap());
///
///     // wrong
///     auto error_result = divide(10, 0);
///     error_result.match(
///         [](int value) { std::cout << "Success: " << value << std::endl; },
///         [](const std::string& err) { std::cout << "Error: " << err << std::endl; }
///     );
///     std::println("is_ok:{} is_err:{} unwrap_err:{}", error_result.is_ok(), error_result.is_err(),
///     error_result.unwrap_err());
///
///     return 0;
/// }

#pragma once
#include <any>
#include <iostream>
#include <stdexcept>
#include <string>

// custom exception（optional）
class ResultException final : public std::runtime_error {
 public:
  explicit ResultException(const std::string& message) : std::runtime_error(message) {}
};

// Result
template <typename T, typename E = std::string>
class Result {
 private:
  // Using variant is actually better, but when T and E are of the same type, it can cause ambiguity.
  // std::variant<T, E> data;
  std::any ok_;
  std::any err_;

 public:
  // Construct a successful Result
  static Result Ok(T value) {
    Result result;
    result.ok_ = std::move(value);
    return result;
  }

  // Construct a wrong Result
  static Result Err(E error) {
    Result result;
    result.err_ = std::move(error);
    return result;
  }

  // Check if successful
  [[nodiscard]] bool is_ok() const { return ok_.has_value(); }

  // Check if error
  [[nodiscard]] bool is_err() const { return err_.has_value(); }

  // Get the successful value and throw an exception if it is an error
  T unwrap() const {
    if (is_err()) {
      throw ResultException("Unwrapped an Err value");
    }
    return std::any_cast<T>(ok_);
  }

  // Get the error value and throw an exception if successful
  E unwrap_err() const {
    if (is_ok()) {
      throw ResultException("Unwrapped_err on an Ok value");
    }
    return std::any_cast<E>(err_);
  }

  // Get values safely, provide default values
  T unwrap_or(T default_value) const { return is_ok() ? std::any_cast<T>(ok_) : default_value; }

  // Pattern matching style processing
  template <typename OkFunc, typename ErrFunc>
  auto match(OkFunc ok_func, ErrFunc err_func) const {
    if (is_ok()) {
      return ok_func(std::any_cast<T>(ok_));
    } else {
      return err_func(std::any_cast<E>(err_));
    }
  }
};
