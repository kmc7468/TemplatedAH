#ifndef TEMPLATEDAH_HEADER_STRING_HPP
#define TEMPLATEDAH_HEADER_STRING_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

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

		template<std::size_t N_, const char32_t(&String_)[N_], typename>
		struct add_raw_string_internal;
		
		template<std::size_t N_, const char32_t(&String_)[N_], std::size_t... Index_>
		struct add_raw_string_internal<N_, String_, std::index_sequence<Index_...>>
		{
			template<std::size_t M_, const char32_t(&String2_)[M_], typename>
			struct add_raw_string_internal_internal;

			template<std::size_t M_, const char32_t(&String2_)[M_], std::size_t... Index2_>
			struct add_raw_string_internal_internal<M_, String2_, std::index_sequence<Index2_...>>
			{
				using type = raw_string<String_[Index_]..., String2_[Index2_]...>;
			};
		};

		template<typename String_, typename String2_>
		struct add_raw_string
		{
			using type = typename add_raw_string_internal<String_::length, String_::value, std::make_index_sequence<String_::length>>::
				template add_raw_string_internal_internal<String2_::length, String2_::value, std::make_index_sequence<String2_::length>>::type;
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

		struct hangul
		{
			static constexpr char32_t chosungs[] =
			{
				U'ぁ', U'あ', U'い', U'ぇ', U'え', U'ぉ', U'け', U'げ', U'こ', U'さ', U'ざ', U'し', U'じ',
				U'す', U'ず', U'せ', U'ぜ', U'そ', U'ぞ'
			};
			static constexpr char32_t jungsungs[] =
			{
				U'た', U'だ', U'ち', U'ぢ', U'っ', U'つ', U'づ', U'て', U'で', U'と', U'ど', U'な', U'に',
				U'ぬ', U'ね', U'の', U'は', U'ば', U'ぱ', U'ひ', U'び'
			};
			static constexpr char32_t jongsungs[] =
			{
				0, U'ぁ', U'あ', U'ぃ', U'い', U'ぅ', U'う', U'ぇ', U'ぉ', U'お', U'か', U'が', U'き', U'ぎ',
				U'く', U'ぐ', U'け', U'げ', U'ご', U'さ', U'ざ', U'し', U'じ', U'ず', U'せ', U'ぜ', U'そ', U'ぞ'
			};
			template<char32_t Character_>
			static constexpr bool is_hangul = Character_ >= 0xAC00 && Character_ <= 0xD7A3;
		};

		template<char32_t Hangul_, typename = void>
		struct divide_hangul;

		template<char32_t Hangul_>
		struct divide_hangul<Hangul_, typename std::enable_if<hangul::is_hangul<Hangul_>>::type>
		{
			static constexpr char32_t chosung =
				hangul::chosungs[((((Hangul_ - 0xAC00) - ((Hangul_ - 0xAC00) % 28)) / 28) - ((((Hangul_ - 0xAC00) - ((Hangul_ - 0xAC00) % 28)) / 28) % 21)) / 21];
			static constexpr char32_t jungsung =
				hangul::jungsungs[(((Hangul_ - 0xAC00) - ((Hangul_ - 0xAC00) % 28)) / 28) % 21];
			static constexpr char32_t jongsung =
				hangul::jongsungs[(Hangul_ - 0xAC00) % 28];
			static constexpr bool error = false;
		};
		template<char32_t Hangul_>
		struct divide_hangul<Hangul_, typename std::enable_if<!hangul::is_hangul<Hangul_>>::type>
		{
			static constexpr bool error = true;
		};

		template<char32_t Jongsung_, std::size_t Index_ = 0>
		struct get_jongsung_index
		{
			static constexpr std::size_t value =
				(hangul::jongsungs[Index_] == Jongsung_) ? Index_ : (get_jongsung_index<Jongsung_, Index_ + 1>::value);
		};
		template<char32_t Jongsung_>
		struct get_jongsung_index<Jongsung_, sizeof(hangul::jongsungs) / sizeof(char32_t)>
		{
			static constexpr std::size_t value = static_cast<std::size_t>(-1);
		};
	}
}

#endif