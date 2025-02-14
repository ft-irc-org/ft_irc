# ft_irc
# 인터넷 릴레이 채팅

## 개요
이 프로젝트는 자신만의 IRC 서버를 만드는 것에 관한 것입니다.
실제 IRC 클라이언트를 사용하여 당신의 서버에 연결하고 테스트할 것입니다.
인터넷은 연결된 컴퓨터들이 서로 상호작용할 수 있게 하는 견고한 표준 프로토콜에 의해 운영됩니다.
이것을 아는 것은 항상 좋은 일입니다.
버전: 8

## 목차
1. 소개
2. 일반 규칙
3. 필수 파트
   3.1 요구사항
   3.2 MacOS 전용
   3.3 테스트 예시
4. 보너스 파트
5. 제출 및 동료평가

## 챕터 I: 소개
인터넷 릴레이 채팅 또는 IRC는 인터넷 상의 텍스트 기반 통신 프로토콜입니다.
공개 또는 비공개로 실시간 메시징을 제공합니다. 사용자들은 직접 메시지를 교환하고 그룹 채널에 참여할 수 있습니다.

IRC 클라이언트들은 채널에 참여하기 위해 IRC 서버에 연결합니다. IRC 서버들은 네트워크를 형성하기 위해 서로 연결됩니다.

## 챕터 II: 일반 규칙
* 프로그램은 어떤 상황에서도 (메모리가 부족할 때도) 충돌하지 않아야 하며, 예기치 않게 종료되어서는 안 됩니다.
  만약 이런 일이 발생하면, 프로젝트는 비기능적인 것으로 간주되어 0점을 받게 됩니다.

* 소스 파일을 컴파일할 Makefile을 제출해야 합니다. 재링크되어서는 안 됩니다.

* Makefile은 최소한 다음 규칙들을 포함해야 합니다:
  $(NAME), all, clean, fclean, re

* c++와 플래그 -Wall -Wextra -Werror로 코드를 컴파일하세요.

* 코드는 C++ 98 표준을 준수해야 합니다. 따라서 플래그 -std=c++98을 추가해도 여전히 컴파일되어야 합니다.

* 가능한 한 많은 C++ 기능을 사용하여 개발하도록 하세요 (예: <string.h> 대신 <cstring> 선택). C 함수를 사용할 수는 있지만, 가능하다면 항상 C++ 버전을 선호하세요.

* 모든 외부 라이브러리와 Boost 라이브러리는 금지됩니다.

## 챕터 III: 필수 파트

### 프로그램 정보
* 프로그램 이름: ircserv
* 제출 파일: Makefile, *.{h, hpp}, *.cpp, *.tpp, *.ipp, 선택적 설정 파일
* Makefile: NAME, all, clean, fclean, re
* 인자: 
  - port: 리스닝 포트
  - password: 연결 비밀번호
* 허용된 외부 함수: 
  C++ 98의 모든 것.
  socket, close, setsockopt, getsockname,
  getprotobyname, gethostbyname, getaddrinfo,
  freeaddrinfo, bind, connect, listen, accept, htons,
  htonl, ntohs, ntohl, inet_addr, inet_ntoa, send,
  recv, signal, sigaction, lseek, fstat, fcntl, poll
  (또는 동등한 것)
* 설명: C++ 98로 작성된 IRC 서버

C++ 98로 IRC 서버를 개발해야 합니다.
클라이언트를 개발해서는 안 됩니다.
서버 간 통신을 처리해서는 안 됩니다.

실행 파일은 다음과 같이 실행됩니다:
```bash
./ircserv <port> <password>
```

* port: IRC 서버가 들어오는 IRC 연결을 수신 대기할 포트 번호
* password: 연결 비밀번호. 서버에 연결하려는 모든 IRC 클라이언트에 필요합니다.

비록 subject와 평가 척도에서 poll()이 언급되었지만,
select(), kqueue(), epoll() 등과 같은 동등한 것을 사용할 수 있습니다.

### III.1 요구사항

* 서버는 동시에 여러 클라이언트를 처리할 수 있어야 하며 절대 멈추지 않아야 합니다.

* 포킹은 허용되지 않습니다. 모든 I/O 작업은 논블로킹이어야 합니다.

* 단 하나의 poll() (또는 동등한 것)만 이러한 모든 작업(읽기, 쓰기, 리스닝 등)을 처리하는 데 사용될 수 있습니다.

* 논블로킹 파일 디스크립터를 사용해야 하기 때문에, poll() (또는 동등한 것) 없이 
  read/recv나 write/send 함수를 사용하는 것이 가능하고 서버가 블로킹되지 않을 것입니다.
  하지만 이는 더 많은 시스템 리소스를 소비할 것입니다.
  따라서 poll() (또는 동등한 것) 없이 파일 디스크립터에서 read/recv나 write/send를 
  시도하면 0점을 받게 됩니다.

* 여러 IRC 클라이언트가 존재합니다. 그 중 하나를 레퍼런스로 선택해야 합니다. 
  선택한 레퍼런스 클라이언트는 평가 과정에서 사용될 것입니다.

* 레퍼런스 클라이언트는 오류 없이 서버에 연결할 수 있어야 합니다.

* 클라이언트와 서버 간의 통신은 TCP/IP (v4 또는 v6)를 통해 이루어져야 합니다.

* 레퍼런스 클라이언트로 서버를 사용하는 것은 공식 IRC 서버를 사용하는 것과 유사해야 합니다.
  하지만 다음 기능들만 구현하면 됩니다:

  * 레퍼런스 클라이언트를 사용하여 인증, 닉네임 설정, 사용자 이름 설정, 채널 참여, 
    개인 메시지 송수신이 가능해야 합니다.
  
  * 한 클라이언트가 채널로 보낸 모든 메시지는 해당 채널에 참여한 다른 모든 클라이언트에게
    전달되어야 합니다.
  
  * 운영자와 일반 사용자가 있어야 합니다.
  
  * 그리고 채널 운영자에게 특화된 다음 명령들을 구현해야 합니다:
    * KICK - 채널에서 클라이언트 퇴출
    * INVITE - 채널에 클라이언트 초대
    * TOPIC - 채널 주제 변경 또는 보기
    * MODE - 채널의 모드 변경:
      - i: 초대 전용 채널 설정/해제
      - t: TOPIC 명령의 제한을 채널 운영자로 설정/해제
      - k: 채널 키(비밀번호) 설정/해제
      - o: 채널 운영자 권한 부여/해제
      - l: 채널 사용자 제한 설정/해제

* 당연히 깔끔한 코드를 작성해야 합니다.

### III.2 MacOS 전용

MacOS는 write()를 다른 Unix OS와 다르게 구현하기 때문에, 
fcntl()을 사용할 수 있습니다.
다른 Unix OS와 유사한 동작을 얻기 위해 논블로킹 모드의 파일 디스크립터를 사용해야 합니다.

하지만 다음과 같이만 fcntl()을 사용할 수 있습니다:
```c
fcntl(fd, F_SETFL, O_NONBLOCK);
```
다른 플래그는 금지됩니다.

### III.3 테스트 예시

모든 가능한 오류와 문제(부분 데이터 수신, 낮은 대역폭 등)를 절대적으로 검증하세요.

서버가 보내는 모든 것을 정확하게 처리하는지 확인하기 위해 nc를 사용한 다음과 같은 간단한 
테스트를 수행할 수 있습니다:

```bash
$> nc -C 127.0.0.1 6667
com^Dman^Dd
$>
```

ctrl+D를 사용하여 명령을 여러 부분으로 나누어 보냅니다: 'com', 그 다음 'man', 그 다음 'd\n'.

명령을 처리하기 위해서는 먼저 수신된 패킷들을 집계하여 재구성해야 합니다.

## 챕터 IV: 보너스 파트

IRC 서버를 실제 IRC 서버처럼 보이게 만들기 위해 추가할 수 있는 추가 기능들입니다:

* 파일 전송 처리
* 봇

보너스 파트는 필수 파트가 완벽한 경우에만 평가됩니다. 완벽하다는 것은 필수 파트가 완전히 
수행되었고 오작동 없이 작동한다는 것을 의미합니다. 모든 필수 요구사항을 통과하지 못했다면,
보너스 파트는 전혀 평가되지 않을 것입니다.

## 챕터 V: 제출 및 동료평가

평소와 같이 Git 저장소에 과제를 제출하세요. 방어 중에는 저장소 내부의 작업만 평가됩니다.
파일 이름이 올바른지 다시 한 번 확인하는 것을 주저하지 마세요.

프로젝트에 대한 테스트 프로그램을 만드는 것을 권장합니다. 비록 제출되지 않고 평가되지 않더라도, 
이러한 테스트는 방어 중에 서버를 테스트하는 데 특히 유용할 수 있으며, 언젠가 다른 ft_irc를 
평가해야 할 경우 동료의 것을 테스트하는 데도 유용할 수 있습니다. 실제로 평가 과정에서 
필요한 어떤 테스트든 자유롭게 사용할 수 있습니다.

레퍼런스 클라이언트는 평가 과정에서 사용될 것입니다.