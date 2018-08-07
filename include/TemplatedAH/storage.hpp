#ifndef TEMPLATEDAH_HEADER_STORAGE_HPP
#define TEMPLATEDAH_HEADER_STORAGE_HPP

#include "string.hpp"

#include <cstddef>

namespace tah
{
	namespace details
	{
		using int_type = long long;

		template<std::size_t Index_, int_type First_, int_type... Data_>
		struct get_value_by_index
		{
			static constexpr int_type value = get_value_by_index<Index_ - 1, Data_...>::value;
		};
		template<int_type First_, int_type... Data_>
		struct get_value_by_index<0, First_, Data_...>
		{
			static constexpr int_type value = First_;
		};

		template<bool IsEmpty_, int_type Front_, int_type... Data_>
		struct stack
		{
			static constexpr int_type front = Front_;
			static constexpr bool empty = false;
			template<std::size_t Size_>
			static constexpr bool assert_elements = sizeof...(Data_) + 1 >= Size_;

			using pop_type = stack<false, Data_...>;
			template<int_type Value_>
			using push_type = stack<false, Value_, Front_, Data_...>;
		};
		template<int_type Front_>
		struct stack<false, Front_>
		{
			static constexpr int_type front = Front_;
			static constexpr bool empty = false;
			template<std::size_t Size_>
			static constexpr bool assert_elements = Size_ <= 1;

			using pop_type = stack<true, -1>;
			template<int_type Value_>
			using push_type = stack<false, Value_, Front_>;
		};
		template<int_type Front_, int_type... Data_>
		struct stack<true, Front_, Data_...>
		{
			static constexpr bool empty = true;
			template<std::size_t Size_>
			static constexpr bool assert_elements = Size_ == 0;

			template<int_type Value_>
			using push_type = stack<false, Value_>;
		};

		using empty_stack = stack<true, -1>;

		template<bool IsEmpty_, int_type Front_, int_type... Data_>
		struct queue
		{
			static constexpr int_type front = Front_;
			static constexpr bool empty = false;
			template<std::size_t Size_>
			static constexpr bool assert_elements = sizeof...(Data_) + 1 >= Size_;

			using dequeue_type = queue<false, Data_...>;
			template<int_type Value_>
			using enqueue_type = queue<false, Front_, Data_..., Value_>;
		};
		template<int_type Front_>
		struct queue<false, Front_>
		{
			static constexpr int_type front = Front_;
			static constexpr bool empty = false;
			template<std::size_t Size_>
			static constexpr bool assert_elements = Size_ <= 1;

			using dequeue_type = queue<true, -1>;
			template<int_type Value_>
			using enqueue_type = queue<false, Front_, Value_>;
		};
		template<int_type Front_, int_type... Data_>
		struct queue<true, Front_, Data_...>
		{
			static constexpr bool empty = true;
			template<std::size_t Size_>
			static constexpr bool assert_elements = Size_ == 0;

			template<int_type Value_>
			using enqueue_type = queue<false, Value_>;
		};

		using empty_queue = queue<true, -1>;

		template<std::size_t Index_, typename NewStorage_, typename... Storages_>
		struct set_storage_by_index;

		template<typename... Storages_>
		struct storages
		{
			template<char32_t Jongsung_>
			using get = typename get_type_by_index<get_jongsung_index<Jongsung_>::value, Storages_...>::type;
			template<char32_t Jongsung_, typename NewStorage_>
			using set_type = typename set_storage_by_index<get_jongsung_index<Jongsung_>::value, NewStorage_, Storages_...>::type;
		};

		using create_storages = storages<
			empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack,
			empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack,
			empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack,
			empty_queue, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack, empty_stack
		>;

		template<std::size_t Index_, typename NewStorage_, typename... Storages_>
		struct set_storage_by_index
		{};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<0, NewStorage_, Storages_...>
		{
			using type = storages<
				NewStorage_, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<1, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, NewStorage_,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<2, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				NewStorage_, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<3, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, NewStorage_,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<4, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				NewStorage_, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<5, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, NewStorage_,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<6, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				NewStorage_, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<7, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, NewStorage_,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<8, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				NewStorage_, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<9, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, NewStorage_,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<10, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				NewStorage_, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<11, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, NewStorage_,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<12, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				NewStorage_, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<13, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, NewStorage_,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<14, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				NewStorage_, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<15, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, NewStorage_,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<16, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				NewStorage_, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<17, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, NewStorage_,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<18, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				NewStorage_, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<19, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, NewStorage_,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<20, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				NewStorage_, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<21, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, NewStorage_,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<22, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				NewStorage_, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<23, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, NewStorage_,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<24, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				NewStorage_, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<25, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, NewStorage_,
				typename get_type_by_index<26, Storages_...>::type, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<26, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				NewStorage_, typename get_type_by_index<27, Storages_...>::type
			>;
		};
		template<typename NewStorage_, typename... Storages_>
		struct set_storage_by_index<27, NewStorage_, Storages_...>
		{
			using type = storages<
				typename get_type_by_index<0, Storages_...>::type, typename get_type_by_index<1, Storages_...>::type,
				typename get_type_by_index<2, Storages_...>::type, typename get_type_by_index<3, Storages_...>::type,
				typename get_type_by_index<4, Storages_...>::type, typename get_type_by_index<5, Storages_...>::type,
				typename get_type_by_index<6, Storages_...>::type, typename get_type_by_index<7, Storages_...>::type,
				typename get_type_by_index<8, Storages_...>::type, typename get_type_by_index<9, Storages_...>::type,
				typename get_type_by_index<10, Storages_...>::type, typename get_type_by_index<11, Storages_...>::type,
				typename get_type_by_index<12, Storages_...>::type, typename get_type_by_index<13, Storages_...>::type,
				typename get_type_by_index<14, Storages_...>::type, typename get_type_by_index<15, Storages_...>::type,
				typename get_type_by_index<16, Storages_...>::type, typename get_type_by_index<17, Storages_...>::type,
				typename get_type_by_index<18, Storages_...>::type, typename get_type_by_index<19, Storages_...>::type,
				typename get_type_by_index<20, Storages_...>::type, typename get_type_by_index<21, Storages_...>::type,
				typename get_type_by_index<22, Storages_...>::type, typename get_type_by_index<23, Storages_...>::type,
				typename get_type_by_index<24, Storages_...>::type, typename get_type_by_index<25, Storages_...>::type,
				typename get_type_by_index<26, Storages_...>::type, NewStorage_
			>;
		};
	}
}

#endif