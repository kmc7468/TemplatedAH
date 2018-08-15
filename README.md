# TemplatedAH
템플릿 메타 프로그래밍을 이용한 아희 인터프리터
## 예제
더 많은 예제는 [examples](https://github.com/kmc7468/TemplatedAH/tree/master/examples) 디렉터리에서 확인하실 수 있습니다.
```cpp
// examples/get_unicode_codepoint.cpp

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

// 출력: 65
## 요구 사항
- C++14 또는 이후의 표준을 지원하는 C++ 컴파일러
	- gcc와 clang의 경우 `-ftemplate-depth-{value}` 컴파일 옵션을 통해 복잡한 아희 코드도 실행시킬 수 있습니다.
- 긴 컴파일 오류 메세지를 읽을 수 있는 용기
- 긴 컴파일 시간을 버텨낼 수 있는 인내심
## 설치 및 사용 방법
TemplatedAH는 Header-only 라이브러리로, 추가적인 컴파일이 필요하지 않습니다.
1. 레포지토리를 복사합니다.
	- `git clone -b stable https://github.com/kmc7468/TemplatedAH.git` 명령어로 복사할 수 있습니다.
	- 항상 최신 버전(안정화가 되지 않았을 수 있습니다.)을 이용하고자 하신다면 위 명령에서 `-b stable`을 삭제하시면 됩니다.
2. 복사된 디렉터리 내부에 있는 `"include"` 디렉터리를 사용하고자 하는 프로젝트 디렉터리 안으로 복사합니다.
3. `"(사용하고자 하는 프로젝트 디렉터리)/include"` 디렉터리를 컴파일러의 포함 디렉터리로 설정합니다.
4. `#include <TemplatedAH/evaluator.hpp>` 전처리문을 코드에 삽입해 사용합니다.
## 매크로
다음에 열거된 매크로를 `#include <TemplatedAH/evaluator.hpp>` 전처리문 이전에 정의하면 특정 상황에서 특수한 동작을 유도할 수 있습니다.
- `TEMPLATEDAH_EOF_ERROR`: 입력이 없을 때 입력에서 값을 받아오려고 하면 컴파일 오류를 발생시킵니다.<br>
정의되지 않았을 경우 이러한 케이스가 발생하면 -1을 받아온 것처럼 처리합니다.
- `TEMPLATEDAH_INVFMT_PASS`: 입력에서 숫자를 받아오려고 했을 때 숫자로 해석할 수 없는 값이면(EOF 제외) -1을 받아온 것처럼 처리합니다. 입력을 읽지는 않습니다.<br>
정의되지 않았을 경우 이러한 케이스가 발생하면 컴파일 오류가 발생합니다.
- `TEMPLATEDAH_INVJONG_PASS`: 종성이 필요하지 않은 명령에 종성이 존재해도 종성이 없는 것처럼 처리합니다.<br>
정의되지 않았을 경우 이러한 케이스가 발생하면 컴파일 오류가 발생합니다.
- `TEMPLATEDAH_INVCHO_PASS`: 아희 표준에 정의되지 않은 초성을 사용하면 초성이 ㅇ인 것처럼 처리합니다.<br>
정의되지 않았을 경우 이러한 케이스가 발생하면 컴파일 오류가 발생합니다.
- `TEMPLATEDAH_NO_PRINT`: `tah::print` 템플릿 함수를 선언/정의하지 않습니다. 코드의 이식성을 높일 수 있습니다.

다음에 열거된 매크로는 TemplatedAH를 조금 더 편하게 사용할 수 있도록 해줍니다.
- `TEMPLATEDAH_STRING8(name: identifier, string: string-literal)`: `name`이라는 이름을 가진 TemplatedAH에서 인식할 수 있는 멀티 바이트 문자열 구조체를 만듭니다. UTF-8을 사용할 것을 권장합니다.<br>
사용 예: `TEMPLATEDAH_STRING8(test, u8"Hello, world!");`
- `TEMPLATEDAH_STRING16(name: identifier, string: utf16-string-literal)`: `name`이라는 이름을 가진 TemplatedAH에서 인식할 수 있는 UTF-16 문자열 구조체를 만듭니다.<br>
사용 예: `TEMPLATEDAH_STRING16(test, u"Hello, world!");`
- `TEMPLATEDAH_STRING32(name: identifier, string: utf32-string-literal)`: `name`이라는 이름을 가진 TemplatedAH에서 인식할 수 있는 UTF-32 문자열 구조체를 만듭니다.<br>
사용 예: `TEMPLATEDAH_STRING32(test, U"Hello, world!");`
- `TEMPLATEDAH_STRING(name: identifier, string: utf32-string-literal)`: `TEMPLATEDAH_STRING32` 매크로와 같습니다.
## 아희
아희는 최초의 난해한 한글 프로그래밍 언어입니다. [여기](http://aheui.github.io/introduction.ko)에서 아희의 소개, 표준, 예제 코드를 확인하실 수 있습니다.
### 통로의 구현
아희 표준에는 통로가 정의되어 있지만, 템플릿 메타 프로그래밍을 이용해 통로를 구현하는 것은 의미가 전혀 없는 것으로 판단되어 스택처럼 동작하도록 구현하였습니다. 통로를 스택이라 생각하시고 사용하시면 됩니다.
## 많이 할 것 같은 질문/요청
**왜 만드셨나요?** 기차에서 심심해서 만들었습니다.<br>
**변태이신가요?** 요즘에는 그럴 수도 있다는 생각을 종종 하곤 합니다.<br>
**[아희++](https://github.com/kmc7468/Aheuiplusplus)도 지원 해 주세요.** 다음에 기차를 탔을 때 심심하면 확장해 보겠습니다. 앞으론 기차를 안 타야겠네요.
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