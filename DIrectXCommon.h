#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <vector>
#include <chrono>

class DirectXCommon {
public:
//������
	void Initialize(WinApp* winApp);
//�`��O����
	void PreDraw();
//�`��㏈��
	void PostDraw();
//�f�o�C�X�擾
	ID3D12Device* GetDevice() const { return device.Get(); }
//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
private:
	//�f�o�C�X������
	void InitializeDevice();
	//�R�}���h������
	void InitializeCommand();
	//�X���b�v�`�F�[��������
	void InitializeSwapchain();
	//�����_�[�^�[�Q�b�g�r���[
	void InitializeRenderTargetView();
	//�[�x�o�b�t�@
	void InitializeDepthBuffer();
	//�t�F���X
	void InitializeFence();
	//FPS�Œ菉����
	void InitializeFixFPS();
	//FPS�Œ�X�V
	void UpdateFixFPS();

private:
	//WindowAPI
	WinApp* winApp = nullptr;

	//�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	//�R�}���h
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	//�X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//�����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	//�[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	//�t�F���X
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	//�L�^����
	std::chrono::steady_clock::time_point reference_;
};