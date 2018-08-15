#ifndef TEMPLATEDAH_HEADER_STRING_HPP
#define TEMPLATEDAH_HEADER_STRING_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace tah
{
#define TEMPLATEDAH_STRING(name, string)			\
struct name											\
{													\
	static constexpr const char32_t str[] = string;	\
	static constexpr std::size_t str_length =		\
		sizeof(str) / sizeof(char32_t);				\
	using str_type = char32_t;						\
};
#define TEMPLATEDAH_STRING8(name, string)			\
struct name											\
{													\
	static constexpr const char str[] = string;		\
	static constexpr std::size_t str_length =		\
		sizeof(str);								\
	using str_type = char;							\
};
#define TEMPLATEDAH_STRING16(name, string)			\
struct name											\
{													\
	static constexpr const char16_t str[] = string;	\
	static constexpr std::size_t str_length =		\
		sizeof(str) / sizeof(char16_t);				\
	using str_type = char16_t;						\
};
#define TEMPLATEDAH_STRING32(name, string)			\
TEMPLATEDAH_STRING(name, string)

	namespace details
	{
		using int_type = long long;

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

		template<typename Ty_, Ty_... String_>
		struct basic_raw_string
		{
			static constexpr Ty_ value[] = { String_... };
			static constexpr std::size_t length = sizeof...(String_);

			using char_type = Ty_;
		};
		template<typename Ty_>
		struct basic_raw_string<Ty_>
		{
			static constexpr std::size_t length = 0;

			using char_type = Ty_;
		};

		template<typename Ty_, Ty_... String_>
		constexpr Ty_ basic_raw_string<Ty_, String_...>::value[];

		template<char... String_>
		using raw_string_utf8 = basic_raw_string<char, String_...>;
		template<char16_t... String_>
		using raw_string_utf16 = basic_raw_string<char16_t, String_...>;
		template<char32_t... String_>
		using raw_string_utf32 = basic_raw_string<char32_t, String_...>;

		template<char32_t... String_>
		using line = raw_string_utf32<String_...>;
		template<typename... Lines_>
		struct lines
		{
			template<std::size_t Index_ >
			using get = typename get_type_by_index<Index_, Lines_...>::type;
			static constexpr std::size_t line = sizeof...(Lines_);
		};

		template<char32_t... String_>
		using raw_string = line<String_...>;

		template<typename String_, std::size_t Index_, std::size_t Length_, typename String_::str_type... String2_>
		struct make_raw_string_internal
		{
			using type = typename make_raw_string_internal<String_, Index_ + 1, Length_, String2_..., String_::str[Index_]>::type;
		};
		template<typename String_, std::size_t Length_, typename String_::str_type... String2_>
		struct make_raw_string_internal<String_, Length_, Length_, String2_...>
		{
			using type = basic_raw_string<typename String_::str_type, String2_...>;
		};

		template<typename String_>
		struct make_raw_string
		{
			using type = typename make_raw_string_internal<String_, 0, String_::str_length - 1>::type;
		};

		template<typename Ty_, std::size_t N_, const Ty_(&String_)[N_], typename>
		struct add_raw_string_internal;
		
		template<typename Ty_, std::size_t N_, const Ty_(&String_)[N_], std::size_t... Index_>
		struct add_raw_string_internal<Ty_, N_, String_, std::index_sequence<Index_...>>
		{
			template<std::size_t M_, const Ty_(&String2_)[M_], typename>
			struct add_raw_string_internal_internal;

			template<std::size_t M_, const Ty_(&String2_)[M_], std::size_t... Index2_>
			struct add_raw_string_internal_internal<M_, String2_, std::index_sequence<Index2_...>>
			{
				using type = basic_raw_string<Ty_, String_[Index_]..., String2_[Index2_]...>;
			};
		};

		template<typename String_, typename String2_>
		struct add_raw_string
		{
			using type = typename add_raw_string_internal<typename String_::char_type, String_::length, String_::value, std::make_index_sequence<String_::length>>::
				template add_raw_string_internal_internal<String2_::length, String2_::value, std::make_index_sequence<String2_::length>>::type;
		};
		template<typename String2_>
		struct add_raw_string<void, String2_>
		{
			using type = String2_;
		};
		template<typename String_>
		struct add_raw_string<String_, void>
		{
			using type = String_;
		};
		template<>
		struct add_raw_string<void, void>
		{
			using type = void;
		};

		template<std::size_t N_, const char32_t(&String_)[N_], typename>
		struct remove_raw_string_prefix_internal;

		template<std::size_t N_, const char32_t(&String_)[N_], std::size_t... Index_>
		struct remove_raw_string_prefix_internal<N_, String_, std::index_sequence<Index_...>>
		{
			using type = raw_string<String_[Index_ + 1]...>;
		};

		template<typename String_, std::size_t Length_, typename = void>
		struct remove_raw_string_prefix;

		template<typename String_, std::size_t Length_>
		struct remove_raw_string_prefix<String_, Length_,
			typename std::enable_if<String_::length != 1>::type>
		{
			using type = typename remove_raw_string_prefix_internal<String_::length, String_::value,
				std::make_index_sequence<String_::length - Length_>>::type;
		};
		template<typename String_, std::size_t Length_>
		struct remove_raw_string_prefix<String_, Length_,
			typename std::enable_if<String_::length == 1>::type>
		{
			using type = void;
		};
		template<std::size_t Length_>
		struct remove_raw_string_prefix<void, Length_>
		{
			using type = void;
		};

		template<std::size_t N_, const char32_t(&String_)[N_], typename>
		struct remove_raw_string_postfix_internal;

		template<std::size_t N_, const char32_t(&String_)[N_], std::size_t... Index_>
		struct remove_raw_string_postfix_internal<N_, String_, std::index_sequence<Index_...>>
		{
			using type = raw_string<String_[Index_]...>;
		};

		template<typename String_, std::size_t Length_, typename = void>
		struct remove_raw_string_postfix;

		template<typename String_, std::size_t Length_>
		struct remove_raw_string_postfix<String_, Length_,
			typename std::enable_if<String_::length != 1>::type>
		{
			using type = typename remove_raw_string_postfix_internal<String_::length, String_::value,
				std::make_index_sequence<String_::length - Length_>>::type;
		};
		template<typename String_, std::size_t Length_>
		struct remove_raw_string_postfix<String_, Length_,
			typename std::enable_if<String_::length == 1>::type>
		{
			using type = void;
		};
		template<std::size_t Length_>
		struct remove_raw_string_postfix<void, Length_>
		{
			using type = void;
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

		/*
		 * flatten_* 계열 템플릿 구조체들은
		 * https://stackoverflow.com/questions/20253473/variadic-template-aliases-as-template-arguments
		 * 에서 가져왔습니다.
		 */

		template<typename Ty_, typename Ty2_>
		struct flatten_lines_internal;
		template<typename... Arg_, typename... HeadArg_, typename... Tail_>
		struct flatten_lines_internal<lines<Arg_...>, lines<lines<HeadArg_...>, Tail_...>>
		{
			using type = typename flatten_lines_internal<
				lines<Arg_...>, lines<HeadArg_..., Tail_...>>::type;
		};
		template<typename... Arg_, typename Head_, typename... Tail_>
		struct flatten_lines_internal<lines<Arg_...>, lines<Head_, Tail_...>>
		{
			using type = typename flatten_lines_internal<lines<Arg_..., Head_>, lines<Tail_...>>::type;
		};
		template<typename... Arg_>
		struct flatten_lines_internal<lines<Arg_...>, lines<>>
		{
			using type = lines<Arg_...>;
		};

		template<typename Lines_>
		struct flatten_lines;
		template<typename... Arg_>
		struct flatten_lines<lines<Arg_...>>
		{
			using type = typename flatten_lines_internal<lines<>, lines<Arg_...>>::type;
		};

		template<typename String_>
		struct split_raw_string
		{
			using type = typename flatten_lines<
				typename split_raw_string_internal<String_, false, 0, String_::length, 0>::type
			>::type;
		};

		struct hangul
		{
			static constexpr char32_t chosungs[] =
			{
				U'ㄱ', U'ㄲ', U'ㄴ', U'ㄷ', U'ㄸ', U'ㄹ', U'ㅁ', U'ㅂ', U'ㅃ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ',
				U'ㅉ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
			};
			static constexpr char32_t jungsungs[] =
			{
				U'ㅏ', U'ㅐ', U'ㅑ', U'ㅒ', U'ㅓ', U'ㅔ', U'ㅕ', U'ㅖ', U'ㅗ', U'ㅘ', U'ㅙ', U'ㅚ', U'ㅛ',
				U'ㅜ', U'ㅝ', U'ㅞ', U'ㅟ', U'ㅠ', U'ㅡ', U'ㅢ', U'ㅣ'
			};
			static constexpr char32_t jongsungs[] =
			{
				0, U'ㄱ', U'ㄲ', U'ㄳ', U'ㄴ', U'ㄵ', U'ㄶ', U'ㄷ', U'ㄹ', U'ㄺ', U'ㄻ', U'ㄼ', U'ㄽ', U'ㄾ',
				U'ㄿ', U'ㅀ', U'ㅁ', U'ㅂ', U'ㅄ', U'ㅅ', U'ㅆ', U'ㅇ', U'ㅈ', U'ㅊ', U'ㅋ', U'ㅌ', U'ㅍ', U'ㅎ'
			};
			template<char32_t Character_>
			static constexpr bool is_hangul = Character_ >= 0xAC00 && Character_ <= 0xD7A3;
			
			static constexpr int_type numbers[] =
			{
				0, 2, 4, 4, 2, 5, 5, 3, 5, 7, 9, 9, 7, 9,
				9, 8, 4, 4, 6, 2, 4, -1, 3, 4, 3, 4, 4, -1
			};
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

		template<char32_t Character_>
		struct is_space
		{
			static constexpr bool value =
				Character_ == U' ' || Character_ == U'\f' || Character_ == U'\n' ||
				Character_ == U'\r' || Character_ == U'\t' || Character_ == U'\v';
		};
		template<char32_t Digit_>
		struct is_digit
		{
			static constexpr bool value = Digit_ >= U'0' && Digit_ <= U'9';
		};

		template<int_type Digit_>
		struct digit_to_character
		{
			static constexpr char32_t value = U'0' + Digit_;
		};
		template<char32_t Character_>
		struct make_raw_string_character
		{
			using type = raw_string<Character_>;
		};

		template<int_type Number_, typename String_ = void, typename = void>
		struct to_string_number;

		template<int_type Number_, typename String_>
		struct to_string_number<Number_, String_, typename std::enable_if<Number_ <= 9 && Number_ >= 0>::type>
		{
			using type =
				typename add_raw_string<
					typename make_raw_string_character<digit_to_character<Number_>::value>::type,
					String_>::type;
		};
		template<int_type Number_, typename String_>
		struct to_string_number<Number_, String_, typename std::enable_if<(Number_ > 9)>::type>
		{
		private:
			static constexpr int_type digit_ = Number_ - ((Number_ / 10) * 10);
			using digit_type_ = typename make_raw_string_character<digit_to_character<digit_>::value>::type;

		public:
			using type = typename add_raw_string<
					typename add_raw_string<
						typename to_string_number<Number_ / 10>::type,
						digit_type_>::type,
				String_>::type;
		};
		template<int_type Number_, typename String_>
		struct to_string_number<Number_, String_, typename std::enable_if<(Number_ < 0)>::type>
		{
		public:
			using type = typename add_raw_string<raw_string<U'-'>,
				typename to_string_number<-Number_, void>::type>::type;
		};

		template<char32_t Digit_>
		struct character_to_digit
		{
			static constexpr int_type value = static_cast<int_type>(Digit_ - U'0');
		};
		template<int_type Base_, int_type Exp_>
		struct pow
		{
			static constexpr int_type value = Base_ * pow<Base_, Exp_ - 1>::value;
		};
		template<int_type Base_>
		struct pow<Base_, 0>
		{
			static constexpr int_type value = 1;
		};

		template<typename String_>
		struct get_raw_string_length
		{
			static constexpr std::size_t value = String_::length;
		};
		template<>
		struct get_raw_string_length<void>
		{
			static constexpr std::size_t value = 0;
		};

		template<typename Input_, typename String_, typename = void>
		struct read_digits;

		template<typename Input_, typename String_>
		struct read_digits<Input_, String_, typename std::enable_if<
				get_raw_string_length<Input_>::value != 0 &&
				is_digit<Input_::value[0]>::value
			>::type>
		{
		private:
			using internal_input_type_ = typename remove_raw_string_prefix<Input_, 1>::type;
			using next_ = read_digits<internal_input_type_, raw_string<Input_::value[0]>>;

		public:
			using input_type = typename next_::input_type;
			using string_type = typename add_raw_string<String_, typename next_::string_type>::type;
		};
		template<typename Input_, typename String_>
		struct read_digits<Input_, String_, typename std::enable_if<
				get_raw_string_length<Input_>::value != 0 &&
				!is_digit<Input_::value[0]>::value &&
				!is_space<Input_::value[0]>::value
			>::type>
		{
			using input_type = Input_;
			using string_type = String_;
		};
		template<typename Input_, typename String_>
		struct read_digits<Input_, String_, typename std::enable_if<
				get_raw_string_length<Input_>::value != 0 &&
				!is_digit<Input_::value[0]>::value &&
				is_space<Input_::value[0]>::value
			>::type>
		{
			using input_type = typename remove_raw_string_prefix<Input_, 1>::type;
			using string_type = String_;
		};
		template<typename Input_, typename String_>
		struct read_digits<Input_, String_, typename std::enable_if<
				get_raw_string_length<Input_>::value == 0
			>::type>
		{
			using input_type = void;
			using string_type = String_;
		};

		template<typename String_, std::size_t Index_>
		struct to_number_from_digits
		{
			static constexpr int_type value =
				character_to_digit<String_::value[String_::length - 1]>::value *
				pow<10, Index_>::value +
				to_number_from_digits<typename remove_raw_string_postfix<String_, 1>::type,
									  Index_ + 1>::value;
		};
		template<std::size_t Index_>
		struct to_number_from_digits<void, Index_>
		{
			static constexpr int_type value = 0;
		};

		template<typename Input_, typename = void>
		struct to_number;

		template<typename Input_>
		struct to_number<Input_, typename std::enable_if<is_digit<Input_::value[0]>::value>::type>
		{
		private:
			using read_digits_ = read_digits<Input_, void>;

		public:
			static constexpr int_type value = to_number_from_digits<typename read_digits_::string_type, 0>::value;

			using input_type = typename read_digits_::input_type;
		};
		template<typename Input_>
		struct to_number<Input_, typename std::enable_if<Input_::value[0] == U'-'>::type>
		{
		private:
			using read_digits_ = read_digits<
				typename remove_raw_string_prefix<Input_, 1>::type, void>;

		public:
			static constexpr int_type value = -to_number_from_digits<typename read_digits_::string_type, 0>::value;

			using input_type = typename read_digits_::input_type;
		};
		template<typename Input_>
		struct to_number<Input_, typename std::enable_if<
			!is_digit<Input_::value[0]>::value && Input_::value[0] != U'-'>::type>
		{
#ifdef TEMPLATEDAH_INVFMT_PASS
			static constexpr int_type value = -1;
#endif

			using input_type = Input_;
		};
		template<>
		struct to_number<void>
		{
#ifndef TEMPLATEDAH_EOF_ERROR
			static constexpr int_type value = -1;
#endif

			using input_type = void;
		};

		template<typename Input_>
		struct to_character
		{
			static constexpr int_type value = static_cast<int_type>(Input_::value[0]);

			using input_type = typename remove_raw_string_prefix<Input_, 1>::type;
		};
		template<>
		struct to_character<void>
		{
#ifndef TEMPLATEDAH_EOF_ERROR
			static constexpr int_type value = -1;
#endif

			using input_type = void;
		};
	}

	using details::int_type;
	template<typename String_>
	using get_string_length = details::get_raw_string_length<String_>;
}

#include "encoding.hpp"
#endif
