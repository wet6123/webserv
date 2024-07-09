#include "include/common.hpp"

int main()
{
	// Test code
	// description: GET_V_NAME 매크로를 사용하여 WEB_SERVER::Status::BadRequest_400의 이름을 출력한다.
	std::string _Result = GET_V_NAME(WEB_SERVER::Status::BadRequest_400);
	std::cout << _Result << std::endl;

	WEB_SERVER::Type::FD _FD = 10;
	WEB_SERVER::Type::PORT _PORT = "8080";
	std::cout << _FD << std::endl;
	std::cout << _PORT << std::endl;


	WEB_SERVER::Type::Method _Method = WEB_SERVER::Method::GET;
	Type::Method A = Method::GET;

	STRING_UTILS::Split("Hello, World!", ",");
	
	return 0;
}