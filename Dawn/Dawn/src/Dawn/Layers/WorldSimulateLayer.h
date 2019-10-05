#pragma once

#include "inc_common.h"
#include "Layer.h"

#include "EntitySystem/PhysicsWorld.h"
#include "EntitySystem/World.h"

namespace Dawn
{
	class WorldSimulateLayer : public Layer
	{
	public:
		WorldSimulateLayer();
		~WorldSimulateLayer();

		virtual void Setup();
		virtual void Process();
		virtual void Free();

	private:
		RefPtr<PhysicsWorld> Physics;
		RefPtr<World> World;
	};
}