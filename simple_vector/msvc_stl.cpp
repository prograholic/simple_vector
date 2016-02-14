#include <type_traits>
#include <memory>

using namespace std;

typedef pointer_traits<shared_ptr<int>>::rebind<long> sp_long_t;

static_assert(is_same<sp_long_t, shared_ptr<long>>::value, "rebind is broken?");