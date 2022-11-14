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
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	///<param name="keyNumber">
	///<returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);
private:
	 ComPtr<IDirectInputDevice8> keyboard;

	 //����̃L�[���
	 BYTE key[256] = {};
	//�O��̃L�[���
	 BYTE keyPre[256] = {};

	 ComPtr<IDirectInput8> directInput;

	 //WinAPI�|�C���^ 
	 WinApp* winApp = nullptr;
};

