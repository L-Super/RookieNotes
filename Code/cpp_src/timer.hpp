/**
 * RAII 计时器
 * 计算运行耗时，同时打印出对应文件位置及函数名
 */

#pragma once
#include <chrono>
#include <iostream>

struct source_location
{
    constexpr source_location(const char *file_name = __builtin_FILE(),
                              const char *function_name = __builtin_FUNCTION(),
                              unsigned int line = __builtin_LINE()) noexcept
        : file_name_(file_name), function_name_(function_name), line_(line)
    {
    }

    static constexpr source_location current(const char *file_name = __builtin_FILE(),
                                             const char *function_name = __builtin_FUNCTION(),
                                             const unsigned int line_number = __builtin_LINE()) noexcept
    {
        return {file_name, function_name, line_number};
    }
    constexpr const char *file_name() const noexcept
    {
        return file_name_;
    }
    constexpr const char *function_name() const noexcept
    {
        return function_name_;
    }
    constexpr unsigned int line() const noexcept
    {
        return line_;
    }

  private:
    const char *file_name_;
    const char *function_name_;
    const unsigned int line_;
};

class Timer
{
  private:
    std::chrono::time_point<std::chrono::system_clock> start_;
    source_location location_;

  public:
    explicit Timer(const source_location &location = source_location::current())
        : start_(std::chrono::system_clock::now()), location_(location)
    {
    }
    ~Timer()
    {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start_;
        std::string func_name(location_.function_name());
        if (func_name.empty())
            std::cout << "function takes time：" << elapsed.count() << " ms\n";
        else
            std::cout << location_.file_name() << ":" << location_.line() << " " << func_name << " takes time："
                      << elapsed.count() << " ms\n";
    }
};
