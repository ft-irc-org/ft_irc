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
    std::string cleanMessage = rowMessage;
    cleanMessage.erase(std::remove(cleanMessage.begin(), cleanMessage.end(), '\r'), cleanMessage.end());
    cleanMessage.erase(std::remove(cleanMessage.begin(), cleanMessage.end(), '\n'), cleanMessage.end());

    std::istringstream iss(cleanMessage);
    std::string token;

    // verb 파싱
    if (!(iss >> verb)) return;

    // params 파싱
    std::string remaining;
    getline(iss, remaining);
    if (remaining.empty()) return;

    // leading space 제거
    remaining = remaining.substr(1);
    
    // trailing parameter 처리
    size_t colonPos = remaining.find(" :");
    if (colonPos != std::string::npos) {
        std::string beforeColon = remaining.substr(0, colonPos);
        std::string afterColon = remaining.substr(colonPos + 2);
        
        std::istringstream paramStream(beforeColon);
        while (paramStream >> token) {
            params.push_back(token);
        }
        if (!afterColon.empty()) {
            params.push_back(afterColon);
        }
    } else {
        std::istringstream paramStream(remaining);
        while (paramStream >> token) {
            params.push_back(token);
        }
    }
}