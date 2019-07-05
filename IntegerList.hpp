#include <iostream>
#include <type_traits>

template<int...Args>  struct List {};

template<typename List> struct Max;

template<template<int...> class List, int Last>
struct Max<List<Last>>
{    
    static constexpr int value = Last;
};

template<template<int...> class List, int First, int Second, int... Rest>
struct Max<List<First, Second, Rest...>>
{    
    static constexpr int value = Max<List<First >= Second ? First : Second, Rest...>>::value;
};


template<typename List, int Add> struct Append;


template<template<int...> class List, int Add, int... Rest>
struct Append<List<Rest...>, Add>
{    
    using type = List<Rest..., Add>;
};


template<typename List, int Idx> struct FindIndexElement;

template<template<int...> class List, int Idx, int First, int... Rest>
struct FindIndexElement<List<First, Rest...>, Idx>
{  
    static constexpr int value = FindIndexElement<List<Rest...>, Idx - 1>::value;  
};

template<template<int...> class List, int First, int... Rest>
struct FindIndexElement<List<First, Rest...>, 0>
{  
    static constexpr int value = First;
};

template<typename List1, typename List2> struct Concat;

template<typename List1, template<int...> class List2, int First, int... Rest>
struct Concat<List1, List2<First, Rest...>>
{
    using type = typename Concat<typename Append<List1, First>::type, List2<Rest...>>::type;
};

template<typename List1, template<int...> class List2>
struct Concat<List1, List2<>>
{
    using type = List1;
};

template<typename List, int Idx, int Value> struct Modify;

template<template<int...> class List, int Idx, int Value, int First, int... Rest>
struct Modify<List<First, Rest...>, Idx, Value>
{
private:
    using List1 = typename Append<List<>, First>::type;
    using List2  = typename Modify<List<Rest...>, Idx-1, Value>::type;
public: 
    using type = typename Concat<List1, List2>::type;
};

template<template<int...> class List, int Value, int First, int... Rest>
struct Modify<List<First, Rest...>, 0, Value>
{
    using type = List<Value, Rest...>;
};

template<typename List, int Index>  struct Remove;
template<typename List, int Value>  struct RemoveValue;

template<template<int...> class List, int Index, int First, int...Rest>
struct Remove<List<First,Rest...>, Index>
{
private:
    using List1  =  typename Append<List<>, First>::type;
    using List2  =  typename Remove<List<Rest...>, Index - 1>::type;
public:
    using type   =  typename Concat<List1, List2>::type;
};

template<template<int...> class List, int First, int...Rest>
struct Remove<List<First,Rest...>, 0>
{
    using type = List<Rest...>;
};

template<bool Cond, typename TrueResult, typename FalseResult> 
struct If_Then
{
    using type = FalseResult; 
};

template<typename TrueResult, typename FalseResult>
struct If_Then<true, TrueResult, FalseResult>
{
    using type = TrueResult;
};

template<template<int...> class List, int Value, int First, int... Rest>  
struct RemoveValue<List<First, Rest...>, Value>
{
    
    template<typename _List1, typename _List2, int _Value> struct remove_helper;  
    
    template<typename _List1, template<int...> class _List2, int _Value, int _First, int... _Rest>
    struct remove_helper<_List1, _List2<_First, _Rest...>, _Value>
    {
        using TList = typename Concat<_List1, typename If_Then<_First==_Value, List<>, List<_First>>::type>::type; 
        using type  = typename remove_helper<TList, _List2<_Rest...>, _Value>::type;
    };
    
    template<typename _List1, template<int...> class _List2, int _Value, int _First>
    struct remove_helper<_List1, _List2<_First>, _Value>
    {
        using TList = typename Concat<_List1, typename If_Then<_First==_Value, List<>, List<_First>>::type>::type; 
        using type  = TList;
    };
    
    using type = typename remove_helper<List<>, List<First, Rest...>, Value>::type;
};



int main()
{
    std::cout << Max<List<1,2,44,4,6>>::value << std::endl;
    std::cout << Max<Append<List<1,2,44,4,6>, 55>::type>::value << std::endl;
    std::cout << "type_list is same: " << std::is_same_v<Append<List<1,2,44,4,6>, 55>::type, List<1,2,44,4,6, 55>> << std::endl;
    std::cout << "type_list index 2 element: "  << FindIndexElement<List<11, 22, 33, 44, 55, 66, 77, 88>, 2>::value << std::endl;
    std::cout << "type_list concat is same: " << std::is_same_v<Concat<List<1, 2, 3>, List<4, 5, 6>>::type, List<1, 2, 3, 4, 5, 6>> << std::endl;
    
    std::cout << "type_list modify is same: " << std::is_same_v<Modify<List<1, 2, 3, 4, 5, 6>, 2, 10>::type, List<1, 2, 10, 4, 5, 6>> << std::endl;
    std::cout << "type_list remove 4 index is same: " << std::is_same_v<Remove<List<1, 2, 3, 4, 5, 6>, 4>::type, List<1, 2, 3, 4, 6>> << std::endl; 
    
    std::cout << "type_list remove value 3 is same: " << std::is_same_v<RemoveValue<List<1, 2, 3, 4, 3, 3>, 3>::type, List<1, 2, 4>> << std::endl; 
    return 0;
}

