#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "WinApp.h"

class Input
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	///<param name="keyNumber">
	///<returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);
private:
	 ComPtr<IDirectInputDevice8> keyboard;

	 //今回のキー状態
	 BYTE key[256] = {};
	//前回のキー状態
	 BYTE keyPre[256] = {};

	 ComPtr<IDirectInput8> directInput;

	 //WinAPIポインタ 
	 WinApp* winApp = nullptr;
};

