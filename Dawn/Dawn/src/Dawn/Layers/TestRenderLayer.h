#pragma once

#include "Layer.h"
#include "Core/GDI/inc_gfx_types.h"

namespace Dawn
{
	class GfxCmdList;

	class TestRenderLayer : public Layer
	{
	public:
		void Setup();
		void Update();
		void Render();
		void Process();
		void Free();

	private:
		mat4 View;
		mat4 Model;
		mat4 Perspective;
		float FoV = 45.0f;
		float CamPosition[3] = { 0, 3, 10 };
	};

}