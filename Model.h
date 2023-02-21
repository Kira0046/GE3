#pragma once

#include <string>
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

using namespace std;
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
public:
	/// <summary>
	/// OBJ読み込み
	/// </summary>
	/// <returns></returns>
	static Model* LoadFromOBJ();

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
private:
	//デバイス
	static ID3D12Device* device;

	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;



	Material material;
	void LoadFromOBJInternal();
};