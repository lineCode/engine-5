#pragma once
//---------------------------------------------------------------------------//
//        Copyright 2016  Immersive Pixels. All Rights Reserved.			 //

//-----------------------------------------------------------------------------
// Inclusions
//-----------------------------------------------------------------------------
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3dcompiler.lib")

//-----------------------------------------------------------------------------
// Inclusions
//-----------------------------------------------------------------------------
#include "inc_gfx.h"
#include "inc_common.h"
//#include "GfxQueue.h"

namespace Dawn 
{
	class GfxDevice;
	class GfxQueue;
	class GfxDescriptorAllocation;
	class GfxCmdList;
	class GfxTexture;
	class GfxRenderTarget;

	namespace GfxBackend 
	{
		static const u32 g_NumFrames = 3;

		// Gfx Context Management Methods
		EResult Initialize(int InWidth, int InHeight, HWND InHWnd, bool InUseVSync, bool InIsFullscreen);
		bool IsInitialized();

		void Shutdown();
	
		ComPtr<ID3D12Device2> GetDevice();
		ComPtr<IDXGISwapChain3> GetSwapChain();

		// Display Methods
		D3D12_VIEWPORT GetViewport();
		D3D12_RECT  GetScissorRect();

		// Rendering methods
		u32 Present(const GfxTexture& InTexture);
		void Flush();
		void Resize(u32 InWidth, u32 InHeight);
		const GfxRenderTarget& GetRenderTarget();
		DXGI_SAMPLE_DESC  GetMultisampleQualityLevels(DXGI_FORMAT InBackBufferFormat, u32 InNumSamples, D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS InFlags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE);

		// Shader methods
		HRESULT CompileShader(LPCWSTR InSrcFile, LPCSTR InEntryPoint, LPCSTR InProfile, ID3DBlob** OutBlob);
		HRESULT ReadShader(LPCWSTR InSrcFile, ID3DBlob** OutBlob);

		// Resource methods
		GfxDescriptorAllocation AllocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE InType, u32 InDescriptors = 1);
		u32 GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type);
		void ReleaseStaleDescriptors(uint64_t finishedFrame);
		std::shared_ptr<GfxQueue> GetQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);
	
	}
};