#pragma once

namespace engine
{
	namespace graphics
	{
		class EventListener
		{
		public:
			virtual void onWindowClosed() = 0;
		};
	}
}
