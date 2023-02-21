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
public:
	/// <summary>
	/// OBJ�ǂݍ���
	/// </summary>
	/// <returns></returns>
	static Model* LoadFromOBJ();

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
private:
	//�f�o�C�X
	static ID3D12Device* device;

	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;

	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;



	Material material;
	void LoadFromOBJInternal();
};