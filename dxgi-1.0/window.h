#pragma once

#include <cstdio>
#include <Windows.h>

constexpr auto SANDBOX_WINDOW_CLASS= L"dxgi-sandbox";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

class Window final
{
public:
	Window(UINT width, UINT height) {
		// ensure that the window class gets registered.
		static auto registered = false;
		if (!registered) {
			WNDCLASSEX wc = {};
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.lpfnWndProc = WndProc;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = SANDBOX_WINDOW_CLASS;
			wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
			if (RegisterClassEx(&wc) == 0) {
				printf("RegisterClassEx: %ld\n", GetLastError());
				exit(-1);
			}
		}

		// create a new window instance.
		mHWND = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			SANDBOX_WINDOW_CLASS,
			L"DXGI - Sandbox",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL, NULL, GetModuleHandle(nullptr), NULL);
		if (mHWND == NULL) {
			printf("CreateWindowEx: %ld\n", GetLastError());
			exit(-1);
		}
		// show and enable window to be used.
		ShowWindow(mHWND, SW_SHOW);
		UpdateWindow(mHWND);
	}

	~Window() {
		DestroyWindow(mHWND);
	}

	HWND hwnd() const { return mHWND; }
private:
	HWND mHWND;
};
