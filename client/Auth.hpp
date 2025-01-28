#ifndef AUTH_HPP
# define AUTH_HPP

# include <iostream>
# include <string>
# include <map>
# include <exception>

# include "../server/ServerConfig.hpp"

class Auth {
	public:
		typedef enum Permission {
			NONE = 0,
			OP = 1 << 0,		       // 채널 운영자 (MODE +o)
			CHANNEL_MODE = 1 << 1,     // 채널 모드 변경 권한
			KICK_POWER = 1 << 2,       // KICK 명령어 사용 권한
			INVITE_POWER = 1 << 3,     // INVITE 명령어 사용 권한
			TOPIC_POWER = 1 << 4       // TOPIC 명령어 사용 권한
		} Permission;

		Auth();
		Auth(const ServerConfig& config);
		~Auth();

		// 권한 확인
		bool hasPermission(const std::string& userNickname, const std::string& channelName, int permissions) const;
		bool isOperator(const std::string& userNickname, const std::string& channelName) const;

		// 권한 부여/박탈
		void grantOperator(const std::string& userNickname, const std::string& channelName);
		void revokeOperator(const std::string& userNickname, const std::string& channelName);
		void grantPermission(const std::string& userNickname, const std::string& channelName, int permissions);
		void revokePermission(const std::string& userNickname, const std::string& channelName, int permissions);

		// 채널/유저 제거
		void removeChannel(const std::string& channelName);
		void removeUser(const std::string& userNickname);
		void removeUserFromChannel(const std::string& userNickname, const std::string& channelName);

		// 닉네임 변경
		void updateNickname(const std::string& oldNickname, const std::string& newNickname);

		// 서버 설정 가져오기
		// const ServerConfig& getConfig() const;
		const std::string& getPassword() const;
		bool isNoob(const std::string& userNickname) const;

	private:
		typedef struct AuthPK {
			std::string userNickname;
			std::string channelName;

			bool operator<(const AuthPK& pk) const;
		} AuthPK;
		
		std::map<AuthPK, unsigned int> authMap;

		const std::string serverPassword;

};

#endif