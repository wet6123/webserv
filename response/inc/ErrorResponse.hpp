#ifndef ERRORRESPONSE_HPP
#define ERRORRESPONSE_HPP

#include "../../common/Define.hpp"
#include "../../common/Config.hpp"
#include "../../common/FileSystem.hpp"
#include "ResponseUtils.hpp"
#include "Response.hpp"

namespace ErrorResponse {
	std::string getErrorResponse(Status status, PORT port);
}

#endif	// ERRORRESPONSE_HPP