#pragma once
#include <string>
#include <DirectXMath.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <d3dx12.h>
#include <wrl.h>



using namespace DirectX;

class Model {

public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	//マテリアル
	struct Material
	{
		std::string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;

		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//定数バッファ用データ構造B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public:
	static Model* LoadFromOBJ(ID3D12Device* device);

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
private:
	void LoadFromOBJInternal();

	void CreateBuffers();

/// <summary>
/// デスクリプタヒープの初期化
/// </summary>
	void InitializeDescriptorHeap();

/// <summary>
/// マテリアル読み込み
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>

	void LoadTexture(const std::string& directoryPath, const std::string& filename);


private:


	std::vector<unsigned short> indices;
	std::vector<VertexPosNormalUv> vertices;

	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	//マテリアル
	Material material;


	static ID3D12Device* device;



	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize=0;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffB1;//定数バッファ

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;

};