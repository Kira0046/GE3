#pragma once
#include <DirectXMath.h>
#include "SpriteCommon.h"


class Sprite
{

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
public:
	enum VertexNumber {
		LB,
		LT,
		RB,
		RT,
	};

public:
	void Initialize(SpriteCommon* _spriteCommon,uint32_t textureIndex=UINT32_MAX);

	void Update();

	void Draw();

	///
	///
	/// セッター
	void SetPosition(const DirectX::XMFLOAT2& position) { position_ = position; }

	void SetTextureIndex(uint32_t index) { this->textureIndex_ = index; }

	void SetRotation(float rotation) { rotationZ = rotation; }

	void SetColor(const DirectX::XMFLOAT4 color) { color_ = color; }

	void SetSize(const DirectX::XMFLOAT2 size) { size_ = size; }

	void SetAnchor(const DirectX::XMFLOAT2 anchorPoint) { anchorPoint_ = anchorPoint; }

	void SetIsFlipX(const bool isFlipX) { isFlipX_ = isFlipX; }

	void SetIsFlipY(const bool isFlipY) { isFlipY_ = isFlipY; }

	void SetIsInvisible(const bool isInvisible) { isInvisible_ = isInvisible; }

	void SetTextureLeftTop(const DirectX::XMFLOAT2 textureLeftTop) { textureLeftTop_ = textureLeftTop; }

	void SetTextureSize(const DirectX::XMFLOAT2 textureSize) { textureSize_ = textureSize; }

	///
	///
	/// ゲッター
	const DirectX::XMFLOAT2& GetPosition()const { return position_; }

	const uint32_t GetTextureIndex() const { return textureIndex_; }

	float GetRotation() const { return rotationZ; }

	const DirectX::XMFLOAT4& GetColor() const { return color_; }

	const DirectX::XMFLOAT2& GetSize() const { return size_; }

	const DirectX::XMFLOAT2& GetAnchor() const { return anchorPoint_; }

	const bool& GetIsFlipX() const { return isFlipX_; }

	const bool& GetIsFlipY() const { return isFlipY_; }

	const bool& GetInvisible() const { return isInvisible_; }

	const DirectX::XMFLOAT2& GetTextureLeftTop() const { return textureLeftTop_; }

	const DirectX::XMFLOAT2& GetTextureSize() const { return textureSize_; }
private:
	void AdjustTextureSize();


	Vertex vertices[4];

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView{};

	D3D12_RESOURCE_DESC resDesc;

	uint32_t textureIndex_ = 0;

	float rotationZ;

	DirectX::XMFLOAT2 position_ = { 0.0f,0.0f };

	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };

	DirectX::XMFLOAT2 size_ = { 100.0f,100.0f };

	DirectX::XMFLOAT2 anchorPoint_ = { 0.5f,0.5f };

	DirectX::XMFLOAT2 textureLeftTop_ = { 0.0f,0.0f };

	DirectX::XMFLOAT2 textureSize_ = { 100.0f,100.0f };

	bool isFlipX_ = false;
	bool isFlipY_ = false;

	bool isInvisible_ = false;

	SpriteCommon* spriteCommon = nullptr;
	ID3D12Resource* constBuffTransform = nullptr;
	ID3D12Resource* constBuffMaterial = nullptr;
	ConstBufferDataTransform* constMapTransform = nullptr;
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	Vertex* vertMap = nullptr;
};

