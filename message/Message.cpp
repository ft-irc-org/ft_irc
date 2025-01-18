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
	size_t pos = 0;
	size_t prev = 0;
	std::string token;
	bool isVerb = true;
	bool isParam = false;
	bool isTrailing = false;

	while ((pos = rowMessage.find(" ", prev)) != std::string::npos) {
		token = rowMessage.substr(prev, pos - prev);
		if (isVerb) {
			verb = token;
			isVerb = false;
			isParam = true;
		} else if (isParam) {
			if (token[0] == ':') {
				params.push_back(rowMessage.substr(prev + 1));
				isParam = false;
				isTrailing = true;
			} else {
				params.push_back(token);
			}
		} else if (isTrailing) {
			params.back() += " " + token;
		}
		prev = pos + 1;
	}
	if (prev < rowMessage.length()) {
		params.push_back(rowMessage.substr(prev));
	}
}