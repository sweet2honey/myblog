# 为什么 Rust 的 HashMap 这么难用啊？

## 前情提要

Rust 的 `HashMap` 在官方的文档里面是一个人畜无害的东西，甚至还因为有 `*map.entry().or_default() += 1` 这种很 rusty 的东西，让人觉得十分有魔力。



直到我在刷 leetcode 的时候没办法写出类似这样的 C++ 代码（不是 Rust 不行是我不行），我就知道事情远远没有这么简单...

```cpp
#include <unordered_map>

int main() {
    auto map = std::unordered_map<int32_t, int32_t>{};

    // init
    for (auto i : {1, 2, 3}) {
        map.emplace(i, i);
    }
    assert(map.size() == 3);

    // find key '1', reduce value by 1, delete if value is 0
    if (const auto it = map.find(1); it != map.end()) {
        it->second--;
        assert(it->second == 0);
        if (it->second == 0) {
            map.erase(it);
        }
    }
    assert(map.size() == 2);
}
```



好吧，那就来看看 `std::collections::HashMap` 吧。



## 回顾例子

可以先回顾一下简单的例子：

[在哈希 map 中存储键和关联值 - Rust 程序设计语言 中文版 (rustwiki.org)](https://rustwiki.org/zh-CN/book/ch08-03-hash-maps.html)

[散列表 HashMap - 通过例子学 Rust 中文版 (rustwiki.org)](https://rustwiki.org/zh-CN/rust-by-example/std/hash.html)



里面提到这些点，稍微记录一下：

1. 可以通过 `HashMap::new()` 或者 `zip` 迭代器然后 `collect` 创建一个 map；
2. `insert(K, V)` 是会移走所有权的；
3. `get(&K) -> Option<&V>`；
4. `Entry` 的 `or_insert() -> &mut V`；
5. `HashMap::iter() -> Iter<'a, K: 'a, V: 'a>` 且 `type Item = (&'a K, &'a V)`；

## 文档

[HashMap in std::collections::hash_map - Rust (rust-lang.org)](https://doc.rust-lang.org/std/collections/hash_map/struct.HashMap.html)



### 基础

```rust
pub struct HashMap<K, V, S = RandomState>
```

- 使用的哈希算法

  The default hashing algorithm is currently SipHash 1-3；

  The hashing algorithm can be replaced on a per-`HashMap` basis using the [`default`](https://doc.rust-lang.org/std/default/trait.Default.html#tymethod.default), [`with_hasher`](https://doc.rust-lang.org/std/collections/hash_map/struct.HashMap.html#method.with_hasher), and [`with_capacity_and_hasher`](https://doc.rust-lang.org/std/collections/hash_map/struct.HashMap.html#method.with_capacity_and_hasher) methods.

- Key 的 trait

  It is required that the keys implement the [`Eq`](https://doc.rust-lang.org/std/cmp/trait.Eq.html) and [`Hash`](https://doc.rust-lang.org/std/hash/trait.Hash.html) traits；

  可以使用：`#[derive(PartialEq, Eq, Hash)]`

### HashMap 的 Methods

- 构造：

  `new()` 、`new_with_capacity()`：初始化一个 map， 附带初始容量。

  `with_hasher()`、` with_capacity_and_hasher()`：可以指定 hasher。

- 基本信息：

  `capacity()`、`len()`、`is_empty()`、`hasher()`

- 访问型迭代器：

  `keys() -> Keys<'_, K, V> `：

  ```rust
  impl<'a, K, V> Iterator for Keys<'a, K, V>
  type Item = &'a K
  ```

  可以看到 key 是不支持修改的。

  

  `values() -> Values<'_, K, V>`

  `values_mut() -> ValuesMut<'_, K, V> `：

  ```rust
  impl<'a, K, V> Iterator for Values<'a, K, V>
  type Item = &'a V
  
  impl<'a, K, V> Iterator for ValuesMut<'a, K, V>
  type Item = &'a mut V
  ```

  

  `iter() -> Iter<'_, K, V> `

  `iter_mut() -> IterMut<'_, K, V>`：

  ```rust
  impl<'a, K, V> Iterator for Iter<'a, K, V>
  type Item = (&'a K, &'a V)
  
  impl<'a, K, V> Iterator for IterMut<'a, K, V>
  type Item = (&'a K, &'a mut V)
  ```

- 消费型迭代器：

  `into_keys()`、`into_values()`

### hash_map::Entry





## 回到故事本身

```rust
fn main() {
    use std::collections::HashMap;

    let mut map = HashMap::new();
    map.insert(1, 1);
    map.insert(2, 2);
    map.insert(3, 3);
    assert_eq!(map.len(), 3);

    // find key '1', reduce value by 1, delete if value is 0.

    // way 1

    assert_eq!(map.get(&1), None);
}
```

