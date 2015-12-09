#pragma once

#include "engine/network/async/HTTPTask.h"
#include "engine/network/async/Promise.h"

namespace engine
{
	namespace network
	{
		namespace async
		{
			template<typename... Args>
			class PromiseHTTPTask : public HTTPTask
			{
			public:
				Promise<Args...> promise;

				// check for 5xx and connection errors and resolve error
				bool checkErrors(sf::Http::Response &response)
				{
					auto status = response.getStatus();
					// std::cout << status << ": " << response.getBody() << std::endl;

					if (status >= 500 && status < 600)
					{
						promise.resolveError(Error::SERVER_DOWN);
						return true;
					}

					if (status == sf::Http::Response::Status::InvalidResponse)
					{
						promise.resolveError(Error::INVALID_RESPONSE);
						return true;
					}

					if (status == sf::Http::Response::Status::ConnectionFailed)
					{
						promise.resolveError(Error::CONNECTION_FAILED);
						return true;
					}

					return false;
				}
			};
		}
	}
}
