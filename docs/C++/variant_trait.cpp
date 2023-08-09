#include <stdint.h>
#include <type_traits>
#include <utility>
#include <variant>

/* variant like trait */
template<typename>
struct is_variant_like : std::false_type {};
template<typename... Types>
struct is_variant_like<std::variant<Types...>> : std::true_type {};

static_assert(is_variant_like<std::variant<uint32_t, double>>{});
static_assert(!is_variant_like<std::size_t>{});

/* variant types all integral */
template<typename Var>
using variant_index_sequence = std::make_index_sequence<std::variant_size_v<Var>>;

// helper
template<typename Var>
class is_variant_all_integral_ {
private:
    template<std::size_t... Is>
    static auto check(std::index_sequence<Is...>)
        -> std::conjunction<std::is_integral<std::variant_alternative_t<Is, Var>>...>;

public:
    using type = decltype(check(variant_index_sequence<Var>{}));
};

// type alias
template<typename Var>
using is_variant_all_integral = typename is_variant_all_integral_<Var>::type;

// variable template
template<typename Var>
inline static constexpr bool is_variant_all_integral_v = is_variant_all_integral<Var>::value;

#define TEST_(expect, ...)                                                                                   \
    {                                                                                                        \
        using Var = std::variant<__VA_ARGS__>;                                                               \
        static_assert((expect == is_variant_all_integral_v<Var>));                                           \
    }

#define TEST_TRUE(...) TEST_(true, __VA_ARGS__)
#define TEST_FALSE(...) TEST_(false, __VA_ARGS__)

/* TODO: pass in any predicates ? */

int main() {
    TEST_TRUE(bool, uint8_t);
    TEST_TRUE(uint16_t, uint32_t);
    TEST_TRUE(uint32_t, std::size_t);
    TEST_FALSE(uint16_t, float);
}