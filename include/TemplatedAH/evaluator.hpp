#ifndef TEMPLATEDAH_HEADER_EVALUATOR_HPP
#define TEMPLATEDAH_HEADER_EVALUATOR_HPP

#include "string.hpp"

#include <cstddef>
#include <type_traits>

namespace tah
{
#define TEMPLATEDAH_SUPPORT_PIPE (false)

	namespace details
	{
		enum class direction
		{
			left = 2,
			right = ~left,
			up = 4,
			down = ~up,
		};

		template<direction Direction_>
		struct reverse_direction
		{
			static constexpr direction value =
				static_cast<direction>(~static_cast<int>(Direction_));
		};

		template<std::size_t X_, std::size_t Y_, std::size_t Speed_, tah::details::direction Direction_>
		struct cursor
		{
			static constexpr std::size_t x = X_;
			static constexpr std::size_t y = Y_;
			static constexpr std::size_t speed = Speed_;
			static constexpr tah::details::direction direction = Direction_;
		};
		
		template<typename Lines_, std::size_t Y_>
		struct get_command_y
		{
			using type = typename Lines_::template get<Y_>;
		};
		template<typename Line_, std::size_t X_>
		struct get_command_x
		{
			static constexpr char32_t value = Line_::type::value[X_];
		};
		template<typename Lines_, typename Cursor_>
		struct get_command
		{
			static constexpr char32_t value = get_command_x<get_command_y<Lines_, Cursor_::y>, Cursor_::x>::value;
		};

		template<typename Storages_, typename Cursor_, char32_t SelectedStorage_>
		struct aheui_states
		{
			static constexpr char32_t selected_storage = SelectedStorage_;

			using storages = Storages_;
			using cursor = Cursor_;
			using storage = typename Storages_::template get<get_jongsung_index<SelectedStorage_>::value>;
		};

		template<typename Lines_, typename Cursor_, typename = void>
		struct move_to_cursor;

		template<typename Lines_, typename Cursor_>
		struct move_to_cursor<Lines_, Cursor_,
			typename std::enable_if<Cursor_::y < Lines_::line &&
									Cursor_::x < Lines_::template get<Cursor_::y>::length>::type>
		{
			using type = Cursor_;
		};
		template<typename Lines_, typename Cursor_>
		struct move_to_cursor<Lines_, Cursor_,
			typename std::enable_if<Cursor_::y >= Lines_::line &&
									Cursor_::x < Lines_::template get<Cursor_::y>::length &&
									(Cursor_::y != static_cast<std::size_t>(-1) &&
									 Cursor_::y != static_cast<std::size_t>(-2))>::type>
		{
			using type = cursor<Cursor_::x, 0, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_to_cursor<Lines_, Cursor_,
			typename std::enable_if<Cursor_::y >= Lines_::line &&
									Cursor_::x < Lines_::template get<Cursor_::y>::length &&
									(Cursor_::y == static_cast<std::size_t>(-1) ||
									 Cursor_::y == static_cast<std::size_t>(-2))>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::line - 1, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_to_cursor<Lines_, Cursor_,
			typename std::enable_if<Cursor_::y < Lines_::line &&
									Cursor_::x >= Lines_::template get<Cursor_::y>::length &&
									(Cursor_::x != static_cast<std::size_t>(-1) &&
									 Cursor_::x != static_cast<std::size_t>(-2))>::type>
		{
			using type = cursor<0, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_to_cursor<Lines_, Cursor_,
			typename std::enable_if<Cursor_::y < Lines_::line &&
									Cursor_::x >= Lines_::template get<Cursor_::y>::length &&
									(Cursor_::x == static_cast<std::size_t>(-1) ||
									 Cursor_::x == static_cast<std::size_t>(-2))>::type>
		{
			using type = cursor<Cursor_::template get<Cursor_::y>::length - 1, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
	}
}

#endif