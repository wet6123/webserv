#include "../inc/ErrorResponse.hpp"

namespace ErrorResponse {

/** 
 * @brief 에러 응답을 반환합니다.
 * @param status 상태 코드
 * @return std::string 에러 응답
*/
std::string getErrorResponse(Status status) {
	std::map<Status, std::string>::iterator it = ResponseHandle::Utils::REASON_PHRASE.find(status);

	if (it != ResponseHandle::Utils::REASON_PHRASE.end()) {
		std::string reason = it->second;
		std::string response = "HTTP/1.1 " + std::to_string(status) + " " + reason + "\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Content-Length: " + std::to_string(reason.length()) + "\r\n";
		response += "Connection: close\r\n";
		response += "\r\n";
		response += reason;
		return response;
	}
	std::string response = "HTTP/1.1 500 Internal Server Error\r\n";
	response += "Content-Type: text/html\r\n";
	response += "Content-Length: 21\r\n";
	response += "Connection: close\r\n";
	response += "\r\n";
	response += "Internal Server Error";
	return response;
}

}