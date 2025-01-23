### 4.1 연결 등록

여기서 설명하는 명령은 사용자나 서버로서 IRC 서버에 연결을 등록하고 올바르게 연결을 해제하는 데 사용됩니다.

클라이언트나 서버 연결이 등록되기 위해 "PASS" 명령이 필요하지는 않지만, server 메시지나 NICK/USER 조합 중 후자보다 먼저 와야 합니다. 실제 연결에 일정 수준의 보안을 제공하기 위해 모든 서버 연결에 비밀번호를 사용하는 것이 강력히 권장됩니다. 클라이언트가 등록하는 데 권장되는 순서는 다음과 같습니다:

           1. Pass 메시지
           2. Nick 메시지
           3. User 메시지

#### 4.1.1 Password 메시지

      명령: PASS
   매개변수: <password>

PASS 명령은 '연결 비밀번호'를 설정하는 데 사용됩니다. 연결 등록을 시도하기 전에 비밀번호를 설정할 수 있고 설정해야 합니다. 현재 이는 클라이언트가 NICK/USER 조합을 보내기 전에 PASS 명령을 보내야 하고 서버는 모든 SERVER 명령 전에 PASS 명령을 *반드시* 보내야 함을 의미합니다. 제공된 비밀번호는 C/N 라인(서버의 경우) 또는 I 라인(클라이언트의 경우)에 포함된 것과 일치해야 합니다. 등록하기 전에 여러 PASS 명령을 보내는 것이 가능하지만 마지막으로 보낸 것만 확인에 사용되며 등록 후에는 변경할 수 없습니다.

숫자 응답:
           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

예제:
           PASS secretpasswordhere

#### 4.1.2 Nickname 메시지

      명령: NICK
   매개변수: <nickname> [ <hopcount> ]

NICK 메시지는 사용자에게 닉네임을 부여하거나 이전 닉네임을 변경하는 데 사용됩니다. <hopcount> 매개변수는 서버에서만 사용되어 닉네임이 홈 서버에서 얼마나 멀리 떨어져 있는지를 나타냅니다. 로컬 연결의 hopcount는 0입니다. 클라이언트가 제공한 경우 무시되어야 합니다.

다른 클라이언트에 대해 동일한 닉네임을 이미 알고 있는 서버에 NICK 메시지가 도착하면 닉네임 충돌이 발생합니다. 닉네임 충돌의 결과로, 닉네임의 모든 인스턴스가 서버의 데이터베이스에서 제거되고, KILL 명령이 발행되어 다른 모든 서버의 데이터베이스에서 닉네임을 제거합니다. 충돌을 일으킨 NICK 메시지가 닉네임 변경이었다면 원래(이전) 닉도 제거되어야 합니다.

서버가 직접 연결된 클라이언트로부터 동일한 NICK을 받으면, 로컬 클라이언트에 ERR_NICKCOLLISION을 발행하고 NICK 명령을 삭제하며 kill을 생성하지 않을 수 있습니다.

숫자 응답:
           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION

예제:
   NICK Wiz                        ; 새로운 닉 "Wiz" 소개

   :WiZ NICK Kilroy                ; WiZ가 자신의 닉네임을 Kilroy로 변경

#### 4.1.3 User 메시지

      명령: USER
   매개변수: <username> <hostname> <servername> <realname>

USER 메시지는 연결 시작 시 새로운 사용자의 사용자 이름, 호스트 이름, 서버 이름 및 실제 이름을 지정하는 데 사용됩니다. 또한 IRC에 새로 도착한 사용자를 나타내기 위해 서버 간 통신에서도 사용됩니다. USER와 NICK이 모두 클라이언트로부터 수신된 후에만 사용자가 등록되기 때문입니다.

서버 간에는 USER가 클라이언트의 NICKNAME으로 접두사가 붙어야 합니다. USER 명령이 직접 연결된 클라이언트에서 올 때 hostname과 servername은 일반적으로 IRC 서버에서 (보안상의 이유로) 무시되지만, 서버 간 통신에서는 사용됩니다. 이는 새로운 사용자가 나머지 네트워크에 소개될 때 동반되는 USER를 보내기 전에 항상 NICK을 원격 서버에 보내야 함을 의미합니다.

realname 매개변수는 공백 문자를 포함할 수 있고 이것이 마지막 매개변수로 인식되도록 하기 위해 콜론(':')을 접두사로 붙여야 한다는 점에 주의해야 합니다.

USER 메시지에만 의존하여 사용자가 자신의 사용자 이름에 대해 거짓말하기 쉽기 때문에 "Identity Server" 사용이 권장됩니다. 사용자가 연결하는 호스트에 그러한 서버가 활성화되어 있으면 사용자 이름은 "Identity Server"의 응답과 같이 설정됩니다.

숫자 응답:
           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

예제:
   USER guest tolmoon tolsun :Ronnie Reagan
                                   ; "guest"라는 사용자 이름과 실제 이름
                                   "Ronnie Reagan"으로 자신을 등록하는 사용자

   :testnick USER guest tolmoon tolsun :Ronnie Reagan
                                   ; USER 명령이 속한 닉네임과 함께 서버
                                   간의 메시지
