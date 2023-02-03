#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXTex.h>

#include "DIrectXCommon.h"


using namespace DirectX;





class SpriteCommon
{
public:
	void Initialize(DirectXCommon* _dxCommon);

	void PreDraw();

	DirectXCommon* GetDirectXCommon() { return dxCommon; }

private:
	const size_t kMaxSRVCount = 2056;


	DirectXCommon* dxCommon = nullptr;

	ID3D12DescriptorHeap* srvHeap = nullptr;


	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;

	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	const size_t textureWidth = 256;

	const size_t textureHeight = 256;

	
};

