#include <iostream>
#include <cstdlib>
#include "typelist.hpp"
#include <tuple>
#include <cmath>
#include <type_traits>


//base opereator
template<class> struct list_helper;

template<template<typename...> class L, typename... Ts>
struct list_helper<L<Ts...>>
{
    template<template<typename> class F>
    using map = L<F<Ts>...>;
    
    static constexpr int length = sizeof...(Ts);
    
    template<template<typename...> class V>
    using cast = V<Ts...>;
    
    template<typename T>
    static constexpr bool contain = std::disjunction_v<std::is_same<T, Ts>...>;
    
    template<class T> struct concat;
    template<template<typename...> class _L, typename... _Ts>
    struct concat<_L<_Ts...>>
    {
        using type = L<Ts..., _Ts...>;
    };
};

template<typename L, template<typename> class F>
using map_t = typename list_helper<L>::template map<F>;

template<typename L>
constexpr int length_v = list_helper<L>::length;

template<typename T, typename U>
using cast_t = typename list_helper<T>::template cast<U>;

template<typename L, typename T>
constexpr bool contain_v = list_helper<L>::template contain<T>;

template<typename T, typename U>
using concat_t = typename list_helper<T>::template concat<U>::type;

template<typename T>
struct convert
{
    using type = double;
};

template<typename T>
using convert_t = typename convert<T>::type;

int main()
{
    using _source_tuple = std::tuple<int,int,int>;
    using _return_tuple = map_t<_source_tuple, convert_t>;
    using _expect_tuple = std::tuple<double, double, double>;
    
    std::cout << "is_same: " << std::is_same_v<_return_tuple, _expect_tuple> << std::endl;
    std::cout << "length: " << length_v<_expect_tuple> << std::endl;
    std::cout << "contain: " << contain_v<_expect_tuple, double> << std::endl;
    
    using _expect_concat_tuple = std::tuple<int, int, int, double, double, double>;
    using _concat_after_tuple = concat_t<_source_tuple, _expect_tuple>;
    std::cout << "is same: " << std::is_same_v<_concat_after_tuple, _expect_concat_tuple> << std::endl;
}


