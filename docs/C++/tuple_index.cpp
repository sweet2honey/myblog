#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

template<typename T, typename Tuple>
struct TupleIndex;

template<typename T, typename... Types>
struct TupleIndex<T, std::tuple<T, Types...>> {
    static constexpr size_t value = 0U;
};

template<typename T, typename U, typename... Types>
struct TupleIndex<T, std::tuple<U, Types...>> {
    static constexpr size_t value = 1 + TupleIndex<T, std::tuple<Types...>>::value;
};

template<typename T, typename Tuple>
inline constexpr auto tuple_index_v = TupleIndex<T, Tuple>::value;

template<typename T, typename Tuple>
inline constexpr auto tuple_index() noexcept -> size_t {
    return tuple_index_v<T, Tuple>;
}

int main() {
    using T1 = std::tuple<int8_t, int16_t, int32_t>;
    static_assert(tuple_index_v<int8_t, T1> == 0);
    static_assert(tuple_index_v<int16_t, T1> == 1);
    static_assert(tuple_index_v<int32_t, T1> == 2);

    using T2 = std::tuple<int8_t, int32_t, int8_t>;
    static_assert(tuple_index<int8_t, T1>() == 0);

    using T3 = std::tuple<std::string, std::vector<int32_t>>;
    static_assert(tuple_index<std::string, T3>() == 0);
    static_assert(tuple_index<std::vector<int32_t>, T3>() == 1);
    // static_assert(tuple_index<double, T3>() == -1); // won't compile
}