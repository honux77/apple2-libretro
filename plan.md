# plan for apple2 libretro

## 무엇을 만들까?

- libretro library
- apple2 에뮬레이션


## 참고 프로젝트

- applewin (windows, c 환경)
- libretro bluemsx (msx 에뮬이지만 구조적으로 참고할 만함)
- mame apple2 관련 소스파일 및 펌웨어 파일

## apple2 하드웨어 구조


## 기능

- 머신 선택 - apple2, apple2p, apple2e, apple2ee
- 모킹 보드
- 다양한 플로피 포맷 지원
- 하드 디스크 지원

## 참고 사이트

- https://www.cs.columbia.edu/~sedwards/apple2fpga/
- https://github.com/mamedev/mame/blob/master/src/mame/apple/apple2.cpp

## 할 일

- 가능하면 자율적으로 의사결정 
- 필요한 권한 미리 확보하기
- 자주 커밋 (작은 피쳐 하나 완성시마다)
- 가능하면 테스트 코드 작성하며 진행 
