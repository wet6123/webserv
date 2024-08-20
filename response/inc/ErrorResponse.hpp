#ifndef ERRORRESPONSE_HPP
#define ERRORRESPONSE_HPP

#include "../../common/Define.hpp"
#include "ResponseUtils.hpp"

namespace ErrorResponse {
	std::string getErrorResponse(Status status);
}

#endif	// ERRORRESPONSE_HPP