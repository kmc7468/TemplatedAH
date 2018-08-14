#ifndef TEMPLATEDAH_HEADER_ENCODING_HPP
#define TEMPLATEDAH_HEADER_ENCODING_HPP

#include <cstddef>
#include <type_traits>

#ifndef TEMPLATEDAH_NO_PRINT
#	include <cstdio>
#	include <cwchar>

#	if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#		include <fcntl.h>
#		include <io.h>
#	endif
#endif

namespace tah
{
	namespace details
	{
		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf8_to_utf32;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf8_to_utf32<String_, Index_, Output_,
			typename std::enable_if<static_cast<unsigned char>(String_::value[Index_]) < 0x80 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<raw_string_utf32<static_cast<unsigned char>(String_::value[Index_])>,
				typename utf8_to_utf32<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf8_to_utf32<String_, Index_, Output_,
			typename std::enable_if<(static_cast<unsigned char>(String_::value[Index_]) & 0xF0) == 0xF0 &&
									static_cast<unsigned char>(String_::value[Index_]) >= 0x80 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf32<
						((static_cast<unsigned char>(String_::value[Index_]) & 0x07) << 18) +
						((static_cast<unsigned char>(String_::value[Index_ + 1]) & 0x3F) << 12) +
						((static_cast<unsigned char>(String_::value[Index_ + 2]) & 0x3F) << 6) +
						(static_cast<unsigned char>(String_::value[Index_ + 3]) & 0x3F)
					>,
					typename utf8_to_utf32<String_, Index_ + 4, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf8_to_utf32<String_, Index_, Output_,
			typename std::enable_if<(static_cast<unsigned char>(String_::value[Index_]) & 0xE0) == 0xE0 &&
									!((static_cast<unsigned char>(String_::value[Index_]) & 0xF0) == 0xF0) &&
									static_cast<unsigned char>(String_::value[Index_]) >= 0x80 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf32<
						((static_cast<unsigned char>(String_::value[Index_]) & 0x0F) << 12) +
						((static_cast<unsigned char>(String_::value[Index_ + 1]) & 0x3F) << 6) +
						(static_cast<unsigned char>(String_::value[Index_ + 2]) & 0x3F)
					>,
				typename utf8_to_utf32<String_, Index_ + 3, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf8_to_utf32<String_, Index_, Output_,
			typename std::enable_if<(static_cast<unsigned char>(String_::value[Index_]) & 0xC0) == 0xC0 &&
									!((static_cast<unsigned char>(String_::value[Index_]) & 0xF0) == 0xF0) &&
									!((static_cast<unsigned char>(String_::value[Index_]) & 0xE0) == 0xE0) &&
									static_cast<unsigned char>(String_::value[Index_]) >= 0x80 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf32<
						((static_cast<unsigned char>(String_::value[Index_]) & 0x1F) << 6) +
						(static_cast<unsigned char>(String_::value[Index_ + 1]) & 0x3F)
					>,
				typename utf8_to_utf32<String_, Index_ + 2, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf8_to_utf32<String_, Index_, Output_,
			typename std::enable_if<Index_ >= get_string_length<String_>::value>::type>
		{
			using type = Output_;
		};

		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf32_to_utf8;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf8<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] < 0x80 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<raw_string_utf8<static_cast<char>(String_::value[Index_])>,
				typename utf32_to_utf8<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf8<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] >= 0x80 &&
									String_::value[Index_] < 0x800 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf8<
						static_cast<char>(0xC0 | (String_::value[Index_] >> 6)),
						static_cast<char>(0x80 | (String_::value[Index_] & 0x3F))
					>,
					typename utf32_to_utf8<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf8<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] >= 0x800  &&
									String_::value[Index_] < 0x10000 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf8<
						static_cast<char>(0xE0 | (String_::value[Index_] >> 12)),
						static_cast<char>(0x80 | ((String_::value[Index_] >> 6) & 0x3F)),
						static_cast<char>(0x80 | (String_::value[Index_] & 0x3F))
					>,
					typename utf32_to_utf8<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf8<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] >= 0x10000 &&
									String_::value[Index_] < 0x110000 &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf8<
						static_cast<char>(0xF0 | (String_::value[Index_] >> 18)),
						static_cast<char>(0x80 | ((String_::value[Index_] >> 12) & 0x3F)),
						static_cast<char>(0x80 | ((String_::value[Index_] >> 6) & 0x3F)),
						static_cast<char>(0x80 | (String_::value[Index_] & 0x3F))
					>,
					typename utf32_to_utf8<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf8<String_, Index_, Output_,
			typename std::enable_if<Index_ >= get_string_length<String_>::value>::type>
		{
			using type = Output_;
		};

		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf32_to_utf16;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf16<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] <= 0xFFFF &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<raw_string_utf16<static_cast<char16_t>(String_::value[Index_])>,
				typename utf32_to_utf16<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf16<String_, Index_, Output_,
			typename std::enable_if<(String_::value[Index_] > 0xFFFF) &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf16<
						static_cast<char16_t>(static_cast<char16_t>(String_::value[Index_] - 0x10000) / 0x400 + 0xD800),
						static_cast<char16_t>(static_cast<char16_t>(String_::value[Index_] - 0x10000) % 0x400 + 0xDC00)
					>,
					typename utf32_to_utf16<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf16<String_, Index_, Output_,
			typename std::enable_if<Index_ >= get_string_length<String_>::value>::type>
		{
			using type = Output_;
		};

		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf16_to_utf32;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf16_to_utf32<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] >= 0xD800 && String_::value[Index_] <= 0xDBFF &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<
					raw_string_utf32<
						(String_::value[Index_] - 0xD800) * 0x400 +
						(String_::value[Index_ + 1] - 0xDC00) + 0x10000
					>,
					typename utf16_to_utf32<String_, Index_ + 2, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf16_to_utf32<String_, Index_, Output_,
			typename std::enable_if<!(String_::value[Index_] >= 0xD800 && String_::value[Index_] <= 0xDBFF) &&
									Index_ < get_string_length<String_>::value>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<raw_string_utf32<String_::value[Index_]>,
				typename utf16_to_utf32<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf16_to_utf32<String_, Index_, Output_,
			typename std::enable_if<Index_ >= get_string_length<String_>::value>::type>
		{
			using type = Output_;
		};

		template<typename String_>
		struct utf8_to_utf16
		{
			using type = typename utf32_to_utf16<
				typename utf8_to_utf32<String_>::type>::type;
		};
		template<typename String_>
		struct utf16_to_utf8
		{
			using type = typename utf32_to_utf8<
				typename utf16_to_utf32<String_>::type>::type;
		};
	}

#ifndef TEMPLATEDAH_NO_PRINT
	template<typename String_>
	typename std::enable_if<
		std::is_same<void, String_>::value
	>::type print(std::FILE*)
	{}
	template<typename String_>
	typename std::enable_if<
		std::is_same<char32_t, typename String_::char_type>::value
	>::type print(std::FILE* stream)
	{
		using string_type = String_;

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		using utf16_string_type = typename details::utf32_to_utf16<string_type>::type;

		_setmode(_fileno(stream), _O_U16TEXT);

		for (std::size_t i = 0; i < utf16_string_type::length; ++i)
		{
			std::putwc(static_cast<wchar_t>(utf16_string_type::value[i]), stream);
		}

		_setmode(_fileno(stream), _O_TEXT);
#else
		using utf8_string_type = typename details::utf32_to_utf8<string_type>::type;

		for (std::size_t i = 0; i < utf8_string_type::length; ++i)
		{
			std::putc(utf8_string_type::value[i], stream);
		}
#endif
	}
	template<typename String_>
	typename std::enable_if<
		std::is_same<char16_t, typename String_::char_type>::value
	>::type print(std::FILE* stream)
	{
		using string_type = String_;

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		_setmode(_fileno(stream), _O_U16TEXT);

		for (std::size_t i = 0; i < string_type::length; ++i)
		{
			std::putwc(static_cast<wchar_t>(string_type::value[i]), stream);
		}

		_setmode(_fileno(stream), _O_TEXT);
#else
		using utf8_string_type = typename details::utf16_to_utf8<string_type>::type;

		for (std::size_t i = 0; i < utf8_string_type::length; ++i)
		{
			std::putc(utf8_string_type::value[i], stream);
		}
#endif
	}
	template<typename String_>
	typename std::enable_if<
		std::is_same<char, typename String_::char_type>::value
	>::type print(std::FILE* stream)
	{
		using string_type = String_;

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
		using utf16_string_type = typename details::utf8_to_utf16<string_type>::type;

		_setmode(_fileno(stream), _O_U16TEXT);

		for (std::size_t i = 0; i < utf16_string_type::length; ++i)
		{
			std::putwc(static_cast<wchar_t>(utf16_string_type::value[i]), stream);
		}

		_setmode(_fileno(stream), _O_TEXT);
#else
		for (std::size_t i = 0; i < string_type::length; ++i)
		{
			std::putc(utf8_string_type::value[i], stream);
		}
#endif
	}

	template<typename String_>
	void print()
	{
		print<String_>(stdout);
	}
	template<>
	void print<void>()
	{}

	template<typename String_>
	void print_org(std::FILE* stream)
	{
		std::fwrite(String_::value, sizeof(typename String_::char_type), String_::length, stream);
	}
	template<typename String_>
	typename std::enable_if<
		std::is_same<char32_t, typename String_::char_type>::value
	>::type print_org_with_bom(std::FILE* stream)
	{
		char32_t bom = 0xFEFF;
		std::fwrite(&bom, sizeof(char32_t), 1, stream);

		print_org<String_>(stream);
	}
	template<typename String_>
	typename std::enable_if<
		std::is_same<char16_t, typename String_::char_type>::value
	>::type print_org_with_bom(std::FILE* stream)
	{
		char16_t bom = 0xFEFF;
		std::fwrite(&bom, sizeof(char16_t), 1, stream);

		print_org<String_>(stream);
	}
	template<typename String_>
	typename std::enable_if<
		std::is_same<char, typename String_::char_type>::value
	>::type print_org_with_bom(std::FILE* stream)
	{
		char bom[] = { 0xEF, 0xBB, 0xBF };
		std::fwrite(bom, sizeof(char), sizeof(bom) / sizeof(char), stream);

		print_org<String_>(stream);
	}
#endif
}

#endif