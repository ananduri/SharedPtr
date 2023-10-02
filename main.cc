#include <vector>

#include "shared_ptr.hh"

struct A {
  int i = 0;
  ~A() {}
};

int main() {
  // A a{};
  // std::vector<int> v;

  // SharedPtr sp = SharedPtr<A>::make_shared();
  // SharedPtr sp_vec = SharedPtr<std::vector<int>>::make_shared();

  // std::cout << sp->i << '\n';
  // std::cout << sp_vec.get()->size() << '\n';

  SharedPtr nested = SharedPtr<SharedPtr<A>>::make_shared();
  std::cout << nested->operator->()->i << '\n';

  return 0;
}
