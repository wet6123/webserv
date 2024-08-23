#include "../inc/ErrorResponse.hpp"

namespace ErrorResponse {
	/** 
	 * @brief 에러 응답을 반환합니다.
	 * @param status 상태 코드
	 * @return std::string 에러 응답
	*/
	std::string getErrorResponse(Status status, PORT port) {
		Response response;
		Server tmp = Config::getServer(port);

		std::string errorPath = tmp.getRootPath() + tmp.get404Path();
		if (String::Itos(status) == errorPath.substr(errorPath.find_last_of('/') + 1, errorPath.find_last_of('.') - errorPath.find_last_of('/') - 1)) {
			
			std::ifstream file(errorPath);
			if (file.is_open()) {
				response.setStatusCode(status);
				response.setHeader("Content-Type", "text/html");
				response.setHeader("Connection", "close");
				String::BinaryBuffer buffer;
				buffer.resize(FileSystem::GetFileSize(file));
				file.read(&buffer[0], buffer.size());
				response.setBody(buffer.str());
				response.setHeader("Content-Length", String::Itos(buffer.size()));
				file.close();
				return response.getResponses().str();
			}
		}
		
		std::map<Status, std::string>::iterator it = ResponseHandle::Utils::REASON_PHRASE.find(status);

		if (it != ResponseHandle::Utils::REASON_PHRASE.end()) {
			response.setStatusCode(status);
			response.setHeader("Content-Type", "text/html");
			response.setHeader("Connection", "close");
			std::string errorBody = "<html><body><h1>" + String::Itos(status) + " " + ResponseHandle::Utils::REASON_PHRASE[status] + "</h1><p>" + ResponseHandle::Utils::REASON_PHRASE[status] + "</p></body></html>";
			response.setBody(errorBody);
			response.setHeader("Content-Length", String::Itos(errorBody.length()));
			return response.getResponses().str();
		}
		response.setStatusCode(InternalServerError_500);
		response.setHeader("Content-Type", "text/html");
		response.setHeader("Connection", "close");
		std::string errorBody = "<html><body><h1>" + String::Itos(InternalServerError_500) + " " + ResponseHandle::Utils::REASON_PHRASE[InternalServerError_500] + "</h1><p>" + ResponseHandle::Utils::REASON_PHRASE[InternalServerError_500] + "</p></body></html>";
		response.setBody(errorBody);
		response.setHeader("Content-Length", String::Itos(errorBody.length()));
		return response.getResponses().str();
	}
}