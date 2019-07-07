#include <type_traits>

template <typename T, T... Xs> struct list { static constexpr int size = sizeof...(Xs); };

template<typename T, T Elem, typename...> struct pairing{};

template<typename TList, typename Value> struct append;
template<typename T, T... Xs, T Ch>
struct append<list<T, Xs...>, pairing<T, Ch>>
{
    using type = list<T, Xs..., Ch>;
};

template<typename TList> struct max;

template<typename T, T First, T Second, T... Xs>
struct max<list<T, First, Second, Xs...>>
{
    static constexpr T value = max<list<T, (First >= Second ? First : Second), Xs...>>::value;
};

template<typename T, T Last>
struct max<list<T, Last>>
{
    static constexpr T value = Last;
};

template<typename TList>  struct bubble_sort;

template<typename T, T... Xs>
struct bubble_sort<list<T, Xs...>>
{

    template<int N, typename TList1, typename TList2> struct bubble_sort_helper_2;

    template<int N, typename TList1, typename _T, _T _First, _T _Second, _T... _Xs>
    struct bubble_sort_helper_2<N, TList1, list<_T, _First, _Second, _Xs...>>
    {
        using type = typename bubble_sort_helper_2<N - 1,
                                                  typename append<TList1, pairing<_T, (_First <= _Second ? _First : _Second)>>::type,
                                                  list<_T, (_First <= _Second ? _Second : _First), _Xs...>>::type;
    };

    template<typename TList1, typename _T, _T _Last>
    struct bubble_sort_helper_2<0, TList1, list<_T, _Last>>
    {
        using type = TList1;
    };


    template<int N, typename TList1, typename TList2> struct bubble_sort_helper;

    template<int N, typename TList, typename _T, _T... _Xs>
    struct bubble_sort_helper<N, TList, list<_T, _Xs...>>
    {
        using List1 = typename append<TList, pairing<_T, max<list<_T, _Xs...>>::value>>::type;
        using List2 = typename bubble_sort_helper_2<N, list<T>, list<_T, _Xs...>>::type;

        using type = typename bubble_sort_helper<(List2::size - 1), List1, List2>::type;
    };

    template<typename TList, typename _T, _T _Last >
    struct bubble_sort_helper<0, TList, list<_T, _Last>>
    {
        using type =  typename append<TList, pairing<_T, _Last>>::type;
    };

    using type = typename bubble_sort_helper<(list<T, Xs...>::size - 1), list<T>, list<T, Xs...>>::type;
};


int main()
{
    using list1 = list<int, 1, 3, 5, 2, 4>;
    using list2 = list<int, 5, 4, 3, 2, 1>;
    using bubble_sort_t = typename bubble_sort<list1>::type;  //冒泡排序，从大到小
    static_assert(std::is_same<bubble_sort_t, list2>::value, " type is not same");

    return 0;
}
