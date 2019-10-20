ScummVM-kor
===========

> ScummVM 2.1.0 "Electric Sheep" (2019-10-11) 기반  
> ScummVM kor.에 있는 한국어 출력 기능을 수정/변환해서 추가  
> [ScummVM-kor with libretro core](https://github.com/swkim01/scummvm-kor) 에 있는 시에라 게임 한국어 출력 기능을 수정/변환해서 추가(Thanks to swkim01)

# 컴파일 방법

+ 각 OS별 컴파일 방법은 http://wiki.scummvm.org/index.php/Compiling_ScummVM 를 참조하기 바랍니다.


## iOS용 프로젝트 작성

- 실기기에서 테스트하기 위해서는 iOS 개발자 프로그램 가입이 필요합니다.(유료 계정이 아니더라도 가능합니다.) Bundle Identifier를 프로파일에 맞게 수정하기 바랍니다.
- 다음 내용을 prepare_workspace.sh로 저장하고 실행합니다.

```bash
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
if [ ! -f ScummVM-iOS-libraries.zip ]; then 
  curl -L "$LIBS_ZIP_URL" -O 
  unzip ScummVM-iOS-libraries.zip 
#  rm ScummVM-iOS-libraries.zip 
fi 

../scummvm-kor/devtools/create_project/xcode/build/Release/create_project ../scummvm-kor --xcode --enable-fluidsynth --disable-jpeg --disable-bink --disable-16bit --disable-mt32emu --disable-nasm --disable-opengl --disable-theora --disable-taskbar --disable-cloud --disable-libcurl --disable-sdlnet 
open scummvm.xcodeproj
```

# 루카스 아츠 게임별 한국어 모드 지정 방법

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

# 시에라 엔터테인먼트 게임별 한글화 대응 

| 게임명              | 언어  | 패키지 | 플랫폼 |
| ----------------- |:----:|:-----:|:-----:|
| 브레인 박사의 성 | Korean | PC | DOS |
| 에코퀘스트 1 | Korean | CD | DOS/Windows |
| 킹즈퀘스트 1 VGA | Korean | PC | DOS |
| 킹즈퀘스트 5 | Korean | Floppy | DOS |
| 킹즈퀘스트 6 | Korean | CD/Floppy | DOS/Windows |
| 가브리엘 나이트1 | Korean | CD | DOS/Windows |
| 스페이스퀘스트 4 | Korean | CD | DOS/Windows |
| 로라 보우 2 태양신의 단도 | Korean | CD | DOS/Windows |

*시에라 엔터테인먼트 게임은 게임 폴더 밑에 ``korean.fnt``(한글 폰트), ``message.map``(메시지 매핑), ``resource.msg``(메시지) 파일이 필요합니다.*  
*[ScummVM-kor with libretro core](https://github.com/swkim01/scummvm-kor) 참조*  


# 링크들

## ScummVM

http://scummvm.org

## Xcode 프로젝트 작성 방법

http://forums.scummvm.org/viewtopic.php?t=13965&postdays=0&postorder=asc&start=60

## ScummVM Kor.

http://wonst719.cafe24.com/zbxe/about

## ScummVM Kor Patch v1.2.1k11(iOS 버전 기능 추가)

http://www.nextcube.org/board/browse/1111/4188

## Korea ADV

http://cafe.naver.com/koreaadv

## ScummVM-kor with libretro core

https://github.com/swkim01/scummvm-kor
