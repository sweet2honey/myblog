#include <type_traits>
#include <utility>
#include <variant>

/* variant like trait */
template <typename> struct is_variant_like : std::false_type {};
template <typename... Types>
struct is_variant_like<std::variant<Types...>> : std::true_type {};

static_assert(is_variant_like<std::variant<uint32_t, double>>{});
static_assert(!is_variant_like<std::size_t>{});

/* variant types all integral */
template <typename Var>
using variant_index_sequence =
    std::make_index_sequence<std::variant_size_v<Var>>;

template <typename Var> class is_variant_all_integral_ {
private:
  template <std::size_t... Is>
  static auto check(std::index_sequence<Is...>) -> std::conjunction<
      std::is_integral<std::variant_alternative_t<Is, Var>>...>;

public:
  using type = decltype(check(variant_index_sequence<Var>{}));
};

#define TEST_(v, ...)                                                          \
  {                                                                            \
    using Var = std::variant<__VA_ARGS__>;                                     \
    static_assert(v == is_variant_all_integral_<Var>::value);                  \
  }

#define TEST_TRUE(...) TEST_(1, __VA_ARGS__)
#define TEST_FALSE(...) TEST_(0, __VA_ARGS__)

/* TODO: pass in any predicates ? */

int main() {
  TEST_TRUE(bool, uint8_t);
  TEST_TRUE(uint16_t, uint32_t);
  TEST_TRUE(uint32_t, std::size_t);
  TEST_FALSE(uint16_t, float);
}
