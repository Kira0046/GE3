#pragma once

#include <Windows.h>

class WinApp
{
public:
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wpram, LPARAM lparam);

public:
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��
public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�I��
	void Finalize();
	//���b�Z�[�W����
	bool ProcessMessage();

	HWND GetHwnd() const { return hwnd; }

	HINSTANCE GetHInstance() const { return w.hInstance; }
private:
	HWND hwnd = nullptr;

	WNDCLASSEX w{};
};

