#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* _spriteCommon)
{
	HRESULT result{};
	assert(_spriteCommon);
	spriteCommon = _spriteCommon;

	
	vertices[LB] = { {   0.0f, 100.0f, 0.0f },{0.0f,1.0f} };//LB
	vertices[LT] = { {   0.0f,   0.0f, 0.0f },{0.0f,0.0f} };//LT
	vertices[RB] = { { 100.0f, 100.0f, 0.0f },{1.0f,1.0f} };//RB
	vertices[RT] = { { 100.0f,   0.0f, 0.0f },{1.0f,0.0f} };//RT
	

	// 頂点データ
	//XMFLOAT3 vertices[] = {
	//{ -0.3f, -0.5f, 0.0f }, // 左下
	//{ -0.3f, +0.5f, 0.0f }, // 左上
	//{ +0.3f, -0.5f, 0.0f }, // 右下
	////{ +0.5f, -0.5f, 0.0f }, // 右下
	////{ -0.5f, +0.5f, 0.0f }, // 左上
	//{ +0.3f, +0.5f, 0.0f }, // 右上
	//};



	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//生成
	result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//転送
	
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);

	//頂点ビュー
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);


	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GUPへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの作成
	result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	assert(SUCCEEDED(result));

	//値を書き込むと自動的に転送される    ps.ここで色が変わる
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);




	//定数バッファの生成
	
	{
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		//定数バッファのマッピング
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
		assert(SUCCEEDED(result));
		constMapTransform->mat = XMMatrixIdentity();

		//constMapTransform->mat.r[0].m128_f32[0] = 2.0f/1280;
		//constMapTransform->mat.r[1].m128_f32[1] = -2.0f/720;
		//constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
		//constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
	}

	//ワールド
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	rotationZ = 0.0f;
	

	//回転
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;

	//平行移動
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	matWorld *= matTrans;

	//射影変換
	XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, WinApp::window_width,
		WinApp::window_height, 0.0f,
		0.0f, 1.0f
	);

	constMapTransform->mat = matWorld * matProjection;

}

void Sprite::Update()
{
	constMapMaterial->color = color_;

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}


	vertices[LB].pos = { left,   bottom,0.0f };
	vertices[LT].pos = { left,   top,   0.0f };
	vertices[RB].pos = { right,  bottom,0.0f };
	vertices[RT].pos = { right,  top,   0.0f };

	std::copy(std::begin(vertices), std::end(vertices), vertMap);



	//ワールド
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();

	//回転
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotationZ));
	matWorld *= matRot;

	//平行移動
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position_.x, position_.y, 0.0f);
	matWorld *= matTrans;

	//射影変換
	XMMATRIX matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, WinApp::window_width,
		WinApp::window_height, 0.0f,
		0.0f, 1.0f
	);

	constMapTransform->mat = matWorld * matProjection;

}

void Sprite::Draw()
{

	if (isInvisible_) {
		return;
	}

	spriteCommon->SetTextureCommands(textureIndex);

	spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	spriteCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());



	spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
