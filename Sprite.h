#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class Sprite
{
public:
	void Initialize(SpriteCommon* _spriteCommon);

	void Draw();

	D3D12_RESOURCE_DESC* GetResDesc() { resDesc; }
private:
	SpriteCommon* spriteCommon = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc;
};

