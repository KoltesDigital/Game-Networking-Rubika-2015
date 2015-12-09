#pragma once

#include <functional>
#include <list>
#include "engine/network/async/Error.h"

namespace engine
{
	namespace network
	{
		namespace async
		{
			template<typename... Args>
			class Promise
			{
			public:
				typedef std::function<void(Args...)> SuccessCallback;
				typedef std::function<void(Error)> ErrorCallback;

				void onSuccess(SuccessCallback callback)
				{
					_successCallbacks.push_back(callback);
				}

				void onError(ErrorCallback callback)
				{
					_errorCallbacks.push_back(callback);
				}

				void resolveSuccess(Args... arguments) const
				{
					for (SuccessCallback callback : _successCallbacks)
					{
						callback(arguments...);
					}
				}

				void resolveError(Error error) const
				{
					for (ErrorCallback callback : _errorCallbacks)
					{
						callback(error);
					}
				}

			private:
				std::list<SuccessCallback> _successCallbacks;
				std::list<ErrorCallback> _errorCallbacks;
			};
		}
	}
}