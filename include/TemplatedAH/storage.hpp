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
	}
}

#endif