#ifndef TEMPLATEDAH_HEADER_ENCODING_HPP
#define TEMPLATEDAH_HEADER_ENCODING_HPP

#include <cstddef>
#include <type_traits>

namespace tah
{
	namespace details
	{
		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf8_to_utf32;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf8_to_utf32<String_, Index_, Output_,
			typename std::enable_if<static_cast<unsigned char>(String_::value[Index_]) < 0x80>::type>
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
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
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
			typename std::enable_if<Index_ >= String_::length>::type>
		{
			using type = Output_;
		};

		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf32_to_utf8;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf8<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] < 0x80 &&
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
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
			typename std::enable_if<Index_ >= String_::length>::type>
		{
			using type = Output_;
		};

		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf32_to_utf16;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf32_to_utf16<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] <= 0xFFFF &&
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
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
			typename std::enable_if<Index_ >= String_::length>::type>
		{
			using type = Output_;
		};

		template<typename String_, std::size_t Index_ = 0, typename Output_ = void, typename = void>
		struct utf16_to_utf32;

		template<typename String_, std::size_t Index_, typename Output_>
		struct utf16_to_utf32<String_, Index_, Output_,
			typename std::enable_if<String_::value[Index_] >= 0xD800 && String_::value[Index_] <= 0xDBFF &&
									Index_ < String_::length>::type>
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
									Index_ < String_::length>::type>
		{
			using type = typename add_raw_string<Output_,
				typename add_raw_string<raw_string_utf32<String_::value[Index_]>,
				typename utf16_to_utf32<String_, Index_ + 1, void>::type
				>::type
			>::type;
		};
		template<typename String_, std::size_t Index_, typename Output_>
		struct utf16_to_utf32<String_, Index_, Output_,
			typename std::enable_if<Index_ >= String_::length>::type>
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
}

#endif