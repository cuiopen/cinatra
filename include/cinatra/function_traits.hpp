#pragma once
#include <functional>
#include <tuple>
//��ͨ����
//����ָ��
//function/lambda
//��Ա����
//��������

//ת��Ϊstd::function�ͺ���ָ�� 
template<typename T>
struct function_traits;

//��ͨ����
template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
public:
	enum { arity = sizeof...(Args) };
	typedef Ret function_type(Args...);
	typedef Ret return_type;
	using stl_function_type = std::function<function_type>;
	typedef Ret(*pointer)(Args...);

	template<size_t I>
	struct args
	{
		static_assert(I < arity, "index is out of range, index must less than sizeof Args");
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};
};

//����ָ��
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)>{};

//std::function
template <typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)>{};

//member function
#define FUNCTION_TRAITS(...) \
	template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{}; \

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

//��������
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())>{};

template <typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function& lambda)
{
	return static_cast<function_traits<Function>::stl_function_type>(lambda);
}

template <typename Function>
typename function_traits<Function>::stl_function_type to_function(Function&& lambda)
{
	return static_cast<function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

template <typename Function>
typename function_traits<Function>::pointer to_function_pointer(const Function& lambda)
{
	return static_cast<typename function_traits<Function>::pointer>(lambda);
}