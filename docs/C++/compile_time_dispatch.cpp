#include <algorithm> // copy_n
#include <cstddef>   // std::size_t
#include <cstring>   // strncmp
#include <iostream>

template<std::size_t N>
struct string_literal {
    constexpr string_literal(const char (&arr)[N]) noexcept {
        std::copy_n(arr, N, value);
    }

    bool operator==(const char* other) const noexcept {
        return std::strncmp(value, other, N) == 0;
    }

    operator const char*() const {
        return value;
    }

    char value[N];
};

template<string_literal C>
inline constexpr auto handler = [] {};

template<>
inline constexpr auto handler<"C++"> = [] { std::cout << "C++ is getting better\n"; };

template<>
inline constexpr auto handler<"rust"> = [] { std::cout << "rust is safe\n"; };

template<string_literal... Cs>
struct dispatcher {
public:
    bool execute(const char* name) const noexcept {
        return (execute_if<Cs>(name) || ...);
    }

private:
    template<string_literal C>
    bool execute_if(const char* name) const noexcept {
        if (C == name) {
            handler<C>();
            return true;
        }

        return false;
    }
};

int main() {
    constexpr string_literal cpp = string_literal("C++");
    constexpr auto dispatch = dispatcher<"C++", "rust", "honey">{};

    dispatch.execute("C++");
    dispatch.execute("rust");
    dispatch.execute("honey");
    dispatch.execute("non exist");
}