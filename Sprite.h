#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"

struct ConstBufferDataMaterial
{
	XMFLOAT4 color;
};

struct ConstBufferDataTransform {
	XMMATRIX mat;
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	void Initialize(SpriteCommon* _spriteCommon);

	void Update();

	void Draw();
private:
	SpriteCommon* spriteCommon = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;
	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12DescriptorHeap* srvHeap = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc;

	const size_t kMaxSRVCount = 2056;

	float rotationZ;

	DirectX::XMFLOAT3 position;
};

