#include <TemplatedAH/evaluator.hpp>
#include <iostream>

TEMPLATEDAH_STRING8(code, u8"밯망희");
TEMPLATEDAH_STRING8(input, u8"A");

int main()
{
	using eval = tah::aheui_eval_utf8<code>::input_utf8<input>::eval;
	tah::print<eval::output_utf8>();

	return 0;
}