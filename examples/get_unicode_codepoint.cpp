#include <TemplatedAH/evaluator.hpp>
#include <iostream>

TEMPLATEDAH_STRING(code, U"밯망희");
TEMPLATEDAH_STRING(input, U"A");

int main()
{
	using eval = tah::aheui_eval<code>::input<input>::eval;

	std::cout << tah::get_string_length<eval::output>::value << std::endl;
	std::cout << (char)eval::output::value[0] << (char)eval::output::value[1] << std::endl;

	return 0;
}