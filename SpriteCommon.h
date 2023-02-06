#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include <DirectXTex.h>
#include <array>

#include "DIrectXCommon.h"
#include <string>


using namespace DirectX;





class SpriteCommon
{
public:
	void Initialize(DirectXCommon* _dxCommon);

	void PreDraw();

	void LoadTexture(uint32_t index, const std::string& fileName);

	void SetTextureCommands(uint32_t index);

	DirectXCommon* GetDirectXCommon() { return dxCommon; }

	ID3D12Resource* GetTextureBuffer(uint32_t index) const { return texBuff[index].Get(); }

private:
	static const size_t kMaxSRVCount = 2056;

	static std::string kDefailtTextureDirectoryPath;
private:
	DirectXCommon* dxCommon = nullptr;

	//ID3D12DescriptorHeap* srvHeap = nullptr;

	

	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;

	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvHeap;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> texBuff;

	const size_t textureWidth = 256;

	const size_t textureHeight = 256;

	
};

