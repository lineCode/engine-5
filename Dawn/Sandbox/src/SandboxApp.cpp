#include "SandboxApp.h"
#include "EntitySystem/Camera/Camera.h"
#include "Rendering/RenderResourceHelper.h"
#include "ResourceSystem/Resources.h"
#include "Core/GDI/inc_gfx.h"
#include "Vendor/ImGui/ImGuiWrapper.h"
#include "UI/Editor/imgui_editor_functions.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderBucket.h"

using namespace Dawn;

SandboxApp::SandboxApp(AppSettings& InSettings)
	: Application(InSettings)
	 
{
}


SandboxApp::~SandboxApp()
{
}

// note-- these will be removed once we get a better structure into the "game"
Camera* g_Camera;
GfxRenderBuffer* g_RenderBuffer;
GfxVertexArray* g_ScreenQuadVAO;

void SandboxApp::Load()
{
	RenderResourceHelper::LoadCommonShaders(ResourceSystem.get());

	g_Camera = CreateCamera(GetWorld().get(),
		"EditorCam",
		Settings.Width,
		Settings.Height,
		0.1f, 10000.0f, 65.0f,
		vec4(0.4f, 0.6f, 0.9f, 1.0f),
		vec3(0, 3, 10)
	);
	g_Camera->GetEntity()->bIsHiddenInEditorHierarchy = false;
	CameraUtils::CalculatePerspective(g_Camera);

	auto Cam1 = CreateCamera(GetWorld().get(),
		"Cam1",
		Settings.Width,
		Settings.Height,
		0.0f, 10000.0f, 65.0f,
		vec4(0.4f, 0.6f, 0.9f, 1.0f),
		vec3(0, 0, 0),
		quat(1, 0, 0, 0)
	);

	Cam1->bIsOrthographic = true;
	CameraUtils::CalculateOthographic(Cam1);

	auto Id = ResourceSystem->LoadFile("Textures/grid.png");
	if (auto GridImage = ResourceSystem->FindImage(Id))
	{
		GDI->GetPrimitiveHelper()->AllocateBuffers
		(
			GridImage,
			ResourceSystem->FindShader(CommonShaderHandles::DebugPrim),
			ResourceSystem->FindShader(EditorShaderHandles::Grid)
		);
	}

	auto IdBuffer = GDI->CreateRenderBuffer(&g_RenderBuffer);
	if (IdBuffer.IsValid)
	{
		g_RenderBuffer->AttachColorTarget(0, Settings.Width, Settings.Height);
		g_RenderBuffer->AttachDepthStencilTarget(Settings.Width, Settings.Height);
	}

	g_ScreenQuadVAO = GfxPrimitiveFactory::AllocateQuad(GDI.get(), vec2(1.0f, -1.0f), 1.0f);

	ImGuiWrapper::Create(this->Settings.Hwnd, this->GetGDI().get());

	
}

void SandboxApp::Resize(int InWidth, int InHeight)
{
	Application::Resize(InWidth, InHeight);

	if (g_RenderBuffer)
	{
		g_RenderBuffer->AttachColorTarget(0, InWidth, InHeight);
		g_RenderBuffer->AttachDepthStencilTarget(InWidth, InHeight);
	}
}

void SandboxApp::Update(float InDeltaTime)
{
	for (Layer* layer : Layers)
	{
		layer->Update(InDeltaTime);
	}
}

void SandboxApp::FixedUpdate(float InFixedTime)
{
	
}

void SandboxApp::Render()
{
}

void SandboxApp::Cleanup()
{
	ImGuiWrapper::Shutdown();
}