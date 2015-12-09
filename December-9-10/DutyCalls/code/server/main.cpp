#include "engine/Engine.h"

int main()
{
	engine::Engine::instance = new engine::Engine();

	if (!engine::Engine::instance->initialize())
		return EXIT_FAILURE;

	if (!engine::Engine::instance->createServer(4242))
		return EXIT_FAILURE;

	engine::Engine::instance->run();

	engine::Engine::instance->deinitialize();
	delete engine::Engine::instance;
	return EXIT_SUCCESS;
}
