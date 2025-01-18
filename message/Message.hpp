#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <iostream>
#include <string>
#include <exception>
#include <vector>

class Message {
	public:
		Message(const std::string& rowMessage); // 생성자, 생성과 동시에 파싱할 수 있도록

		const std::string& getVerb() const; // 명령어 반환 (ex. PRIVMSG, JOIN, PING)
		const std::vector<std::string>& getParams() const; // 파람 전체를 반환 (ex. ["#channel", "message"], ["localhost"], ["user1", "user2"])

		const std::string& getParam(size_t index) const; // 파람 중 index번째를 반환 (ex. 0 -> "#channel", 1 -> "message")

		size_t getParamCount() const; // 파람의 개수를 반환

	private:
		std::string verb;
		std::vector<std::string> params;
		
		// 원본: "/msg hi,hi2,hi3 test msg,test msg2"
		// 서버 수신:
		// "PRIVMSG hi :test msg,test msg2"     -> params = ["hi", "test msg,test msg2"]
		// "PRIVMSG hi2 :test msg,test msg2"    -> params = ["hi2", "test msg,test msg2"]
		// "PRIVMSG hi3 :test msg,test msg2"    -> params = ["hi3", "test msg,test msg2"]
		void parse(const std::string& rowMessage);
};

#endif