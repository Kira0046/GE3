#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <vector>
#include <chrono>

class DirectXCommon {
public:
//初期化
	void Initialize(WinApp* winApp);
//描画前処理
	void PreDraw();
//描画後処理
	void PostDraw();
//デバイス取得
	ID3D12Device* GetDevice() const { return device.Get(); }
//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
private:
	//デバイス初期化
	void InitializeDevice();
	//コマンド初期化
	void InitializeCommand();
	//スワップチェーン初期化
	void InitializeSwapchain();
	//レンダーターゲットビュー
	void InitializeRenderTargetView();
	//深度バッファ
	void InitializeDepthBuffer();
	//フェンス
	void InitializeFence();
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();

private:
	//WindowAPI
	WinApp* winApp = nullptr;

	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;

	//コマンド
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

	//スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	//レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;

	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	//記録時間
	std::chrono::steady_clock::time_point reference_;
};