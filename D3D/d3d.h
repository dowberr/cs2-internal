#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>  
#include <iostream>
#include <string>
#include <regex>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline Present oPresent;
inline HWND Window = 0;
inline WNDPROC oWndProc;
inline ID3D11Device* pDevice = 0;
inline ID3D11DeviceContext* pContext = 0;
inline ID3D11RenderTargetView* MainRenderTargetView;

HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID InitiateImGui();


