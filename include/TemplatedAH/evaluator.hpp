#ifndef TEMPLATEDAH_HEADER_EVALUATOR_HPP
#define TEMPLATEDAH_HEADER_EVALUATOR_HPP

#include "string.hpp"

#include <cstddef>

namespace tah
{
	namespace details
	{
		enum class direction
		{
			left = 1,
			right = ~left,
			up = 2,
			down = ~up,
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
	}
}

#endif