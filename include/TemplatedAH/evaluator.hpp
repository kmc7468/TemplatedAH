#ifndef TEMPLATEDAH_HEADER_EVALUATOR_HPP
#define TEMPLATEDAH_HEADER_EVALUATOR_HPP

#include "storage.hpp"
#include "string.hpp"

#include <cstddef>
#include <type_traits>

namespace tah
{
#define TEMPLATEDAH_SUPPORT_PIPE (false)

#define TEMPLATEDAH_VERSION_STRING "1.0.0"
#define TEMPLATEDAH_VERSION 1
#define TEMPLATEDAH_VERSION_MAJOR TEMPLATEDAH_VERSION
#define TEMPLATEDAH_VERSION_MINOR 0
#define TEMPLATEDAH_VERSION_PATCH 0

#define TEMPLATEDAH_CPP_STANDARD 201402L // C++14

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

			using reversed_type = cursor<X_, Y_, Speed_, reverse_direction<Direction_>::value>;
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

		template<typename Cursor_, typename = void>
		struct move_cursor;

		template<typename Cursor_>
		struct move_cursor<Cursor_, typename std::enable_if<Cursor_::direction == direction::left>::type>
		{
			using type = cursor<Cursor_::x + Cursor_::speed, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Cursor_>
		struct move_cursor<Cursor_, typename std::enable_if<Cursor_::direction == direction::right>::type>
		{
			using type = cursor<Cursor_::x - Cursor_::speed, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Cursor_>
		struct move_cursor<Cursor_, typename std::enable_if<Cursor_::direction == direction::up>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y - Cursor_::speed, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Cursor_>
		struct move_cursor<Cursor_, typename std::enable_if<Cursor_::direction == direction::down>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y + Cursor_::speed, Cursor_::speed, Cursor_::direction>;
		};

		template<char32_t Chosung_, char32_t Jongsung_, typename States_>
		struct aheui_eval;

		template<typename States_, typename = void>
		struct aheui_eval_internal_e;
		template<typename States_>
		struct aheui_eval_internal_e<States_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ + rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage>;
		};
		template<typename States_>
		struct aheui_eval_internal_e<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage>;
		};

		template<typename States_, typename = void>
		struct aheui_eval_internal_E;
		template<typename States_>
		struct aheui_eval_internal_E<States_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ * rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage>;
		};
		template<typename States_>
		struct aheui_eval_internal_E<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage>;
		};

		template<typename States_, typename = void>
		struct aheui_eval_internal_x;
		template<typename States_>
		struct aheui_eval_internal_x<States_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ - rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage>;
		};
		template<typename States_>
		struct aheui_eval_internal_x<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage>;
		};

		template<typename States_, typename = void>
		struct aheui_eval_internal_s;
		template<typename States_>
		struct aheui_eval_internal_s<States_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ / rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage>;
		};
		template<typename States_>
		struct aheui_eval_internal_s<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage>;
		};

		template<typename States_, typename = void>
		struct aheui_eval_internal_f;
		template<typename States_>
		struct aheui_eval_internal_f<States_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ % rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage>;
		};
		template<typename States_>
		struct aheui_eval_internal_f<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage>;
		};

		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'¤§', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_e<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'¤¨', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_E<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'¤¼', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_x<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'¤¤', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_s<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'¤©', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_f<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
		};
	}
}

#endif