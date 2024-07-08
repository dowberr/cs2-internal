#include "../imgui/imgui.h"
#include "Windows.h"
#include "../imgui/imgui_impl_dx11.h"
#include "d3d.h"
#include "Variables.h"
#include "Defines.h"
#include "..//GuiSDK/CheatMenu.h"
#include "..//GuiSDK/Font.h"
#include "../SDK/Entity.h"
#include "..//Cheats/ESP.h"
#include "..//Cheats/AimBot.h"

VOID InitiateImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	CustomFont = io.Fonts->AddFontFromMemoryTTF(ubuntu_1, sizeof(ubuntu_1), 14.5);

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}


LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true; 

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}




HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!Initiated)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			Window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &MainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(Window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			InitiateImGui();
			Initiated = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;

	RenderUI();
	EnableCheats();
	DrawFov();
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &MainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}