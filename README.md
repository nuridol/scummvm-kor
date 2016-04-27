ScummVM-kor
===========

>1.8.0(2016-03-04)을 기반으로 ScummVM kor.에 있는 한국어 출력 기능을 추가해 iOS용 빌드가 가능하도록 수정/변환한 버전입니다.

# 사용법
+ iOS 9 SDK를 사용한 Xcode 7.3에서 빌드 가능합니다.
+ 프로젝트 파일을 작성해서 빌드하기 바랍니다.
+ 실기기에서 테스트하기 위해서는 iOS 개발자 프로그램 가입이 필요합니다.(유료 계정이 아니더라도 가능합니다.) Bundle Identifier를 프로파일에 맞게 수정하기 바랍니다.

# 프로젝트 작성
- 다음 내용을 prepare_workspace.sh로 저장하고 실행합니다.

```
#!/bin/bash 

LIBS_ZIP_URL="http://bsr43.free.fr/scummvm/ScummVM-iOS-libraries.zip" 
GIT_REPO_URL="https://github.com/nuridol/scummvm-kor.git" 

# Clone the repository 
git clone "$GIT_REPO_URL" 

# Compile create_project 
(cd scummvm-kor/devtools/create_project/xcode; xcodebuild) 

# Create the workspace 
mkdir build 
cd build 
curl -L "$LIBS_ZIP_URL" -O 
unzip ScummVM-iOS-libraries.zip 
rm ScummVM-iOS-libraries.zip 

../scummvm-kor/devtools/create_project/xcode/build/Release/create_project ../scummvm-kor --xcode --enable-fluidsynth --disable-jpeg --disable-bink --disable-16bit --disable-mt32emu --disable-nasm --disable-opengl --disable-theora --disable-taskbar 
open scummvm.xcodeproj
```

# 게임별 한국어 모드 지정 방법
| 게임명              | 언어  | 모드  | 플랫폼 |
| ----------------- |:----:|:-----:|:-----:|
| 매니악 맨션          |Korean| V1   | DOS |
| 풀 쓰로틀            |Korean| V1   | DOS |
| 샘 & 맥스 히트 더 로드 |Korean| V1   | DOS |
| 룸 VGA Ver.        |Korean| V1   | DOS |
| 텐타클의 최후의 날     |Korean| V2   | DOS |
| 더 디그             |Korean| V2   | DOS |
| 룸 EGA Ver.        |Korean| V2   | DOS |
| 인디아나 존스3        |Korean| V2   | DOS |
| 인디아나 존스4        |Korean| V2   | DOS |
| 원숭이 섬의 비밀1     |Korean| V2   | DOS |
| 원숭이 섬의 비밀2     |Korean| V2   | DOS |

*V1 대상 게임은 sub 폴더 밑에 ``<게임ID>.dat`` 파일이 필요합니다.*

# 링크들
## ScummVM
http://scummvm.org

## Xcode 프로젝트 작성 방법
http://forums.scummvm.org/viewtopic.php?t=13965&postdays=0&postorder=asc&start=60

## ScummVM Kor.(한글화)
http://wonst719.cafe24.com/zbxe/about

## ScummVM Kor Patch v1.2.1k11(iOS 버전 기능 추가)
http://www.nextcube.org/board/browse/1111/4188
