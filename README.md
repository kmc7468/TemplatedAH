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

	std::cout << tah::get_string_length<eval::output>::value << std::endl;
	std::cout << (char)eval::output::value[0] << (char)eval::output::value[1] << std::endl;

	return 0;
}
```
## 구현 진행 상황
### 명령의 구현
#### ㅇ 묶음
- [x] ㅇ
- [x] ㅎ
#### ㄷ 묶음
- [x] ㄷ
- [x] ㄸ
- [x] ㅌ
- [x] ㄴ
- [x] ㄹ
#### ㅁ 묶음
- [x] ㅁ
- [x] ㅂ
- [x] ㅃ
- [x] ㅍ
#### ㅅ 묶음
- [ ] ㅅ
- [ ] ㅆ
- [ ] ㅈ
- [ ] ㅊ
### 예외 처리
- [ ] 직사각형이 아닌 코드 실행
## 요구 사항
- C++14 또는 이후의 표준을 지원하는 C++ 컴파일러
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
## 아희
아희는 최초의 난해한 한글 프로그래밍 언어입니다. [여기](http://aheui.github.io/introduction.ko)에서 아희의 소개, 표준, 예제 코드를 확인하실 수 있습니다.
### 통로의 구현
아희 표준에는 통로가 정의되어 있지만, 템플릿 메타 프로그래밍을 이용해 통로를 구현하는 것은 의미가 전혀 없는 것으로 판단되어 스택처럼 동작하도록 구현하였습니다. 통로를 스택이라 생각하시고 사용하시면 됩니다.
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