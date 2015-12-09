#pragma once

#include <list>
#include <map>
#include "engine/util/Guid.h"
#include "engine/util/StringId.h"
#include "engine/network/ContainerFactory.h"
#include "engine/network/EventListener.h"
#include "engine/gameplay/Entity.h"
#include "engine/gameplay/EntityFactory.h"

namespace engine
{
	namespace gameplay
	{
		class Manager : public network::EventListener
		{
		public:
			typedef std::map<util::Guid, gameplay::Entity*> EntitiesById;
			typedef std::list<gameplay::Entity*> Entities;

			void deinitialize();

			// network::EventListener
			virtual void onConnected(network::Client &client);
			virtual void onDisconnected(network::Client &client);

			network::data::Container *createContainer(util::StringId containerId);
			
			const Entity *createEntity(util::StringId entityTypeId);
			const Entity *createEntity(util::StringId entityTypeId, const util::Guid &entityId, const util::Guid &ownerId);

			void destroyEntity(const util::Guid &entityId);
			void destroyEntity(Entities::iterator *listIt);

			void updateEntity(const util::Guid &entityId, network::data::Container &container);

			void update();

			static Manager *instance;

		private:
			EntitiesById _entitiesById;
			Entities _entities;
			EntityFactory _entityactory;
			network::ContainerFactory _containerFactory;
			float _updateCountdown;
		};
	}
}
