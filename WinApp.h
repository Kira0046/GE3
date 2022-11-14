#pragma once

#include <Windows.h>

class WinApp
{
public:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wpram, LPARAM lparam);

public:
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();
	//メッセージ処理
	bool ProcessMessage();

	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetHInstance() const { return w.hInstance; }
private:
	HWND hwnd = nullptr;

	WNDCLASSEX w{};
};

