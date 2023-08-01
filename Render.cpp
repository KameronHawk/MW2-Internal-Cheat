#include "Render.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Globals {
	IDXGISwapChain3* pSwapChain = nullptr;
	ID3D12Device* pD3DDevice;
	ID3D12CommandQueue* D3D12CommandQueue;

	HANDLE gWindow;
	uintptr_t ModBase;

	int Width;
	int Height;

	uintptr_t PEB;

	ImFont* Font = nullptr;
	ID3D12DescriptorHeap* D3D12DescHeapBackBuff = nullptr;
	ID3D12DescriptorHeap* D3D12DesHeapImGuiRender = nullptr;
	ID3D12GraphicsCommandList* D3D12CommandList = nullptr;
	ID3D12Fence* D3D12Fence = nullptr;
	UINT64 D3D12FenceValue = 0;


}





struct FrameContext {
	ID3D12CommandAllocator* CommandAllocator = nullptr;
	ID3D12Resource* MainRenderTargetResource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE MainRenderTargetDesc;
};


uint32_t BuffersCount = -1;
FrameContext* FrameCtx;

namespace Draw {

	ImFont* Font;

	WNDPROC OriginalWndProc;

	//initalize d3d12
	bool InitD3d12() {
		Globals::D3D12CommandQueue = *(ID3D12CommandQueue**)((uintptr_t)Globals::pSwapChain + 0x118);
		
		ImGui::CreateContext();
		unsigned char* pixels;
		int width, height;

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		Menu::SetupTheme();
		io.Fonts->AddFontDefault();
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		io.IniFilename = NULL;
		CreateEvent(nullptr, false, false, nullptr);

		DXGI_SWAP_CHAIN_DESC sdesc;
		Globals::pSwapChain->GetDesc(&sdesc);
		Globals::Height = sdesc.BufferDesc.Height;
		Globals::Width = sdesc.BufferDesc.Width;
		sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sdesc.OutputWindow = (HWND)Globals::gWindow;
		sdesc.Windowed = ((GetWindowLongPtr((HWND)Globals::gWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

		BuffersCount = sdesc.BufferCount;

		FrameCtx = new FrameContext[BuffersCount];

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
		DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		DescriptorImGuiRender.NumDescriptors = BuffersCount;
		DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		if (Globals::pD3DDevice->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&Globals::D3D12DesHeapImGuiRender)) != S_OK) {
			false;
		}

		ID3D12CommandAllocator* Allocator;
		if (Globals::pD3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK) {
			return false;
		}
		for (size_t i = 0; i < BuffersCount; i++) {
			FrameCtx[i].CommandAllocator = Allocator;
		}
		if (Globals::pD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&Globals::D3D12CommandList)) != S_OK || Globals::D3D12CommandList->Close() != S_OK) {
			return false;
		}

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffer;
		DescriptorBackBuffer.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		DescriptorBackBuffer.NumDescriptors = BuffersCount;
		DescriptorBackBuffer.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DescriptorBackBuffer.NodeMask = 1;

		if (Globals::pD3DDevice->CreateDescriptorHeap(&DescriptorBackBuffer, IID_PPV_ARGS(&Globals::D3D12DescHeapBackBuff)) != S_OK) {

			return false;
		}

		const auto rtvDescriptorSize = Globals::pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = Globals::D3D12DescHeapBackBuff->GetCPUDescriptorHandleForHeapStart();

		for (size_t i = 0; i < BuffersCount; i++) {
			ID3D12Resource* pBackBuffer = nullptr;
			FrameCtx[i].MainRenderTargetDesc = rtvHandle;
			Globals::pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			Globals::pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
			FrameCtx[i].MainRenderTargetResource = pBackBuffer;
			rtvHandle.ptr += rtvDescriptorSize;
		}



		ImGui_ImplWin32_Init(Globals::gWindow);
		ImGui_ImplDX12_Init(Globals::pD3DDevice, BuffersCount,
			DXGI_FORMAT_R8G8B8A8_UNORM, Globals::D3D12DesHeapImGuiRender,
			Globals::D3D12DesHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
			Globals::D3D12DesHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());

		Font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
		

		return true;
	}


	void End() {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DrawList->PushClipRectFullScreen();
		// rendering
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		FrameContext& currentFrameContext = FrameCtx[Globals::pSwapChain->GetCurrentBackBufferIndex()];
		currentFrameContext.CommandAllocator->Reset();


		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = currentFrameContext.MainRenderTargetResource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Globals::D3D12CommandList->Reset(currentFrameContext.CommandAllocator, NULL);
		Globals::D3D12CommandList->ResourceBarrier(1, &barrier);
		Globals::D3D12CommandList->OMSetRenderTargets(1, &currentFrameContext.MainRenderTargetDesc, FALSE, NULL);
		Globals::D3D12CommandList->SetDescriptorHeaps(1, &Globals::D3D12DesHeapImGuiRender);
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Globals::D3D12CommandList);
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		Globals::D3D12CommandList->ResourceBarrier(1, &barrier);
		Globals::D3D12CommandList->Close();

		Globals::D3D12CommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&Globals::D3D12CommandList);
	}

	HWND MyGetWindow() {
		EnumWindows(WndwCallBack, GetCurrentProcessId());
		return (HWND)Globals::gWindow;
	}



	BOOL CALLBACK WndwCallBack(HWND Hwnd, LPARAM Param) {
		DWORD ProcID;
		GetWindowThreadProcessId(Hwnd, &ProcID);

		if (ProcID == Param) {
			WCHAR ClassNameBuffer[MAX_PATH];
			GetClassName(Hwnd, ClassNameBuffer, MAX_PATH);
			std::wstring ClassName(ClassNameBuffer);

			if (ClassName.compare((L"MSCTFIME UI")) && ClassName.compare((L"IME")) &&
				ClassName.compare((L"ConsoleWindowClass"))) {
				Globals::gWindow = Hwnd;
				return false;
			}
		}
		return true;
	}


	// begin draw
	void Begin() {
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();

		if (Settings::bShowMenu) {
			Menu::RenderMenu();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("##Backbuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

	}

	LRESULT MyWndProc(HWND Hwnd, UINT Msg, WPARAM Param, LPARAM LParam) {
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplWin32_WndProcHandler(Hwnd, Msg, Param, LParam);

		switch (Msg) {
		case WM_KEYDOWN: {
			if (Param == VK_INSERT || Param == VK_F5) {

				Settings::bShowMenu = !Settings::bShowMenu;
			}
			break;
		}
		};

		if (io.WantCaptureMouse && (Msg == WM_LBUTTONDOWN || Msg == WM_LBUTTONUP || Msg == WM_RBUTTONDOWN || Msg == WM_RBUTTONUP || Msg == WM_MBUTTONDOWN || Msg == WM_MBUTTONUP || Msg == WM_MOUSEWHEEL || Msg == WM_MOUSEMOVE)) {

			return TRUE;
		}



		return CallWindowProc(Draw::OriginalWndProc, Hwnd, Msg, Param, LParam);
	}
}

	


	void Draw::DrawLine(const ImVec2& From, const ImVec2& To, ImVec4 Color, float Thickness) {
		ImGuiWindow* Window = ImGui::GetCurrentWindow();
		Window->DrawList->AddLine(From, To, ImGui::GetColorU32({ Color}), Thickness);
	}

	void Draw::DrawRect(const ImVec2& From, const ImVec2& To, ImVec4 Color, float Rounding, float Thickness)
	{
		ImGuiWindow* Window = ImGui::GetCurrentWindow();
		Window->DrawList->AddRect(From, To, ImGui::GetColorU32({ Color}), Rounding, NULL, Thickness);
	}

	void Draw::vDrawText(const std::string& Text, const ImVec2& Position, float Size, ImVec4 Color, bool Center) {
		ImGuiWindow* Window = ImGui::GetCurrentWindow();

		ImVec2 TextSize = Font->CalcTextSizeA(Size, FLT_MAX, 0.0f, Text.c_str());

		if (Center) {
			Window->DrawList->AddText(Font, Size, { Position.x - TextSize.x / 2.0f, Position.y }, ImGui::GetColorU32({ Color }), Text.c_str());
		}
		else {
			Window->DrawList->AddText(Font, Size, { Position.x, Position.y }, ImGui::GetColorU32({ Color}), Text.c_str());
		}


	}

	void Draw::DrawRectFilled(const ImVec2& From, const ImVec2& To, ImVec4 Color, float Rounding) {
		ImGuiWindow* Window = ImGui::GetCurrentWindow();
		Window->DrawList->AddRectFilled(From, To, ImGui::GetColorU32({ Color}), Rounding);
	}

	void Draw::DrawCrossHair() {
		ImGuiWindow* Window = ImGui::GetCurrentWindow();
		Window->DrawList->AddLine({ (float)Globals::Width / 2, (float)(Globals::Height / 2) - Settings::CrossHairSize - 1 }, { (float)Globals::Width / 2, (float)(Globals::Height / 2) + Settings::CrossHairSize + 1 }, ImGui::GetColorU32({ Settings::CrossHairColor }));
		Window->DrawList->AddLine({ (float)(Globals::Width / 2) - Settings::CrossHairSize - 1, (float)(Globals::Height / 2) }, { (float)(Globals::Width / 2) + Settings::CrossHairSize + 1, (float)(Globals::Height / 2) }, ImGui::GetColorU32({ Settings::CrossHairColor}));
	}

	void Draw::DrawCornerBox(const ImVec2& top_left, const ImVec2& bottom_right, const ImVec4& color, float distance)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		// Calculate corner size based on the distance
		float min_distance = 10.0f; // Minimum distance value in your range
		float max_distance = 400.0f; // Maximum distance value in your range
		float min_corner_size = 10.0f; // Minimum corner size
		float max_corner_size = 25.0f; // Maximum corner size
		float distance_factor = 1.0f - (distance - min_distance) / (max_distance - min_distance);
		float corner_size = min_corner_size + (max_corner_size - min_corner_size) * distance_factor;

		float line_thickness = 1.0f; // Adjust this value as needed

		// Top-left corner
		draw_list->AddLine(ImVec2(top_left.x, top_left.y), ImVec2(top_left.x + corner_size, top_left.y), ImGui::GetColorU32(color), line_thickness);
		draw_list->AddLine(ImVec2(top_left.x, top_left.y), ImVec2(top_left.x, top_left.y + corner_size), ImGui::GetColorU32(color), line_thickness);

		// Top-right corner
		draw_list->AddLine(ImVec2(bottom_right.x - corner_size, top_left.y), ImVec2(bottom_right.x, top_left.y), ImGui::GetColorU32(color), line_thickness);
		draw_list->AddLine(ImVec2(bottom_right.x, top_left.y), ImVec2(bottom_right.x, top_left.y + corner_size), ImGui::GetColorU32(color), line_thickness);

		// Bottom-right corner
		draw_list->AddLine(ImVec2(bottom_right.x, bottom_right.y - corner_size), ImVec2(bottom_right.x, bottom_right.y), ImGui::GetColorU32(color), line_thickness);
		draw_list->AddLine(ImVec2(bottom_right.x - corner_size, bottom_right.y), ImVec2(bottom_right.x, bottom_right.y), ImGui::GetColorU32(color), line_thickness);

		// Bottom-left corner
		draw_list->AddLine(ImVec2(top_left.x, bottom_right.y - corner_size), ImVec2(top_left.x, bottom_right.y), ImGui::GetColorU32(color), line_thickness);
		draw_list->AddLine(ImVec2(top_left.x, bottom_right.y), ImVec2(top_left.x + corner_size, bottom_right.y), ImGui::GetColorU32(color), line_thickness);
	}
