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
