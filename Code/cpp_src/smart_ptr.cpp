#include <utility>

template <typename T>
class unique_ptr {
public:
	explicit unique_ptr(T* ptr = nullptr)
		: ptr_(ptr) {}
	~unique_ptr()
	{
		delete ptr_;
	}
	unique_ptr(unique_ptr&& other)
	{
		ptr_ = other.release();
	}
	// 子类指针向基类指针的转换
	template <typename U>
	unique_ptr(unique_ptr<U>&& other)
	{
		ptr_ = other.release();
	}
	unique_ptr& operator=(unique_ptr rhs)
	{
		rhs.swap(*this);
		return *this;
	}
	T* release()
	{
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}
	void swap(unique_ptr& rhs)
	{
		using std::swap;
		swap(ptr_, rhs.ptr_);
	}
	T* get() const { return ptr_; }
	T& operator*() const { return *ptr_; }
	T* operator->() const { return ptr_; }
	operator bool() const { return ptr_; }
private:
	T* ptr_;
};


class shared_count {
public:
	shared_count() noexcept : count_(1) {}
	void add_count() noexcept
	{
		++count_;
	}
	long reduce_count() noexcept
	{
		return --count_;
	}
	long get_count() const noexcept
	{
		return count_;
	}

private:
	long count_;
};

template <typename T>
class shared_ptr {
public:
	template <typename U>
	friend class shared_ptr;

	explicit shared_ptr(T* ptr = nullptr) : ptr_(ptr)
	{
		if (ptr) {
			shared_count_ = new shared_count();
		}
	}
	~shared_ptr()
	{
		if (ptr_ && !shared_count_->reduce_count()) {
			delete ptr_;
			delete shared_count_;
		}
	}

	shared_ptr(const shared_ptr& other)
	{
		ptr_ = other.ptr_;
		if (ptr_) {
			other.shared_count_->add_count();
			shared_count_ = other.shared_count_;
		}
	}
	template <typename U>
	shared_ptr(const shared_ptr<U>& other) noexcept
	{
		ptr_ = other.ptr_;
		if (ptr_) {
			other.shared_count_->add_count();
			shared_count_ = other.shared_count_;
		}
	}
	template <typename U>
	shared_ptr(shared_ptr<U>&& other) noexcept
	{
		ptr_ = other.ptr_;
		if (ptr_) {
			shared_count_ = other.shared_count_;
			other.ptr_ = nullptr;
		}
	}
	template <typename U>
	shared_ptr(const shared_ptr<U>& other, T* ptr) noexcept
	{
		ptr_ = ptr;
		if (ptr_) {
			other.shared_count_->add_count();
			shared_count_ = other.shared_count_;
		}
	}
	shared_ptr& operator=(shared_ptr rhs) noexcept
	{
		rhs.swap(*this);
		return *this;
	}

	T* get() const noexcept
	{
		return ptr_;
	}
	long use_count() const noexcept
	{
		if (ptr_) {
			return shared_count_->get_count();
		}
		else {
			return 0;
		}
	}
	void swap(shared_ptr& rhs) noexcept
	{
		using std::swap;
		swap(ptr_, rhs.ptr_);
		swap(shared_count_, rhs.shared_count_);
	}

	T& operator*() const noexcept
	{
		return *ptr_;
	}
	T* operator->() const noexcept
	{
		return ptr_;
	}
	operator bool() const noexcept
	{
		return ptr_;
	}

private:
	T* ptr_;
	shared_count* shared_count_;
};

template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) noexcept
{
	lhs.swap(rhs);
}

// 对应std cast类型转换
template <typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& other) noexcept
{
	T* ptr = static_cast<T*>(other.get());
	return shared_ptr<T>(other, ptr);
}

template <typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& other) noexcept
{
	T* ptr = reinterpret_cast<T*>(other.get());
	return shared_ptr<T>(other, ptr);
}

template <typename T, typename U>
shared_ptr<T> const_pointer_cast(const shared_ptr<U>& other) noexcept
{
	T* ptr = const_cast<T*>(other.get());
	return shared_ptr<T>(other, ptr);
}

template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& other) noexcept
{
	T* ptr = dynamic_cast<T*>(other.get());
	return shared_ptr<T>(other, ptr);
}

#include <iostream>
class shape {
public:
	virtual ~shape() {}
};

class circle : public shape {
public:
	~circle() { std::cout<<"~circle()\n"; }
};

int main()
{
	shared_ptr<circle> ptr1(new circle());
	std::cout << "use count of ptr1 is" << ptr1.use_count() << "\n";
	shared_ptr<shape> ptr2;
	std::cout << "use count of ptr2 was " << ptr2.use_count() << "\n";
	ptr2 = ptr1;
	std::cout << "use count of ptr2 is now " << ptr2.use_count() << "\n";
	if (ptr1) {
		std::cout<<"ptr1 is not empty\n";
	}

	shared_ptr<circle> ptr3 = dynamic_pointer_cast<circle>(ptr2);
	std::cout << "use count of ptr3 is " << ptr3.use_count() << "\n";
}
