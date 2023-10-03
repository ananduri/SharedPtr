#include <cassert>
#include <iostream>

// Making this thread-safe is out of scope.

template <typename T> class SharedPtr final {
public:
  SharedPtr() = delete;

  ~SharedPtr() {
    --(*ref_count_);
    delete_data();
  }

  template <typename... Args> static SharedPtr make_shared(Args &&...args) {
    T *const ptr = new T(std::forward<Args>(args)...);
    size_t *ref_count = new size_t{0};
    return SharedPtr{ptr, ref_count};
  }

  SharedPtr(SharedPtr const &shared_ptr)
      : ptr_{shared_ptr.ptr_}, ref_count_{shared_ptr.ref_count_} {
    ++(*ref_count_);
  }

  // Can also add generalized move ctor and copy/move assignment operator.
  template <typename U>
  SharedPtr(SharedPtr<U> const &shared_ptr)
      : ptr_{shared_ptr.get()}, ref_count_{shared_ptr.get_count()} {
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

  size_t *get_count() const { return ref_count_; }

private:
  explicit SharedPtr(T *ptr, size_t *ref_count)
      : ptr_{ptr}, ref_count_{ref_count} {}

  void delete_data() {
    assert(*ref_count_ >= 0u);
    assert(ptr_ != nullptr);
    if (*ref_count_ == 0u) {
      delete ptr_;
      delete ref_count_;
    }
  }

  T *ptr_ = nullptr;
  size_t *ref_count_ = nullptr;
};
