#include <catch/catch.hpp>

#include <SFML/Network/Packet.hpp>
#include "engine/network/data/Container.h"
#include "engine/network/data/Number.h"
#include "engine/network/Protocol.h"

class TestContainer : public engine::network::data::Container
{
public:
	TestContainer()
		: _value(0)
	{
		// empty
	}

	TestContainer(sf::Int32 value)
		: _value(value)
	{
		// empty
	}

	sf::Int32 getValue() const
	{
		return _value;
	}

	virtual engine::util::StringId getContainerId() const override
	{
		return engine::util::StringId("TestContainer");
	}

	virtual void declare(engine::network::Protocol &protocol) override
	{
		protocol.declare(_value);
	}

private:
	engine::network::data::Number<sf::Int32> _value;
};

TEST_CASE("Protocol")
{
	TestContainer senderContainer(42);
	REQUIRE(senderContainer.getValue() == 42);

	engine::network::Protocol senderProtocol;
	senderContainer.declare(senderProtocol);

	sf::Packet packet;
	senderProtocol.serialize(packet);

	// packet travels over network

	TestContainer receiverContainer;
	REQUIRE(receiverContainer.getValue() == 0);

	engine::network::Protocol receiverProtocol;
	receiverContainer.declare(receiverProtocol);
	receiverProtocol.deserialize(packet);
	REQUIRE(receiverContainer.getValue() == 42);
}
