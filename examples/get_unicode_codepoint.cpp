#include <TemplatedAH/evaluator.hpp>
#include <iostream>

TEMPLATEDAH_STRING(code, U"밯망희");
TEMPLATEDAH_STRING(input, U"A");

int main()
{
	using eval = tah::aheui_eval<code>::input<input>::eval;
	tah::print<eval::output>();

	return 0;
}