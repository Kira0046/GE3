﻿#include "Object3d.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float Object3d::radius = 5.0f;				// 底面の半径
const float Object3d::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* Object3d::device = nullptr;
//UINT Object3d::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Object3d::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3d::rootsignature;
ComPtr<ID3D12PipelineState> Object3d::pipelinestate;
//ComPtr<ID3D12DescriptorHeap> Object3d::descHeap;
//ComPtr<ID3D12Resource> Object3d::vertBuff;
//ComPtr<ID3D12Resource> Object3d::indexBuff;
//ComPtr<ID3D12Resource> Object3d::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE Object3d::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE Object3d::gpuDescHandleSRV;
XMMATRIX Object3d::matView{};
XMMATRIX Object3d::matProjection{};
XMFLOAT3 Object3d::eye = { 0, 0, -50.0f };
XMFLOAT3 Object3d::target = { 0, 0, 0 };
XMFLOAT3 Object3d::up = { 0, 1, 0 };
//D3D12_VERTEX_BUFFER_VIEW Object3d::vbView{};
//D3D12_INDEX_BUFFER_VIEW Object3d::ibView{};
//Object3d::Material Object3d::material;
//Object3d::VertexPosNormalUv Object3d::vertices[vertexCount];
//unsigned short Object3d::indices[planeCount * 3];
//std::vector<Object3d::VertexPosNormalUv> Object3d::vertices;
//std::vector<unsigned short> Object3d::indices;

void Object3d::StaticInitialize(ID3D12Device * device, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);

	Object3d::device = device;
		
	// デスクリプタヒープの初期化
	//InitializeDescriptorHeap();

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

	// テクスチャ読み込み
	//LoadTexture();

	// モデル生成
	CreateModel();

}

void Object3d::PreDraw(ID3D12GraphicsCommandList * cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Object3d::cmdList == nullptr);

	// コマンドリストをセット
	Object3d::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	// コマンドリストを解除
	Object3d::cmdList = nullptr;
}

Object3d * Object3d::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	Object3d* object3d = new Object3d();
	if (object3d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize()) {
		delete object3d;
		assert(0);
		return nullptr;
	}
	
	//スケールをセット
	float scale_val = 20;
	object3d->scale = {
		scale_val,scale_val,scale_val
	};

	return object3d;
}

void Object3d::SetEye(XMFLOAT3 eye)
{
	Object3d::eye = eye;

	UpdateViewMatrix();
}

void Object3d::SetTarget(XMFLOAT3 target)
{
	Object3d::target = target;

	UpdateViewMatrix();
}

void Object3d::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}



void Object3d::InitializeCamera(int window_width, int window_height)
{
	// ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void Object3d::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVertexShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	//CD3DX12_ROOT_PARAMETER rootparams[2];
	//rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}





void Object3d::CreateModel()
{
	HRESULT result = S_FALSE;

	//std::vector<VertexPosNormalUv> realVertices;
	//// 頂点座標の計算（重複あり）
	//{
	//	realVertices.resize((division + 1) * 2);
	//	int index = 0;
	//	float zValue;

	//	// 底面
	//	zValue = prizmHeight / 2.0f;
	//	for (int i = 0; i < division; i++)
	//	{
	//		XMFLOAT3 vertex;
	//		vertex.x = radius * sinf(XM_2PI / division * i);
	//		vertex.y = radius * cosf(XM_2PI / division * i);
	//		vertex.z = zValue;
	//		realVertices[index++].pos = vertex;
	//	}
	//	realVertices[index++].pos = XMFLOAT3(0, 0, zValue);	// 底面の中心点
	//	// 天面
	//	zValue = -prizmHeight / 2.0f;
	//	for (int i = 0; i < division; i++)
	//	{
	//		XMFLOAT3 vertex;
	//		vertex.x = radius * sinf(XM_2PI / division * i);
	//		vertex.y = radius * cosf(XM_2PI / division * i);
	//		vertex.z = zValue;
	//		realVertices[index++].pos = vertex;
	//	}
	//	realVertices[index++].pos = XMFLOAT3(0, 0, zValue);	// 天面の中心点
	//}

	//// 頂点座標の計算（重複なし）
	//{
	//	int index = 0;
	//	// 底面
	//	for (int i = 0; i < division; i++)
	//	{
	//		unsigned short index0 = i + 1;
	//		unsigned short index1 = i;
	//		unsigned short index2 = division;

	//		vertices[index++] = realVertices[index0];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index2]; // 底面の中心点
	//	}
	//	// 底面の最後の三角形の1番目のインデックスを0に書き換え
	//	vertices[index - 3] = realVertices[0];

	//	int topStart = division + 1;
	//	// 天面
	//	for (int i = 0; i < division; i++)
	//	{
	//		unsigned short index0 = topStart + i;
	//		unsigned short index1 = topStart + i + 1;
	//		unsigned short index2 = topStart + division;

	//		vertices[index++] = realVertices[index0];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index2]; // 天面の中心点
	//	}
	//	// 天面の最後の三角形の1番目のインデックスを0に書き換え
	//	vertices[index - 2] = realVertices[topStart];

	//	// 側面
	//	for (int i = 0; i < division; i++)
	//	{
	//		unsigned short index0 = i + 1;
	//		unsigned short index1 = topStart + i + 1;
	//		unsigned short index2 = i;
	//		unsigned short index3 = topStart + i;

	//		if (i == division - 1)
	//		{
	//			index0 = 0;
	//			index1 = topStart;
	//		}

	//		vertices[index++] = realVertices[index0];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index2];

	//		vertices[index++] = realVertices[index2];
	//		vertices[index++] = realVertices[index1];
	//		vertices[index++] = realVertices[index3];
	//	}
	//}

	// 頂点インデックスの設定
	/*{
		for (int i = 0; i < _countof(indices); i++)
		{
			indices[i] = i;
		}
	}*/

	// 法線方向の計算
	//for (int i = 0; i < _countof(indices) / 3; i++)
	//{// 三角形１つごとに計算していく
	//	// 三角形のインデックスを取得
	//	unsigned short index0 = indices[i * 3 + 0];
	//	unsigned short index1 = indices[i * 3 + 1];
	//	unsigned short index2 = indices[i * 3 + 2];
	//	// 三角形を構成する頂点座標をベクトルに代入
	//	XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
	//	XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
	//	XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
	//	// p0→p1ベクトル、p0→p2ベクトルを計算
	//	XMVECTOR v1 = XMVectorSubtract(p1, p0);
	//	XMVECTOR v2 = XMVectorSubtract(p2, p0);
	//	// 外積は両方から垂直なベクトル
	//	XMVECTOR normal = XMVector3Cross(v1, v2);
	//	// 正規化（長さを1にする)
	//	normal = XMVector3Normalize(normal);
	//	// 求めた法線を頂点データに代入
	//	XMStoreFloat3(&vertices[index0].normal, normal);
	//	XMStoreFloat3(&vertices[index1].normal, normal);
	//	XMStoreFloat3(&vertices[index2].normal, normal);
	//}





	







	//UINT sizeVB = static_cast<UINT>(sizeof(vertices));
	



	
	


	

	

	
	

	

	



	//UINT sizeIB = static_cast<UINT>(sizeof(indices));

	
}

void Object3d::UpdateViewMatrix()
{
	// ビュー行列の更新
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

bool Object3d::Initialize()
{
	// nullptrチェック
	assert(device);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));

	//resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);

	return true;
}

void Object3d::Update()
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	matWorld = XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	constMap->mat = matWorld * matView * matProjection;	// 行列の合成
	constBuffB0->Unmap(0, nullptr);


}

void Object3d::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Object3d::cmdList);
		
	if (model_ == nullptr)return;




	// 定数バッファビューをセット
	//cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	

	model_->Draw(cmdList, 1);
}
