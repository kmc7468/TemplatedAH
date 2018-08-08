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
#define TEMPLATEDAH_IS_STABLE false

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

		template<typename Storages_, typename Cursor_, char32_t SelectedStorage_, bool IsExited_>
		struct aheui_states
		{
			static constexpr char32_t selected_storage = SelectedStorage_;
			static constexpr bool is_exited = IsExited_;

			using storages = Storages_;
			using cursor = Cursor_;
			using storage = typename Storages_::template get<get_jongsung_index<SelectedStorage_>::value>;
		};

		using create_states = aheui_states<create_storages, cursor<0, 0, 1, direction::down>, 0, false>;

		template<typename Lines_, typename Cursor_, typename = void>
		struct move_to_cursor;

		template<typename Lines_, typename Cursor_>
		struct move_to_cursor<Lines_, Cursor_,
			typename std::enable_if < Cursor_::y < Lines_::line &&
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
			using type = cursor<Lines_::template get<Cursor_::y>::length - 1, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};

		template<typename Cursor_, typename = void>
		struct move_cursor;

		template<typename Cursor_>
		struct move_cursor<Cursor_, typename std::enable_if<Cursor_::direction == direction::left>::type>
		{
			using type = cursor<Cursor_::x - Cursor_::speed, Cursor_::y, Cursor_::speed, Cursor_::direction>;
		};
		template<typename Cursor_>
		struct move_cursor<Cursor_, typename std::enable_if<Cursor_::direction == direction::right>::type>
		{
			using type = cursor<Cursor_::x + Cursor_::speed, Cursor_::y, Cursor_::speed, Cursor_::direction>;
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
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
		};
		template<typename States_>
		struct aheui_eval_internal_e<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
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
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
		};
		template<typename States_>
		struct aheui_eval_internal_E<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
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
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
		};
		template<typename States_>
		struct aheui_eval_internal_x<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
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
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
		};
		template<typename States_>
		struct aheui_eval_internal_s<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
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
				typename move_cursor<typename States_::cursor>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
		};
		template<typename States_>
		struct aheui_eval_internal_f<States_, typename std::enable_if<!States_::storage::template assert_elements<2>>::type>
		{
			static constexpr bool is_success = false;

			using states_type = aheui_states<typename States_::storages,
				typename move_cursor<typename States_::cursor::reversed_type>::type, States_::selected_storage, States_::is_exited>;
			using output_type = void;
		};

		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'ㄷ', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_e<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = typename internal_type_::output_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'ㄸ', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_E<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = typename internal_type_::output_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'ㅌ', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_x<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = typename internal_type_::output_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'ㄴ', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_s<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = typename internal_type_::output_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'ㄹ', Jongsung_, States_>
		{
		private:
			using internal_type_ = aheui_eval_internal_f<States_>;

		public:
			static constexpr bool is_success = internal_type_::is_success;

			using states_type = typename internal_type_::states_type;
			using output_type = typename internal_type_::output_type;
		};
		template<char32_t Jongsung_, typename States_>
		struct aheui_eval<U'ㅎ', Jongsung_, States_>
		{
			static constexpr bool is_success = true;

			using states_type = aheui_states<typename States_::storages,
				typename States_::cursor, States_::selected_storage, true>;
			using output_type = void;
		};

		template<typename Output_, int_type Result_>
		struct aheui_output
		{
			static constexpr int_type result = Result_;

			using output = Output_;
		};

		template<typename Lines_, typename Input_, typename Output_, typename States_, typename = void>
		struct aheui_run_internal;

		template<typename Lines_, typename Input_, typename Output_, typename Eval_, typename = void>
		struct aheui_run_internal_internal;

		template<typename Lines_, typename Input_, typename Output_, typename Eval_>
		struct aheui_run_internal_internal<Lines_, Input_, Output_, Eval_,
			typename std::enable_if<!Eval_::states_type::is_exited>::type>
		{
		private:
			using moved_cursor_internal_ = typename Eval_::states_type::cursor;
			using moved_cursor_ = typename move_to_cursor<Lines_, moved_cursor_internal_>::type;

		public:
			using next_eval = typename aheui_run_internal<
				Lines_, Input_, typename Eval_::states_type, Output_>::eval_;
		};
		template<typename Lines_, typename Input_, typename Output_, typename Eval_>
		struct aheui_run_internal_internal<Lines_, Input_, Output_, Eval_,
			typename std::enable_if<Eval_::states_type::is_exited>::type>
		{
			using next_eval = Eval_;
		};

		template<typename Lines_, typename Input_, typename States_, typename Output_>
		struct aheui_run_internal<Lines_, Input_, States_, Output_,
			typename std::enable_if<hangul::is_hangul<get_command<Lines_, typename States_::cursor>::value>>::type>
		{
			template<typename Lines_, typename Input_, typename Output_, typename Eval_, typename>
			friend struct aheui_run_internal_internal;

		private:
			static constexpr char32_t command_ = get_command<Lines_, typename States_::cursor>::value;
			static constexpr char32_t chosung_ = divide_hangul<command_>::chosung;
			static constexpr char32_t jungsung_ = divide_hangul<command_>::jungsung;
			static constexpr char32_t jongsung_ = divide_hangul<command_>::jongsung;
			using new_cursor_ = typename compute_direction<jungsung_, typename States_::cursor>::type;
			using eval_ = aheui_eval<chosung_, jongsung_,
				aheui_states<typename States_::storages, new_cursor_, States_::selected_storage, States_::is_exited>>;
			using internal_ = aheui_run_internal_internal<Lines_, Input_, Output_, eval_>;

		public:
			using states = typename internal_::next_eval::states_type;
			using output = typename add_raw_string<Output_, typename internal_::next_eval::output_type>::type;
		};

		template<typename Lines_, typename Input_, typename States_>
		struct aheui_run
		{
			using lines_type = Lines_;
			using input_type = Input_;
			using states_type = States_;

			template<typename String_>
			using input = aheui_run<Lines_, typename add_raw_string<Input_, typename make_raw_string<String_>::type>::type, States_>;
			using eval = aheui_run_internal<Lines_, Input_, States_, void>;
		};
		template<typename Lines_, typename States_>
		struct aheui_run<Lines_, void, States_>
		{
			using lines_type = Lines_;
			using states_type = States_;

			template<typename String_>
			using input = aheui_run<Lines_, typename make_raw_string<String_>::type, States_>;
			using eval = aheui_run_internal<Lines_, void, States_, void>;
		};
	}

	using details::int_type;
	using details::aheui_output;

	template<typename Code_>
	using aheui_eval = details::aheui_run<
		typename details::split_raw_string<typename details::make_raw_string<Code_>::type>::type,
		void, details::create_states>;

	template<typename Code_, typename States_>
	using aheui_eval_raw = details::aheui_run<
		typename details::split_raw_string<typename details::make_raw_string<Code_>::type>::type,
		void, States_>;
}

#endif