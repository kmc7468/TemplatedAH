#ifndef TEMPLATEDAH_HEADER_STRING_HPP
#define TEMPLATEDAH_HEADER_STRING_HPP

#include <cstddef>
#include <type_traits>

namespace tah
{
	namespace details
	{
		template<std::size_t Index_, typename First_, typename... Data_>
		struct get_type_by_index
		{
			using type = typename get_type_by_index<Index_ - 1, Data_...>::type;
		};
		template<typename First_, typename... Data_>
		struct get_type_by_index<0, First_, Data_...>
		{
			using type = First_;
		};

		template<char32_t... String_>
		struct line
		{
			static constexpr char32_t value[] = { String_... };
			static constexpr std::size_t length = sizeof(value) / sizeof(char32_t);
		};
		template<typename... Lines_>
		struct lines
		{
			template<int Index_>
			using get = typename get_type_by_index<Index_, Lines_...>::type;
			static constexpr std::size_t line = sizeof...(Lines_);
		};

		template<char32_t... String_>
		using raw_string = line<String_...>;

		template<typename String_, std::size_t Index_, std::size_t Length_, char32_t... String2_>
		struct make_raw_string_internal
		{
			using type = typename make_raw_string_internal<String_, Index_ + 1, Length_, String2_..., String_::str[Index_]>::type;
		};
		template<typename String_, std::size_t Length_, char32_t... String2_>
		struct make_raw_string_internal<String_, Length_, Length_, String2_...>
		{
			using type = raw_string<String2_...>;
		};

		template<typename String_>
		struct make_raw_string
		{
			using type = typename make_raw_string_internal<String_, 0, sizeof(String_::str) / sizeof(char32_t) - 1>::type;
		};

		template<typename String_, bool NewLine_, std::size_t Index_, std::size_t Length_, char32_t Character_, char32_t... String2_>
		struct split_raw_string_internal
		{
			using type = typename split_raw_string_internal<String_, NewLine_, Index_ + 1, Length_, String_::value[Index_], String2_..., Character_>::type;
		};
		template<typename String_, bool NewLine_, std::size_t Length_, char32_t Character_, char32_t... String2_>
		struct split_raw_string_internal<String_, NewLine_, 0, Length_, Character_, String2_...>
		{
			using type = typename split_raw_string_internal<String_, NewLine_, 1, Length_, String_::value[0], String2_...>::type;
		};
		template<typename String_, bool NewLine_, std::size_t Index_, std::size_t Length_, char32_t... String2_>
		struct split_raw_string_internal<String_, NewLine_, Index_, Length_, U'\n', String2_...>
		{
			using type = lines<line<String2_...>, typename split_raw_string_internal<String_, true, Index_ + 1, Length_, String_::value[Index_]>::type>;
		};
		template<typename String_, std::size_t Length_, char32_t Character_, char32_t... String2_>
		struct split_raw_string_internal<String_, true, Length_, Length_, Character_, String2_...>
		{
			using type = line<String2_..., Character_>;
		};
		template<typename String_, std::size_t Length_, char32_t Character_, char32_t... String2_>
		struct split_raw_string_internal<String_, false, Length_, Length_, Character_, String2_...>
		{
			using type = lines<line<String2_..., Character_>>;
		};

		template<typename String_>
		struct split_raw_string
		{
			using type = typename split_raw_string_internal<String_, false, 0, String_::length, 0>::type;
		};
	}
}

#endif