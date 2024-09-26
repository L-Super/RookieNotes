/**
 * @file scoped_thread.h
 * @author Listening (you@domain.com)
 * @brief C++ 20 std::jthread 简单实现。源自《C++现代编程实战》
 * @date 2022-12-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <thread>

class scoped_thread
{
public:
	template <typename... Arg>
	scoped_thread(Arg &&...arg)
			: thread_(std::forward<Arg>(arg)...)
	{
	}
	scoped_thread(scoped_thread &&other)
			: thread_(std::move(other.thread_))
	{
	}
	scoped_thread(const scoped_thread &) = delete;
	~scoped_thread()
	{
		if (thread_.joinable())
		{
			thread_.join();
		}
	}

	scoped_thread &operator=(const scoped_thread &) = delete;
	scoped_thread &operator=(scoped_thread &&__t) noexcept
	{
		if (thread_.joinable())
			std::terminate();
		this->thread_.swap(__t.thread_);
		return *this;
	}

private:
	std::thread thread_;
};

/**
 * @class C++ 20 jthread 简单实现
 * 源自C++ 并发编程第2版 - 2.3 转移所有权
 */
class joining_thread
{
	std::thread t;

public:
	joining_thread() noexcept = default;
	template <typename Callable, typename... Args>
	explicit joining_thread(Callable &&func, Args &&...args) : t(std::forward<Callable>(func), std::forward<Args>(args)...)
	{
	}
	explicit joining_thread(std::thread t_) noexcept : t(std::move(t_))
	{
	}
	joining_thread(joining_thread &&other) noexcept : t(std::move(other.t))
	{
	}
	joining_thread &operator=(joining_thread &&other) noexcept
	{
		if (joinable())
		{
			join();
		}
		t = std::move(other.t);
		return *this;
	}
	joining_thread &operator=(std::thread other) noexcept
	{
		if (joinable())
			join();
		t = std::move(other);
		return *this;
	}
	~joining_thread() noexcept
	{
		if (joinable())
			join();
	}
	void swap(joining_thread &other) noexcept
	{
		t.swap(other.t);
	}
	std::thread::id get_id() const noexcept
	{
		return t.get_id();
	}
	bool joinable() const noexcept
	{
		return t.joinable();
	}
	void join()
	{
		t.join();
	}
	void detach()
	{
		t.detach();
	}
	std::thread &as_thread() noexcept
	{
		return t;
	}
	const std::thread &as_thread() const noexcept
	{
		return t;
	}
};