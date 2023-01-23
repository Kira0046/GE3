#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

void Sprite::Initialize(SpriteCommon* _spriteCommon)
{
	HRESULT result{};
	assert(_spriteCommon);
	spriteCommon = _spriteCommon;

	// ���_�f�[�^
	//XMFLOAT3 vertices[] = {
	//{ -0.3f, -0.5f, 0.0f }, // ����
	//{ -0.3f, +0.5f, 0.0f }, // ����
	//{ +0.3f, -0.5f, 0.0f }, // �E��
	////{ +0.5f, -0.5f, 0.0f }, // �E��
	////{ -0.5f, +0.5f, 0.0f }, // ����
	//{ +0.3f, +0.5f, 0.0f }, // �E��
	//};

	Vertex vertices[]={
		{{ -0.3f, -0.5f, 0.0f },{0.0f,1.0f}},//
		{{ -0.3f, +0.5f, 0.0f },{0.0f,0.0f}},//
		{{ +0.3f, -0.5f, 0.0f },{1.0f,1.0f}},//
		{{ +0.3f, +0.5f, 0.0f },{1.0f,0.0f}},//
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//����
	result = spriteCommon->GetDirectXCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//�]��
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	//���_�r���[
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

}

void Sprite::Draw()
{

	spriteCommon->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	spriteCommon->GetDirectXCommon()->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
