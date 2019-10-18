#pragma once
#include "../GfxGDI.h"

//#ifdef USE_OPENGL_GFX
#include "inc_opengl.h"
#include "Buffer.h"

namespace Dawn
{
	class DAWN_API GfxGLGDI : public GfxGDI
	{
	public:
		bool Init(const AppSettings& InSettings);
		void Present();
		void Shutdown();
	public:
		void ActivateTextureSlot(u32 InIndex) override;
		void DrawIndexed(GfxResId VertexArrayId) override;
		void DrawArray(GfxResId VertexArrayId) override;
		void DrawInstanced(GfxResId VertexArrayId, u32 InAmount) override;
		void Clear() override;
		void SetViewport(u32 InLeft, u32 InTop, u32 InRight, u32 InBottom) override;

	public:
		GfxResId CreateVertexBuffer(void* Vertices, u32 Size, GfxVertexBuffer** OutBuffer) override;
		GfxResId CreateIndexBuffer(u32* Indices, u32 Size, GfxIndexBuffer** OutBuffer) override;
		GfxResId CreateVertexArray(GfxVertexArray** OutBuffer) override;
		GfxResId CreateShader(GfxShader** OutShader) override;
		GfxResId CreateTexture(u8* Data, u32 Width, u32 Height, u16 ChannelsPerPixel, GfxWrapDesc Wrap,
			GfxFilterDesc Filter, bool GenerateMipMaps, GfxTexture** OutTexture) override;
		GfxResId CreateRenderBuffer(GfxRenderBuffer** OutTexture) override;

		// todo-- move this back to protected (gb, 10/13)
		HGLRC hRC;
	protected:
		HWND HwnD;
		HDC hDC;
	};
}
//#endif