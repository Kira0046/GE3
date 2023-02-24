#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>

class Model {
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
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

	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};


public:
	static void StaticInitialize(ID3D12Device* _device);

	static Model* StaticCreateFromOBJ(std::string filePath);
private:
	void LoadModel(std::string filePath);

	void InitializeDesCriptorHeap();

	void LoadTexture(const std::string& directoryPath, const std::string& filename);
	
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:
	static ID3D12Device* device;

public:
	void Initialize(std::string filePath);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	Material GetMaterial() { return material; }

private:
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//マテリアル
	Material material;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	//static VertexPosNormalUv vertices[vertexCount];
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	//static unsigned short indices[planeCount * 3];
	std::vector<unsigned short> indices;
};