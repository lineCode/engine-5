#include "LightComponents.h"
#include "../World.h"
#include "../Transform/Transform.h"

namespace Dawn
{
	//MAKE_TYPE_BEGIN(DirectionalLight)
	//	ADD_MEMBER(DirectionalLight, Color, vec4)
	//MAKE_TYPE_END()

	DirectionalLight* LightUtils::CreateDirectionalLight(World* InWorld, quat InOrientation, vec4 InColor)
	{
		EntityId e = InWorld->CreateEntity("Light");
		if (!e.IsValid)
			return nullptr;

		Transform* t = InWorld->MakeComponent<Transform>();
		t->Position = vec3(0, 20, -10);
		t->Rotation = InOrientation;
		t->Scale = vec3(1, 1, 1);
		InWorld->AddComponent<Transform>(e, t);

		DirectionalLight* l = InWorld->MakeComponent<DirectionalLight>();
		l->Color = InColor;
		InWorld->AddComponent<DirectionalLight>(e, l);
		return l;
	}

	PointLight* LightUtils::CreatePointLight(vec3 InPosition, vec4 InColor, float InRange)
	{
		return nullptr;
	}

	SpotLight* LightUtils::CreateSpotLight(vec3 InPosition, quat InOrientation, vec4 InColor, float InAngle, float InRange)
	{
		return nullptr;
	}
}