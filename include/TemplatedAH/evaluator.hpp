﻿#ifndef TEMPLATEDAH_HEADER_EVALUATOR_HPP
#define TEMPLATEDAH_HEADER_EVALUATOR_HPP

#include "debugger.hpp"
#include "storage.hpp"
#include "string.hpp"

#include <cstddef>
#include <type_traits>

namespace tah
{
#define TEMPLATEDAH_SUPPORT_PIPE (false)

#define TEMPLATEDAH_VERSION_STRING "1.0"
#define TEMPLATEDAH_VERSION 1
#define TEMPLATEDAH_VERSION_MAJOR TEMPLATEDAH_VERSION
#define TEMPLATEDAH_VERSION_MINOR 0
#define TEMPLATEDAH_IS_STABLE true

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

		template<char32_t Jungsung_, typename Cursor_, typename = void>
		struct compute_direction
		{
			using type = Cursor_;
		};

		template<std::size_t X_, std::size_t Y_, std::size_t Speed_, tah::details::direction Direction_>
		struct cursor;

		template<typename Cursor_>
		struct compute_direction<U'ㅏ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 1, direction::right>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅑ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 2, direction::right>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅓ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 1, direction::left>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅕ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 2, direction::left>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅗ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 1, direction::up>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅛ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 2, direction::up>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅜ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 1, direction::down>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅠ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, 2, direction::down>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅡ', Cursor_,
			typename std::enable_if<Cursor_::direction == direction::left ||
			Cursor_::direction == direction::right>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅡ', Cursor_,
			typename std::enable_if<Cursor_::direction == direction::up ||
			Cursor_::direction == direction::down>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y, Cursor_::speed, reverse_direction<Cursor_::direction>::value>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅣ', Cursor_,
			typename std::enable_if<Cursor_::direction == direction::left ||
			Cursor_::direction == direction::right>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y, Cursor_::speed, reverse_direction<Cursor_::direction>::value>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅣ', Cursor_,
			typename std::enable_if<Cursor_::direction == direction::up ||
			Cursor_::direction == direction::down>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Cursor_>
		struct compute_direction<U'ㅢ', Cursor_>
		{
			using type = cursor<Cursor_::x, Cursor_::y, Cursor_::speed, reverse_direction<Cursor_::direction>::value>;
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
		template<typename Line_, std::size_t X_, typename = void>
		struct get_command_x;

		template<typename Line_, std::size_t X_>
		struct get_command_x<Line_, X_, typename std::enable_if<
			X_ < Line_::length
			>::type>
		{
			static constexpr char32_t value = Line_::value[X_];
		};
		template<typename Line_, std::size_t X_>
		struct get_command_x<Line_, X_, typename std::enable_if<
			X_ >= Line_::length
			>::type>
		{
			static constexpr char32_t value = U'ㅇ';
		};
		template<typename Lines_, typename Cursor_>
		struct get_command
		{
			static constexpr char32_t value = get_command_x<
				typename get_command_y<Lines_, Cursor_::y>::type, Cursor_::x>::value;
		};

		template<typename Ty_, bool HasValue_, Ty_ Value_>
		struct optional
		{
			static constexpr bool has_value = HasValue_;
			static constexpr Ty_ value = Value_;
		};

		template<bool HasValue_, int_type Value_>
		using optional_int_type = optional<int_type, HasValue_, Value_>;
		template<int_type Value_>
		using create_optional_int_type = optional_int_type<true, Value_>;
		using create_optional_int_type_empty = optional_int_type<false, -1>;

		template<typename Storages_, typename Cursor_, char32_t SelectedStorage_, bool IsExited_,
			typename Result_>
		struct aheui_states
		{
			static constexpr char32_t selected_storage = SelectedStorage_;
			static constexpr bool is_exited = IsExited_;

			using storages = Storages_;
			using cursor = Cursor_;
			using storage = typename Storages_::template get<SelectedStorage_>;
			using result = Result_;
		};

		using create_states = aheui_states<create_storages, cursor<0, 0, 1, direction::down>, 0, false,
			create_optional_int_type_empty>;

		template<typename Lines_, std::size_t X_, std::size_t Index_ = Lines_::line - 1, typename = void>
		struct get_line_by_x;

		template<typename Lines_, std::size_t X_, std::size_t Index_>
		struct get_line_by_x<Lines_, X_, Index_, typename std::enable_if<
			Lines_::template get<Index_>::length <= X_ && Index_ != static_cast<std::size_t>(-1)
		>::type>
		{
			static constexpr std::size_t value = get_line_by_x<Lines_, X_, Index_ - 1>::value;
		};
		template<typename Lines_, std::size_t X_, std::size_t Index_>
		struct get_line_by_x<Lines_, X_, Index_, typename std::enable_if<
			(Lines_::template get<Index_>::length > X_) && Index_ != static_cast<std::size_t>(-1)
		>::type>
		{
			static constexpr std::size_t value = Index_ + 1;
		};
		template<typename Lines_, std::size_t X_, std::size_t Index_>
		struct get_line_by_x<Lines_, X_, Index_, typename std::enable_if<
			Index_ == static_cast<std::size_t>(-1)
		>::type>
		{
			static constexpr std::size_t value = 0;
		};

		template<typename Lines_, typename Cursor_, typename = void>
		struct move_cursor;

		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::left &&
			Cursor_::x - Cursor_::speed < Lines_::template get<Cursor_::y>::length>::type>
		{
			using type = cursor<Cursor_::x - Cursor_::speed, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::left &&
			Cursor_::x - Cursor_::speed >= Lines_::template get<Cursor_::y>::length>::type>
		{
			using type = cursor<Lines_::template get<Cursor_::y>::length - 1, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::right &&
			Cursor_::x + Cursor_::speed < Lines_::template get<Cursor_::y>::length>::type>
		{
			using type = cursor<Cursor_::x + Cursor_::speed, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::right &&
			Cursor_::x + Cursor_::speed >= Lines_::template get<Cursor_::y>::length>::type>
		{
			using type = cursor<0, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::up &&
			Cursor_::y - Cursor_::speed < get_line_by_x<Lines_, Cursor_::x>::value>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y - Cursor_::speed, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::up &&
			Cursor_::y - Cursor_::speed >= get_line_by_x<Lines_, Cursor_::x>::value>::type>
		{
			using type = cursor<Cursor_::x, get_line_by_x<Lines_, Cursor_::x>::value, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::down &&
			Cursor_::y + Cursor_::speed < get_line_by_x<Lines_, Cursor_::x>::value>::type>
		{
			using type = cursor<Cursor_::x, Cursor_::y + Cursor_::speed, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Lines_, typename Cursor_>
		struct move_cursor<Lines_, Cursor_, typename std::enable_if<Cursor_::direction == direction::down &&
			Cursor_::y + Cursor_::speed >= get_line_by_x<Lines_, Cursor_::x>::value>::type>
		{
			using type = cursor<Cursor_::x, 0, Cursor_::speed, Cursor_::direction>;
		};

		template<char32_t Chosung_, char32_t Jongsung_, typename States_, typename Lines_,
			typename Input_, typename Output_, typename = void>
		struct aheui_eval;

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_e;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_e<States_, Lines_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ + rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_e<States_, Lines_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_E;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_E<States_, Lines_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ * rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_E<States_, Lines_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_x;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_x<States_, Lines_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ - rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_x<States_, Lines_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_s;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_s<States_, Lines_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ / rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_s<States_, Lines_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_f;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_f<States_, Lines_, typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ % rhs_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_f<States_, Lines_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_g;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_g<States_, Lines_, typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages,
				typename States_::cursor, States_::selected_storage, true,
				create_optional_int_type<States_::storage::front>>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_g<States_, Lines_, typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
		private:
			using new_storage_ = typename States_::storage::template push_type<8>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename States_::cursor,
				States_::selected_storage, true,
				create_optional_int_type_empty>;
		};

		template<typename States_, typename Lines_, typename Input_>
		struct aheui_eval_internal_q_read_number
		{
		private:
			using internal_ = to_number<Input_>;
			static constexpr int_type read_value_ = internal_::value;
			using new_storage_ = typename States_::storage::template push_type<read_value_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using input_type = typename internal_::input_type;
		};
		template<typename States_, typename Lines_, typename Input_>
		struct aheui_eval_internal_q_read_character
		{
		private:
			using internal_ = to_character<Input_>;
			static constexpr int_type read_value_ = internal_::value;
			using new_storage_ = typename States_::storage::template push_type<read_value_>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using input_type = typename internal_::input_type;
		};
		template<typename States_, typename Lines_, char32_t Jongsung_>
		struct aheui_eval_internal_q
		{
		private:
			using new_storage_ = typename States_::storage::template push_type<
				hangul::numbers[get_jongsung_index<Jongsung_>::value]
			>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_a_write_number;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_a_write_number<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
		private:
			static constexpr int_type value_ = States_::storage::front;
			using new_storage_ = typename States_::storage::pop_type;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using output_type = typename to_string_number<value_>::type;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_a_write_number<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using output_type = void;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_a_write_character;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_a_write_character<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
		private:
			static constexpr char32_t value_ = static_cast<char32_t>(States_::storage::front);
			using new_storage_ = typename States_::storage::pop_type;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using output_type = raw_string<value_>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_a_write_character<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using output_type = void;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_a;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_a<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
		private:
			using new_storage_ = typename States_::storage::pop_type;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_a<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_Q;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_Q<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
		private:
			using new_storage_ = typename States_::storage::copy_type;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_Q<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_v;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_v<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			using new_storage_ =
				typename States_::storage::pop_type::template swap_type<States_::storage::front>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_v<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, char32_t Jongsung_>
		struct aheui_eval_internal_t
		{
		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor>::type, Jongsung_,
				States_::is_exited, typename States_::result>;
		};

		template<typename States_, typename Lines_, char32_t Jongsung_, typename = void>
		struct aheui_eval_internal_T;
		template<typename States_, typename Lines_, char32_t Jongsung_>
		struct aheui_eval_internal_T<States_, Lines_, Jongsung_,
			typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
		private:
			static constexpr int_type move_value_ =
				States_::storage::front;
			using new_storage_ = typename States_::storage::pop_type;
			using new_storage_jong_ = typename States_::storages::template get<Jongsung_>::template push_type<move_value_>;
			using internal_states_type_ = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename internal_states_type_::storages::template set_type<Jongsung_, new_storage_jong_>,
				typename internal_states_type_::cursor, internal_states_type_::selected_storage, internal_states_type_::is_exited,
				typename internal_states_type_::result>;
		};
		template<typename States_, typename Lines_, char32_t Jongsung_>
		struct aheui_eval_internal_T<States_, Lines_, Jongsung_,
			typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_w;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_w<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<2>>::type>
		{
		private:
			static constexpr int_type rhs_ = States_::storage::front;
			using lhs_type_ = typename States_::storage::pop_type;
			static constexpr int_type lhs_ = lhs_type_::front;
			using new_storage_ = typename lhs_type_::pop_type::template push_type<lhs_ >= 1>;

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_w<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<typename States_, typename Lines_, typename = void>
		struct aheui_eval_internal_c;
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_c<States_, Lines_,
			typename std::enable_if<States_::storage::template assert_elements<1>>::type>
		{
		private:
			static constexpr int_type value_ = States_::storage::front;
			using new_storage_ = typename States_::storage::pop_type;

			template<typename Cursor_, int_type Value_, typename = void>
			struct internal_;
			template<typename Cursor_, int_type Value_>
			struct internal_<Cursor_, Value_, typename std::enable_if<Value_ != 0>::type>
			{
				using type = Cursor_;
			};
			template<typename Cursor_, int_type Value_>
			struct internal_<Cursor_, Value_, typename std::enable_if<Value_ == 0>::type>
			{
				using type = cursor<Cursor_::x, Cursor_::y, Cursor_::speed,
					reverse_direction<Cursor_::direction>::value>;
			};

		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages::template set_type<States_::selected_storage, new_storage_>,
				typename move_cursor<Lines_, typename internal_<typename States_::cursor, value_>::type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};
		template<typename States_, typename Lines_>
		struct aheui_eval_internal_w<States_, Lines_,
			typename std::enable_if<!States_::storage::template assert_elements<1>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
		};

		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㄷ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_e<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㄸ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_E<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅌ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_x<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㄴ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_s<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㄹ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_f<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅎ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_g<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅂ', Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Jongsung_ == U'ㅇ'>::type>
		{
		private:
			using internal_type_ = aheui_eval_internal_q_read_number<States_, Lines_, Input_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;
			
			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = typename internal_type_::input_type;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅂ', Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Jongsung_ == U'ㅎ'>::type>
		{
		private:
			using internal_type_ = aheui_eval_internal_q_read_character<States_, Lines_, Input_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = typename internal_type_::input_type;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅂ', Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Jongsung_ != U'ㅇ' && Jongsung_ != U'ㅎ'>::type>
		{
		private:
			using internal_type_ = aheui_eval_internal_q<States_, Lines_, Jongsung_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅁ', Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Jongsung_ == U'ㅇ'>::type>
		{
		private:
			using internal_type_ = aheui_eval_internal_a_write_number<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type =
				typename add_raw_string<Output_, typename internal_type_::output_type>::type;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅁ', Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Jongsung_ == U'ㅎ'>::type>
		{
		private:
			using internal_type_ = aheui_eval_internal_a_write_character<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type =
				typename add_raw_string<Output_, typename internal_type_::output_type>::type;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅁ', Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Jongsung_ != U'ㅇ' && Jongsung_ != U'ㅎ'>::type>
		{
		private:
			using internal_type_ = aheui_eval_internal_a<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅇ', Jongsung_, States_, Lines_, Input_, Output_>
		{
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<Lines_, typename States_::cursor>::type, States_::selected_storage, States_::is_exited,
				typename States_::result>;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<
#ifdef TEMPLATEDAH_INVJONG_PASS
			char32_t Jongsung_,
#endif
			typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅃ',
#ifdef TEMPLATEDAH_INVJONG_PASS
			Jongsung_
#else
			0
#endif
			, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_Q<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<
#ifdef TEMPLATEDAH_INVJONG_PASS
			char32_t Jongsung_,
#endif
			typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅍ',
#ifdef TEMPLATEDAH_INVJONG_PASS
			Jongsung_
#else
			0
#endif
			, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_v<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅅ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_t<States_, Lines_, Jongsung_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<char32_t Jongsung_, typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅆ', Jongsung_, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_T<States_, Lines_, Jongsung_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<
#ifdef TEMPLATEDAH_INVJONG_PASS
			char32_t Jongsung_,
#endif
			typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅈ',
#ifdef TEMPLATEDAH_INVJONG_PASS
			Jongsung_
#else
			0
#endif
			, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_w<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
		template<
#ifdef TEMPLATEDAH_INVJONG_PASS
			char32_t Jongsung_,
#endif
			typename States_, typename Lines_, typename Input_, typename Output_>
		struct aheui_eval<U'ㅊ',
#ifdef TEMPLATEDAH_INVJONG_PASS
			Jongsung_
#else
			0
#endif
			, States_, Lines_, Input_, Output_>
		{
		private:
			using internal_type_ = aheui_eval_internal_c<States_, Lines_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = Output_;
			using input_type = Input_;
		};
#ifdef TEMPLATEDAH_INVCHO_PASS
		template<char32_t Chosung_, char32_t Jongsung_, typename States_, typename Lines_,
			typename Input_, typename Output_>
		struct aheui_eval<Chosung_, Jongsung_, States_, Lines_, Input_, Output_,
			typename std::enable_if<Chosung_ == U'ㄱ' || Chosung_ == U'ㄲ' || Chosung_ == U'ㅉ' || Chosung_ == U'ㅋ'>::type>
		{
		public:
			static constexpr bool is_success = true;

			using states_type = aheui_states<
					typename States_::storages, typename move_cursor<Lines_, typename States_::cursor>::type,
					States_::selected_storage, States_::is_exited, typename States_::result
				>;
			using output_type = Output_;
			using input_type = Input_;
		};
#endif

		template<typename Lines_, typename Input_, typename Output_, typename States_, typename = void>
		struct aheui_run_internal;

		template<typename Lines_, typename Eval_, typename = void>
		struct aheui_run_internal_internal;

		template<typename Lines_, typename Eval_>
		struct aheui_run_internal_internal<Lines_, Eval_,
			typename std::enable_if<!Eval_::states_type::is_exited>::type>
		{
		public:
			using next = typename aheui_run_internal<
				Lines_, typename Eval_::input_type, typename Eval_::output_type, aheui_states<
					typename Eval_::states_type::storages, typename Eval_::states_type::cursor,
					Eval_::states_type::selected_storage, Eval_::states_type::is_exited,
					typename Eval_::states_type::result
				>>::internal_::next;
		};
		template<typename Lines_, typename Eval_>
		struct aheui_run_internal_internal<Lines_, Eval_,
			typename std::enable_if<Eval_::states_type::is_exited>::type>
		{
			using next = Eval_;
		};

		template<typename Lines_, typename Input_, typename Output_, typename States_>
		struct aheui_run_internal<Lines_, Input_, Output_, States_,
			typename std::enable_if<hangul::is_hangul<get_command<Lines_, typename States_::cursor>::value>>::type>
		{
			template<typename Lines2_, typename Eval_, typename>
			friend struct aheui_run_internal_internal;

		private:
			static constexpr char32_t command_ = get_command<Lines_, typename States_::cursor>::value;
			static constexpr char32_t chosung_ = divide_hangul<command_>::chosung;
			static constexpr char32_t jungsung_ = divide_hangul<command_>::jungsung;
			static constexpr char32_t jongsung_ = divide_hangul<command_>::jongsung;
			using new_cursor_ = typename compute_direction<jungsung_, typename States_::cursor>::type;
			using eval_ = aheui_eval<chosung_, jongsung_,
				aheui_states<typename States_::storages, new_cursor_, States_::selected_storage, States_::is_exited,
				typename States_::result>, Lines_, Input_, Output_>;
			using internal_ = aheui_run_internal_internal<Lines_, eval_>;

		public:
			using states = typename internal_::next::states_type;
			using output = typename internal_::next::output_type;
			using result = typename states::result;

			using output_utf8 = typename utf32_to_utf8<output>::type;
			using output_utf16 = typename utf32_to_utf16<output>::type;
			using output_utf32 = output;

			using input_type = typename internal_::next::input_type;
		};
		template<typename Lines_, typename Input_, typename Output_, typename States_>
		struct aheui_run_internal<Lines_, Input_, Output_, States_,
			typename std::enable_if<!hangul::is_hangul<get_command<Lines_, typename States_::cursor>::value>>::type>
		{
			template<typename Lines2_, typename Eval_, typename>
			friend struct aheui_run_internal_internal;

		private:
			using eval_ = aheui_eval<U'ㅇ', 0, States_, Lines_, Input_, Output_>;
			using internal_ = aheui_run_internal_internal<Lines_, eval_>;

		public:
			using states = typename internal_::next::states_type;
			using output = typename internal_::next::output_type;
			using result = typename states::result;

			using output_utf8 = typename utf32_to_utf8<output>::type;
			using output_utf16 = typename utf32_to_utf16<output>::type;
			using output_utf32 = output;

			using input_type = typename internal_::next::input_type;
		};

		template<typename Lines_, typename Input_, typename States_>
		struct aheui_run
		{
			using lines_type = Lines_;
			using input_type = Input_;
			using states_type = States_;

			template<typename String_>
			using input = aheui_run<Lines_, typename add_raw_string<Input_, typename make_raw_string<String_>::type>::type, States_>;
			template<typename String_>
			using input_utf8 = aheui_run<Lines_, typename add_raw_string<Input_,
				typename utf8_to_utf32<
					typename make_raw_string<String_>::type>::type>::type, States_>;
			template<typename String_>
			using input_utf16 = aheui_run<Lines_, typename add_raw_string<Input_,
				typename utf16_to_utf32<
				typename make_raw_string<String_>::type>::type>::type, States_>;
			template<typename String_>
			using input_utf32 = input<String_>;
			using eval = aheui_run_internal<Lines_, Input_, void, States_>;
		};
		template<typename Lines_, typename States_>
		struct aheui_run<Lines_, void, States_>
		{
			using lines_type = Lines_;
			using states_type = States_;

			template<typename String_>
			using input = aheui_run<Lines_, typename make_raw_string<String_>::type, States_>;
			template<typename String_>
			using input_utf8 = aheui_run<Lines_, typename utf8_to_utf32<
				typename make_raw_string<String_>::type>::type, States_>;
			template<typename String_>
			using input_utf16 = aheui_run<Lines_, typename utf16_to_utf32<
				typename make_raw_string<String_>::type>::type, States_>;
			template<typename String_>
			using input_utf32 = input<String_>;
			using eval = aheui_run_internal<Lines_, void, void, States_>;
		};
	}

	template<typename Code_>
	using aheui_eval = details::aheui_run<
		typename details::split_raw_string<typename details::make_raw_string<Code_>::type>::type,
		void, details::create_states>;

	template<typename Code_, typename States_>
	using aheui_eval_raw = details::aheui_run<
		typename details::split_raw_string<typename details::make_raw_string<Code_>::type>::type,
		void, States_>;

	template<typename Code_>
	using aheui_eval_utf8 = details::aheui_run<
		typename details::split_raw_string<typename details::utf8_to_utf32<
			typename details::make_raw_string<Code_>::type>::type>::type,
		void, details::create_states>;

	template<typename Code_>
	using aheui_eval_utf16 = details::aheui_run<
		typename details::split_raw_string<typename details::utf16_to_utf32<
		typename details::make_raw_string<Code_>::type>::type>::type,
		void, details::create_states>;

	template<typename Code_>
	using aheui_eval_utf32 = aheui_eval<Code_>;

	template<typename Code_, typename States_>
	using aheui_eval_raw_utf8 = details::aheui_run<
		typename details::split_raw_string<typename details::utf8_to_utf32<
		typename details::make_raw_string<Code_>::type>::type>::type,
		void, States_>;

	template<typename Code_, typename States_>
	using aheui_eval_raw_utf16 = details::aheui_run<
		typename details::split_raw_string<typename details::utf16_to_utf32<
		typename details::make_raw_string<Code_>::type>::type>::type,
		void, States_>;

	template<typename Code_, typename States_>
	using aheui_eval_raw_utf32 = aheui_eval_raw<Code_, States_>;
}

#endif