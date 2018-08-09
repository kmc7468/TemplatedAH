#include <TemplatedAH/evaluator.hpp>
#include <iostream>

TEMPLATEDAH_STRING(code, U"땨뎌더희");

int main()
{
	using storages = tah::details::create_storages;
	using storage_pushed = storages::get<0>::push_type<1>::push_type<2>::push_type<3>;
	using states = tah::details::aheui_states<storages::set_type<0, storage_pushed>,
		tah::details::cursor<0, 0, 1, tah::details::direction::right>, 0, false, tah::details::create_optional_int_type_empty>;

	using eval = tah::aheui_eval_raw<code, states>::eval;
	std::cout << (eval::result::value) << std::endl;
	std::cout << (eval::states::storage::front) << std::endl;

	return 0;
}