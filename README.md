# TemplatedAH
템플릿 메타 프로그래밍을 이용한 아희 인터프리터
## 예제
더 많은 예제는 [examples](https://github.com/kmc7468/TemplatedAH/tree/master/examples) 디렉터리에서 확인하실 수 있습니다.
```cpp
// examples/calc_example.cpp
#include <TemplatedAH/evaluator.hpp>
#include <iostream>

TEMPLATEDAH_STRING(code, U"땨뎌더희");

int main()
{
	using storages = tah::details::create_storages;
	using storage_pushed = storages::get<0>::push_type<1>::push_type<2>::push_type<3>;
	using states = tah::details::aheui_states<storages::set_type<0, storage_pushed>,
		tah::details::cursor<0, 0, 1, tah::details::direction::right>, 0, false>;

	using eval = tah::aheui_eval_raw<code, states>::eval;
	std::cout << (eval::states::storage::front) << std::endl;

	return 0;
}
```
## 요구 사항
- C++14 또는 이후의 표준을 지원하는 C++ 컴파일러
- 긴 컴파일 오류 메세지를 읽을 수 있는 용기
- 긴 컴파일 시간을 버텨낼 수 있는 인내심
## 설치 및 사용 방법
TemplatedAH는 Header-only 라이브러리로, 추가적인 컴파일이 필요하지 않습니다.
1. 레포지토리를 복사합니다.
	- `git clone -b stable https://github.com/kmc7468/TemplatedAH.git` 명령어로 복사할 수 있습니다.
	- 항상 최신 버전(안정화가 되지 않았을 수 있습니다.)을 이용하고자 하신다면 위 명령에서 `-b stable`을 삭제하시면 됩니다.
2. 복사된 디렉터리 내부에 있는 include 디렉터리를 사용하고자 하는 프로젝트 디렉터리 안으로 복사합니다.
3. `"(사용하고자 하는 프로젝트 디렉터리)/include"` 디렉터리를 컴파일러의 포함 디렉터리로 설정합니다.
4. `#include <TemplatedAH/evaluator.hpp>` 전처리문을 코드에 삽입해 사용합니다.
## 매크로
다음에 열거된 매크로를 `#include <TemplatedAH/evaluator.hpp>` 전처리문 이전에 정의하면 특정 상황에서 특수한 동작을 유도할 수 있습니다.
- `TEMPLATEDAH_EOF_ERROR`: 입력이 없을 때 입력에서 값을 받아오려고 하면 컴파일 오류를 발생시킵니다.<br>
정의되지 않았을 경우 이러한 케이스가 발생하면 -1을 받아온 것처럼 처리합니다.
- `TEMPLATEDAH_INVFMT_PASS`: 입력에서 숫자를 받아오려고 했을 때 숫자로 해석할 수 없는 값이면(EOF 제외) -1을 받아온 것처럼 처리합니다. 입력을 읽지는 않습니다.<br>
정의되지 않았을 경우 이러한 케이스가 발생하면 컴파일 오류가 발생합니다.
## 아희
아희는 최초의 난해한 한글 프로그래밍 언어입니다. [여기](http://aheui.github.io/introduction.ko)에서 아희의 소개, 표준, 예제 코드를 확인하실 수 있습니다.
## 라이선스
```
MIT License

Copyright (c) 2018 kmc7468

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```