#include <vector>

#include "shared_ptr.hh"

struct A {
  int i = 0;
};

struct B : A {
  int j = 1;
};

int main() {
  SharedPtr sp_b = SharedPtr<B>::make_shared();
  SharedPtr<A> sp_a{sp_b};
  SharedPtr sp_vec = SharedPtr<std::vector<int>>::make_shared();

  std::cout << sp_a->i << '\n';
  std::cout << sp_b->j << '\n';
  std::cout << sp_vec.get()->size() << '\n';
  return 0;
}
