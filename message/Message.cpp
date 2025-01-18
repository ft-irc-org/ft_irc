#include "Message.hpp"

Message::Message(const std::string& rowMessage) {
	parse(rowMessage); // 생성과 동시에 파싱
}

const std::string& Message::getVerb() const {
	return verb;
}

const std::vector<std::string>& Message::getParams() const {
	return params;
}

const std::string& Message::getParam(size_t index) const {
	return params[index];
}

size_t Message::getParamCount() const {
	return params.size();
}

void Message::parse(const std::string& rowMessage) {
	// TODO: 파싱 구현
	// 이런 메시지가 있으면
	// PRIVMSG #channel :Hello : World :: This is a message

	// 올바른 파싱 방식
	// Message.verb = "PRIVMSG"
	// Message.params = ["#channel", "Hello : World :: This is a message"] <- vector<string>
}