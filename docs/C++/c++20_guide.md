# C++ 20 The Complete Guide 
## Source
感谢 Nicolai 大叔！

[C++20 - The Complete Guide](https://cppstd20.com/)

## Preface
I'm not sure 看完 *C++ 17 The Complete Guide* 之后的东西我还记得多少，或许有 `optional` `variant` `_v` `if constexpr` ？
所以这次开坑 C++ 20 的时候，就顺手记一下吧。

## Comparisons and Operator <=>
> The new operator `<=>` (also called the spaceship operator) was introduced。

火箭🚀运算符。

### Motivation
想要很好地定义一个结构体的比较运算符是很麻烦的，通常：
1. 要写 6 个函数（尽管都是通过 `==` 和 `<` 来定义的）；
2. `constexpr` `noexcept` `nodiscard`
3. 引入 `friend`

于是显得很啰嗦、繁琐。

在 C++ 20 里面，`==` 可以隐式地表示 `!=` 啦！
如果编译器找不到 `a != b` 的定义，它会尝试翻译成 `!(a == b)`，如果还不行，它会把 `a` `b` 顺序反过来。
于是，对于 `a != b`，但凡有以下几个函数之一，都是可以编译过的：
- A free-standing operator!=(TypeA, TypeB)
- A free-standing operator==(TypeA, TypeB)
- A free-standing operator==(TypeB, TypeA)
- A member function TypeA::operator!=(TypeB)
- A member function TypeA::operator==(TypeB)
- A member function TypeB::operator==(TypeA)

通常来说， `==` 处理相等的情况，`<=>` 处理关系顺序，通过 `<=> = default` 定义后，我们获得了两个运算符的默认实现，实现则是*逐个比较成员*，所以，成员的定义顺序会影响两个运算符的结果。
```c++
class Value {
    ...
    auto operator<=>(const Value& rhs) const = default;
    auto operator==(const Value& rhs) const = default;  // implicitly generated
};
```

于是这两个运算符处理不同、但却相关的东西：
> operator== defines equality and can be used by the equality operators == and !=.
> 
> operator<=> defines the ordering and can be used by the relational operators <, <=, >, and >=.

你也可以实现自己的 `<=>` 运算符来更好地控制你的类的行为，这样就能决定在比较中哪些成员需要参与。

这种机制得以实现，是因为：在对比较表达式求值的时候，如果编译器找不到对应类型的运算符的直接定义，它会 **重写 `rewritten`** 表达式，以求找到一个合适实现。
重写会调用相反的运算符、调换参数的顺序、甚至允许第一个操作数的隐式转换。

如：
```c++
x <= y
(x <=> y) <= 0
0 <= (y <=> x)
```
就是一种可能的重写的路径。

这种方式会进行三向比较 `three way comparison`，返回一个可以跟 0 比较的值。
> If the value of x<=>y is equal to 0, x and y are equal or equivalent.
> 
> If the value of x<=>y is less than 0, x is less than y.
> 
> If the value of x<=>y is greater than 0, x is greater than y.

`<=>` 的返回值是 `comparison category` 中的一种，可能是 `strong ordering` `weak ordering` `partial ordering`。

### Defining and Using Comparisons
记得引头文件 `#include <compare>`。

`<=>` 返回的不是布尔值，而是一个表示小于、大于、或者相等/相同的三目比较结果，这跟 C 的 `strcmp()` 有点像，最大的区别是，这**不是一个整数**。C++ 标准库提供了三种可能的返回值类型，用于表示比较的类型 `category`。

**Comparison Categories**
