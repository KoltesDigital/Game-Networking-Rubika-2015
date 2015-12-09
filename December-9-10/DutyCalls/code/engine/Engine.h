#pragma once

#include "engine/graphics/EventListener.h"
#include "engine/util/Guid.h"

namespace engine
{
	class Engine : public graphics::EventListener
	{
	public:
		enum class InstanceType
		{
			CLIENT,
			SERVER,
		};

		util::Guid clientId;

		bool initialize();
		void deinitialize();

		bool createClient(const std::string &host, sf::Uint16 port);
		bool createServer(sf::Uint16 port);

		void run();
		float getDeltaTime() const;

		// graphics::EventListener
		virtual void onWindowClosed();

		static Engine *instance;

	private:
		InstanceType _instanceType;
		bool _running;
		float _deltaTime;
	};
}
