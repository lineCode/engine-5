#pragma once
#include "inc_common.h"
#include "inc_core.h"
#include "Application.h"
#include "EntryPoint.h"

class SandboxApp : public Dawn::Application
{
public:
	SandboxApp(AppSettings& InSettings);
	~SandboxApp();

	virtual void Load() override;
	virtual void Update(float InDeltaTime) override;
	virtual void FixedUpdate(float InFixedTime) override;
	virtual void Render() override;
	void Resize(int InWidth, int InHeight) override;
	void Cleanup() override;
};

Dawn::Application* Dawn::CreateApplication()
{
	AppSettings Settings = {
		0, // HWND will be provided by the app later on!
		L"Sandbox Game", 
		1280,
		720,
		false, // fullscreen
		false, // vsync 
		32,
		32,
		8
	};

	return new SandboxApp(Settings);
}