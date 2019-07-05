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
    using TestList1 = List<1, 2, 3, 4, 5, 6>;
    //Max最大值
    std::cout << "TestList1 max value is: " <<  Max<TestList1>::value << std::endl;  //output 6.
    //尾部添加一个元素后，并输出最大值
    using TestList2 = Append<TestList1, 7>::type;
    std::cout << "max value is: " << Max<TestList2>::value << std::endl;  //output 7.
    //判断添加后的类型是否一致
    std::cout << "TestList2 is same: " << std::is_same_v<TestList2, List<1, 2, 3, 4, 5, 6, 7>> << std::endl;
    //查找TestList2的第2个元素
    std::cout << "TestList2 index 2 element is: "  << FindIndexElement<TestList2, 2>::value << std::endl; //output 3
    //连接两个列表成一个列表
    using List1 = List<1, 2, 3>;
    using List2 = List<4, 5, 6>;
    std::cout << "concat is same: " << std::is_same_v<Concat<List1, List2>::type, List<1, 2, 3, 4, 5, 6>> << std::endl;
    //根据下标改变列表的值
    std::cout << "TestList2 modify index 2 value to 10 is same: " << std::is_same_v<Modify<List<1, 2, 3, 4, 5, 6>, 2, 10>::type, List<1, 2, 10, 4, 5, 6>> << std::endl;
    //根据下标删除元素
    using Remove_Index_4_List = Remove<List<1, 2, 3, 4, 5, 6>, 4>::type;  //删除了数字5.
    std::cout << "type_list remove index[4] is same: " << std::is_same_v<Remove_Index_4_List, List<1, 2, 3, 4, 6>> << std::endl; 
    //根据值删除列表下搜索符合的值
    using Remove_Value_3_List= RemoveValue<List<1, 2, 3, 4, 3, 3>, 3>::type;
    std::cout << "type_list remove value 3 is same: " << std::is_same_v<Remove_Value_3_List, List<1, 2, 4>> << std::endl; 
    return 0;
}





