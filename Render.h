#pragma once
#include <iostream>
#include <d3d12.h>
#include <dxgi.h>
#include "menu.h"
#include <dxgi1_4.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#pragma comment(lib, "D3d12.lib")

#define OFFSET_PRESENT 0x1120

struct IDXGISwapChain3;

struct ID3D12Device;

struct ID3D11DeviceContext;

typedef void* HANDLE;

typedef unsigned __int64  uintptr_t;


namespace Globals {

	extern IDXGISwapChain3* pSwapChain;
	extern ID3D12Device* pD3DDevice;
	extern ID3D12CommandQueue* D3D12CommandQueue;
	extern HANDLE gWindow;
	extern uintptr_t ModBase;
	extern int Width;
	extern int Height;
	extern uintptr_t PEB;
	extern ImGuiWindow* pWindow;

	extern ImFont* Font;
	extern ID3D12DescriptorHeap* D3D12DescHeapBackBuff;
	extern ID3D12DescriptorHeap* D3D12DesHeapImGuiRender;
	extern ID3D12GraphicsCommandList* D3D12CommandList;
	extern ID3D12Fence* D3D12Fence;
	extern UINT64 D3D12FenceValue;


}



namespace Draw {
	extern WNDPROC OriginalWndProc;
	
	bool InitD3d12();
	
	void Begin();

	void End();

	HWND MyGetWindow();

	


	void DrawLine(const ImVec2& From, const ImVec2& To, ImVec4 Color, float Thickness);
	void DrawRect(const ImVec2& From, const ImVec2& To, ImVec4 Color, float Rounding, float Thickness);
	void vDrawText(const std::string& Text, const ImVec2& Position, float Size, ImVec4 Color, bool Center=false);
	void DrawRectFilled(const ImVec2& From, const ImVec2& To, ImVec4 Color, float Rounding);
	void DrawCrossHair();
	void DrawCornerBox(const ImVec2& top_left, const ImVec2& bottom_right, const ImVec4& color, float distance);

	BOOL CALLBACK WndwCallBack(HWND Hwnd, LPARAM Param);
	LRESULT MyWndProc(HWND Hwnd, UINT Msg, WPARAM Param, LPARAM LParam);
}




