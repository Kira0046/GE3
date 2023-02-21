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
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	//�}�e���A��
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

	//�萔�o�b�t�@�p�f�[�^�\��B1
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
/// �f�X�N���v�^�q�[�v�̏�����
/// </summary>
	void InitializeDescriptorHeap();

/// <summary>
/// �}�e���A���ǂݍ���
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>

	void LoadTexture(const std::string& directoryPath, const std::string& filename);


private:


	std::vector<unsigned short> indices;
	std::vector<VertexPosNormalUv> vertices;

	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;

	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	//�}�e���A��
	Material material;


	static ID3D12Device* device;



	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize=0;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffB1;//�萔�o�b�t�@

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;

};