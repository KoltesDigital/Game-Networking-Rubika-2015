#include "engine/network/async/HTTPTask.h"
#include "engine/network/Manager.h"

namespace engine
{
	namespace network
	{
		namespace async
		{
			HTTPTask::HTTPTask()
				: _thread(&HTTPTask::run, this)
				, _state(State::PENDING_LAUNCH)
			{
				// empty
			}

			void HTTPTask::update()
			{
				switch (_state)
				{
				case State::PENDING_LAUNCH:
					_state = State::LAUNCHED;
					buildRequest(_request);
					_thread.launch();
					break;

				case State::PENDING_TERMINATE:
					_state = State::TERMINATED;
					processResponse(_response);
					break;
				}
			}

			void HTTPTask::launch()
			{
				_thread.launch();
			}

			void HTTPTask::terminate()
			{
				_thread.terminate();
				_state = State::TERMINATED;
			}

			bool HTTPTask::isTerminated()
			{
				return _state == State::TERMINATED;
			}

			void HTTPTask::run()
			{
				_response = network::Manager::instance->httpClient.sendRequest(_request);
				if (_state == State::LAUNCHED)
					_state = State::PENDING_TERMINATE;
			}
		}
	}
}
