#pragma once

#include <string>

namespace engine
{
	namespace network
	{
		namespace async
		{
			enum class Error
			{
				INVALID_RESPONSE,
				SERVER_DOWN,
				CONNECTION_FAILED,
			};

			const std::string &getErrorMessage(Error error);
		}
	}
}
