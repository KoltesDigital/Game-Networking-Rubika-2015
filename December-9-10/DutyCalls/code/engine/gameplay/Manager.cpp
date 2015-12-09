#include "engine/gameplay/Manager.h"

#include <cassert>
#include "engine/network/Client.h"
#include "engine/network/Manager.h"
#include "engine/gameplay/entities/Player.h"
#include "engine/gameplay/messages/AssignClientId.h"
#include "engine/gameplay/messages/CreateEntity.h"
#include "engine/gameplay/messages/DestroyEntity.h"
#include "engine/gameplay/messages/UpdateEntity.h"
#include "engine/Engine.h"

namespace engine
{
	namespace gameplay
	{
		Manager *Manager::instance = nullptr;

		void Manager::deinitialize()
		{
			for (const gameplay::Entity *entity : _entities)
				delete entity;
			_entities.clear();
		}

		void Manager::onConnected(network::Client &client)
		{
			if (network::Manager::instance->isServer())
			{
				client.id = util::guidGenerator.newGuid();
				messages::AssignClientId assignClientIdMessage(client.id);
				network::Manager::instance->send(client, assignClientIdMessage, true);

				for (const Entity *entity : _entities)
				{
					messages::CreateEntity createEntityMessage(entity);
					network::Manager::instance->send(client, createEntityMessage, true);
				}

				util::Guid entityId = util::guidGenerator.newGuid();
				createEntity(entities::Player::ENTITY_TYPE_ID, entityId, client.id);
			}
		}

		void Manager::onDisconnected(network::Client &client)
		{
			auto it = _entities.begin();
			while (it != _entities.end())
			{
				const Entity *entity = *it;

				if (entity->ownerId == client.id)
				{
					destroyEntity(&it);
				}
			}
		}
		
		network::data::Container *Manager::createContainer(util::StringId containerId)
		{
			network::data::Container *container = _containerFactory.createContainer(containerId);
			assert(container);
			return container;
		}

		const Entity *Manager::createEntity(util::StringId entityTypeId)
		{
			return createEntity(entityTypeId, util::guidGenerator.newGuid(), network::Manager::instance->localId);
		}

		const Entity *Manager::createEntity(util::StringId entityTypeId, const util::Guid &entityId, const util::Guid &ownerId)
		{
			auto it = _entitiesById.find(entityId);
			if (it != _entitiesById.end())
			{
				// entity exists already
				return it->second;
			}

			Entity *entity = _entityactory.createEntity(entityTypeId);
			assert(entity);
			entity->id = entityId;
			entity->ownerId = ownerId;

			_entitiesById.insert(std::pair<util::Guid, gameplay::Entity*>(entityId, entity));
			_entities.push_back(entity);

			if (network::Manager::instance->isServer() || network::Manager::instance->localId == ownerId)
			{
				messages::CreateEntity createEntityMessage(entity);
				network::Manager::instance->broadcast(createEntityMessage, true);
			}

			return entity;
		}

		void Manager::destroyEntity(const util::Guid &entityId)
		{
			auto it = _entitiesById.find(entityId);
			if (it == _entitiesById.end())
				return;

			Entity *entity = it->second;

			if (network::Manager::instance->isServer() || network::Manager::instance->localId == entity->ownerId)
			{
				messages::DestroyEntity message(entity);
				network::Manager::instance->broadcast(message, true);
			}

			delete entity;
			_entitiesById.erase(it);
			_entities.remove(entity);
		}

		void Manager::destroyEntity(Entities::iterator *listIt)
		{
			assert(listIt);
			const Entity *entity = **listIt;

			if (network::Manager::instance->isServer() || network::Manager::instance->localId == entity->ownerId)
			{
				messages::DestroyEntity message(entity);
				network::Manager::instance->broadcast(message, true);
			}

			delete entity;
			_entitiesById.erase(entity->id);
			*listIt = _entities.erase(*listIt);
		}

		void Manager::updateEntity(const util::Guid &entityId, network::data::Container &container)
		{
			auto it = _entitiesById.find(entityId);
			if (it == _entitiesById.end())
				return;

			Entity *entity = it->second;

			if (network::Manager::instance->isServer() || network::Manager::instance->localId == entity->ownerId)
			{
				messages::UpdateEntity message(entity, container);
				network::Manager::instance->broadcast(message, false);
			}

			entity->readContainer(container);
		}

		void Manager::update()
		{
			for (Entity *entity : _entities)
				entity->update();

			_updateCountdown -= engine::Engine::instance->getDeltaTime();
			if (_updateCountdown <= 0.f)
			{
				_updateCountdown = 0.2f;

				for (Entity *entity : _entities)
				{
					if (entity->ownerId != network::Manager::instance->localId)
						continue;

					if (entity->getEntityTypeId() == entities::Player::ENTITY_TYPE_ID)
					{
						entities::Player::PositionContainer container;
						entity->writeContainer(container);

						messages::UpdateEntity message(entity, container);
						network::Manager::instance->broadcast(message, false);
					}
				}
			}
		}
	}
}
