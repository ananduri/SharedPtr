#include <cassert>
#include <iostream>

// Making this thread-safe is out of scope.
// Could stress-test by making SharedPtr<SharedPtr<T>> objects
// ^that doesn't work because the default ctor is deleted--defaulting it allows
// this to work

template <typename T> class SharedPtr final {
public:
  // SharedPtr() = delete;
  SharedPtr() = default;

  ~SharedPtr() {
    --(*ref_count_);
    delete_data();
  }

  template <typename... Args> static SharedPtr make_shared(Args &&...args) {
    T *const ptr = new T(std::forward<Args>(args)...);
    size_t *ref_count = new size_t{0};
    return SharedPtr{ptr, ref_count};
  }

  // Also think about copy ctor for creating from types convertible to T.
  // "generalized copy ctor"
  SharedPtr(SharedPtr const &shared_ptr)
      : ptr_{shared_ptr.ptr_}, ref_count_{shared_ptr.ref_count_} {
    ++(*ref_count_);
  }

  SharedPtr &operator=(SharedPtr const &shared_ptr) {
    --(*ref_count_);
    delete_data();
    ptr_ = shared_ptr.ptr_;
    ref_count_ = shared_ptr.ref_count_;
    ++(*ref_count_);
    return *this;
  }

  // Move operations also need to "zero out" the moved-from object.
  SharedPtr(SharedPtr &&shared_ptr) noexcept
      : ptr_{shared_ptr.ptr_}, ref_count_{shared_ptr.ref_count_} {
    shared_ptr.ptr_ = nullptr;
    shared_ptr.ref_count = nullptr;
  }

  SharedPtr &operator=(SharedPtr &&shared_ptr) noexcept {
    --(*ref_count_);
    delete_data();
    ptr_ = shared_ptr.ptr_;
    ref_count_ = shared_ptr.ref_count_;
    shared_ptr.ptr_ = nullptr;
    shared_ptr.ref_count_ = nullptr;
    return *this;
  }

  T *get() const { return ptr_; }

  T operator*() const { return *ptr_; }

  T *operator->() const { return ptr_; }

private:
  explicit SharedPtr(T *ptr, size_t *ref_count)
      : ptr_{ptr}, ref_count_{ref_count} {}

  void delete_data() {
    assert(*ref_count_ >= 0u);
    assert(ptr_ != nullptr);
    if (*ref_count_ == 0u) {
      ptr_->~T();
      delete ptr_;
      delete ref_count_;
    }
  }

  T *ptr_ = nullptr;
  size_t *ref_count_ = nullptr;
};
