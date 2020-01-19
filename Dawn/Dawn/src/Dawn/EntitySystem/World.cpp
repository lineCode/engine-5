#include "stdafx.h"
#include "World.h"
#include "Camera/Camera.h"
#include "Transform/Transform.h"
#include "Lights/LightComponents.h"
#include "StaticMesh/ModelView.h"
#include "ResourceSystem/Resources.h"
#include "ResourceSystem/ResourceSystem.h"
#include "Application.h"
#include "EntityTable.h"

namespace Dawn
{
	Camera* World::ActiveCamera = nullptr;

	World::World()
		: Entities(this)
	{
	}

	World::~World()
	{
	}

	void World::UpdateSystems()
	{
		for (auto pair : SystemTable)
		{
			pair.second->Update(this);
		}
	}

	void World::Shutdown()
	{
		for (auto pair : SystemTable)
			SafeDelete(pair.second);

		ActiveCamera = nullptr;
		Entities.Clear();
		InitFuncTable.clear();
	//	Entities.Clear();
		SystemTable.clear();
	}

	//
	// This function creates a camera entity that is then moved into 
	// a list to save the entity id.
	//
	Camera* World::CreateCamera(std::string& InName, u32 Width, u32 Height,
		float InNearZ, float InFarZ, float InFoV, vec4 InClearColor, 
		const vec3& InPosition, const quat& InOrientation)
	{
		Entity e = CreateEntity(InName);
		if (!e.IsValid())
			return nullptr;

		Transform* t = this->MakeComponent<Transform>();
		t->Position = InPosition;
		t->Rotation = InOrientation;
		t->Scale = vec3(1, 1, 1);
		AddComponent<Transform>(e, t);

		Camera* c = this->MakeComponent<Camera>();
		c->AspectRatio = (float)Width / (float)Height;
		c->Height = Height;
		c->Width = Width;
		c->NearZ = InNearZ;
		c->FarZ = InFarZ;
		c->FieldOfView = InFoV;
		c->ClearColor = InClearColor;
		c->WorldUp = vec3(0, 1, 0);

		auto componentId = AddComponent<Camera>(e, c);
		if (!componentId.IsValid)
			return nullptr;

		CameraEntities.emplace_back(e.Id);
		return c;
	}

	i32 World::CreateModelEntity(std::string& InName, std::string& InModelName, vec3& InPosition, vec3& InScale, quat& InRotation)
	{
		Entity e = CreateEntity(InName);
		if (!e.IsValid())
			return -1;

		auto rs = g_Application->GetResourceSystem();

		Transform* t = this->MakeComponent<Transform>();
		t->Position = InPosition;
		t->Rotation = InRotation;
		t->Scale = InScale;
		AddComponent<Transform>(e, t);

		ModelView* mv = this->MakeComponent<ModelView>();
		mv->ModelId = rs->GetFileId(InModelName);
		mv->ResourceId = rs->LoadFile(mv->ModelId);
		AddComponent<ModelView>(e, mv);

		return e.Id;
	}

	void World::AddCamera(Camera* Cam)
	{
		// todo (gb, 01/11/20) - add checks if camera is already added!
		auto entity = Cam->GetEntity();
		CameraEntities.push_back(entity.Id);
	}

	//
	// Returns the camera by a given index (0-last camera)
	//
	Camera* World::GetCamera(const i32 InId)
	{
		if (CameraEntities.size() == 0)
			return nullptr;

		i32 e = CameraEntities[InId];
		Entity entity = Entities.Get(e);

		if (!entity.IsValid())
			return nullptr;

		return this->GetComponentByEntity<Camera>(entity);
	}

	std::vector<Camera*> World::GetCameras()
	{ 
		std::vector<Camera*> Cams;

		for (auto CamId : CameraEntities)
		{
			Entity entity = Entities.Get(CamId);
			auto CamRef = GetComponentByEntity<Camera>(entity);
			if (CamRef)
				Cams.push_back(CamRef);
		}

		return Cams;
	}

	//
	// Creates a new Entity with the given name and returns it's 
	// entity handle.
	//
	Entity World::CreateEntity(const std::string &InName)
	{
		return Entities.Create(InName);
	}

	//
	// Destroys the given entity with all it's components!
	//
	void World::DestroyEntity(const Entity& InEntity)
	{
		auto components = GetComponentTypesByEntity(InEntity);
		
		for (auto component : components)
		{
			GetTableByString(component)->Destroy(InEntity);
		}

		Entities.Remove(InEntity.Id);
	}

	//
	// Returns all the component type names for a specific 
	// entity! This is mainly used for editor handling.
	//
	std::vector<std::string> World::GetComponentTypesByEntity(const Entity& InEntity)
	{
		std::vector<std::string> components;

		for (int i = 0; i < ComponentTables.size(); ++i)
		{
			// once we hit the first empty table pointer
			// we just break since there won't be anymore tables ahead.
			if (ComponentTables[i] == nullptr)
				break;

			if (ComponentTables[i]->ComponentForEntityExists(InEntity))
				components.emplace_back(ComponentTables[i]->GetTypeName());
		}

		return components;
	}

	
	struct ComponentInitData
	{
		std::string Type;
		void* Payload;
	};

	bool World::LoadLevel(World* InWorld, Level* InLevel)
	{
		if (!InLevel->Id.IsValid)
			return false;


		// unloading should be done here

		std::vector<ComponentInitData> compToInit;

		for (i32 i = 0; i < InLevel->Entities.size(); ++i)
		{
			auto EntityData = &InLevel->Entities[i];
			auto Entity = InWorld->CreateEntity(EntityData->Name);

			for (i32 x = 0; x < EntityData->IdToComponent.size(); ++x)
			{
				u32 id = EntityData->IdToComponent[x];
				auto CompData = &InLevel->Components[id];
				auto Type = DawnType::GetType(CompData->Type);
				if (Type != nullptr)
				{
					auto Component = Type->CreateInstance();
					auto CompId = InWorld->AddComponentByString(Entity, CompData->Type, Component);

					for(auto& CompVal : CompData->ComponentValues)
						Type->DeserializeMember(Component, CompVal.VariableName, CompVal.Data);

					ComponentInitData data;
					data.Type = CompData->Type;
					data.Payload = Component;
					compToInit.push_back(data);
				}
			}
		}

		for (auto data : compToInit)
		{
			InWorld->ExecuteComponentInitFunc(data.Payload, data.Type);
		}


		return true;
	}

	void*  World::GetComponentByName(const Entity& InEntity, const std::string& InName)
	{
		BaseComponentTable* ComponentTable = nullptr;

		for (auto& table : ComponentTables)
		{
			if (table->GetTypeName() == InName)
			{
				ComponentTable = table.get();
				break;
			}
		}

		return ComponentTable->GetComponentByEntity(InEntity);
		//it->second->
	}

	void World::AddSystem(ISystem* InSystem)
	{
		auto type = InSystem->AccessType();
		auto it = SystemTable.find(type->GetName());
		if (it != SystemTable.end())
			return;

		SystemTable.insert(std::make_pair(type->GetName(), InSystem));
	}

	std::array<Entity, MaxNumbersOfEntities>&  World::GetEntities(u32* OutCount)
	{
		return Entities.GetEntities(OutCount);
	}

	Camera* World::GetActiveCamera()
	{
		return ActiveCamera;
	}

	void World::SetActiveCamera(Camera* InCamera)
	{
		ActiveCamera = InCamera;
	}

	BaseComponentTable* World::GetTableByString(const std::string& InName)
	{
		for (auto& Table : ComponentTables)
			if (Table->GetTypeName() == InName)
				return Table.get();

		return nullptr;
	}

	EntityMetaData* World::GetEntityMetaData(const Entity& InEntity)
	{
		return Entities.GetMeta(InEntity);
	}

	void World::ExecuteComponentInitFunc(void* InComponent, const std::string& InString)
	{
		auto it = this->InitFuncTable.find(InString);
		if (it != InitFuncTable.end())
			(it->second)(this, InComponent);
	}
}