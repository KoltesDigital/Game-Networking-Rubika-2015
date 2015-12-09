#pragma once

#include <cassert>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/Http.hpp>

namespace engine
{
	namespace network
	{
		namespace async
		{
			class HTTPTask
			{
			public:
				HTTPTask();

				void update();

				void launch();
				void terminate();

				bool isTerminated();

				virtual void buildRequest(sf::Http::Request &request) = 0;
				virtual void processResponse(sf::Http::Response &response) = 0;

			private:
				enum class State : char
				{
					PENDING_LAUNCH,
					LAUNCHED,
					PENDING_TERMINATE,
					TERMINATED,
				};

				sf::Thread _thread;
				sf::Http::Request _request;
				sf::Http::Response _response;
				volatile State _state;

				void run();
			};
		}
	}
}
