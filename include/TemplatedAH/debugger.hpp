#ifndef TEMPLATEDAH_HEADER_DEBUGGER_HPP
#define TEMPLATEDAH_HEADER_DEBUGGER_HPP

#include "string.hpp"

#include <cstddef>

namespace tah
{
	namespace details
	{
		template<typename Storage_, typename = void>
		struct dump_storage;
		template<typename Storage_>
		struct dump_storage<Storage_, typename std::enable_if<!Storage_::empty &&
			!Storage_::pop_type::empty>::type>
		{
			using type =
				typename add_raw_string<
					typename to_string_number<Storage_::front>::type,
					typename add_raw_string<
						raw_string<U',', U' '>,
						typename dump_storage<typename Storage_::pop_type>::type
					>::type
				>::type;
		};
		template<typename Storage_>
		struct dump_storage<Storage_, typename std::enable_if<!Storage_::empty &&
			Storage_::pop_type::empty>::type>
		{
			using type = typename to_string_number<Storage_::front>::type;
		};
		template<typename Storage_>
		struct dump_storage<Storage_, typename std::enable_if<Storage_::empty>::type>
		{
			using type = void;
		};

		template<typename Storages_, std::size_t Index_, typename = void>
		struct dump_storages;
		template<typename Storages_, std::size_t Index_>
		struct dump_storages<Storages_, Index_, typename std::enable_if<Index_ < 27>::type>
		{
			using type =
				typename add_raw_string<
					raw_string<U'¾Æ' + Index_, U':', U' '>,
					typename add_raw_string<
						typename add_raw_string<
							typename dump_storage<typename Storages_::template get<hangul::jongsungs[Index_]>>::type,
							typename make_raw_string_character<U'\n'>::type
						>::type,
						typename dump_storages<Storages_, Index_ + 1>::type
					>::type
				>::type;
		};
		template<typename Storages_, std::size_t Index_>
		struct dump_storages<Storages_, Index_, typename std::enable_if<Index_ == 27>::type>
		{
			using type =
				typename add_raw_string<
					raw_string<U'Û', U':', U' '>,
					typename add_raw_string<
						typename dump_storage<typename Storages_::template get<hangul::jongsungs[Index_]>>::type,
						typename dump_storages<Storages_, Index_ + 1>::type
					>::type
				>::type;
		};
		template<typename Storages_, std::size_t Index_>
		struct dump_storages<Storages_, Index_, typename std::enable_if<Index_ >= 28>::type>
		{
			using type = void;
		};
	}

	namespace debugger
	{
		template<typename States_>
		struct dump_storages
		{
			using type = typename details::dump_storages<typename States_::storages, 0>::type;
		};
	}
}

#endif