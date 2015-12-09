#include "engine/network/async/Error.h"

namespace engine
{
	namespace network
	{
		namespace async
		{
			static const std::string InvalidResponseMessage("Invalid response");
			static const std::string ServerDownMessage("Server down");
			static const std::string ConnectionFailedMessage("Connection failed");
			static const std::string UnknownErrorMessage("???");

			const std::string &getErrorMessage(Error error)
			{
				switch (error)
				{
				case Error::INVALID_RESPONSE:
					return InvalidResponseMessage;
				case Error::SERVER_DOWN:
					return ServerDownMessage;
				case Error::CONNECTION_FAILED:
					return ConnectionFailedMessage;
				}
				return UnknownErrorMessage;
			}
		}
	}
}
