#include "gui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "menus.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideparameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
	{
		return true;
	}

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED) {

			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::resetDevice();
		}
	}return 0;
	case WM_SYSCOMMAND: {
		// disable alt application window
		if ((wideParameter & 0xfff0) == SC_KEYMENU) {
			return 0;
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;
	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;
	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON) {
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);
			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::width &&
				gui::position.y >= 0 && gui::position.y <= 19)
			{
				SetWindowPos(gui::window, HWND_TOPMOST, rect.left,rect.top,0,0,SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
			}
		}
	}
	}

	return DefWindowProcW(window, message, wideParameter, longParameter);
}

//handle window creation and destruction
void gui::createHWindow(const char* windowName, const char* className) noexcept {
	
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA(className,windowName,WS_POPUP,100,100,width,height,0,0,windowClass.hInstance,0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}
void gui::destroyHWindow()noexcept {
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

//handle device creation
bool gui::createDevice() {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d) {
		return false;
	}
	ZeroMemory(&presentParameters, sizeof(presentParameters));
	presentParameters.Windowed = true;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = true;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,&device) < 0) {
		return false;
	}
	return true;
}
void gui::resetDevice() {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = device->Reset(&presentParameters);
	if (result == D3DERR_INVALIDCALL) {
		IM_ASSERT(0);
	}
	ImGui_ImplDX9_CreateDeviceObjects();
}
void gui::destroDevice() {
	if (device) {
		device->Release();
		device = nullptr;
	}
	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}

//handle imgui creation and destruction
void gui::createImgui() noexcept {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;
	ImGui::StyleColorsDark;

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}
void gui::destroyImgui() noexcept {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::beginRender() noexcept {
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	// start the Dear imgui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void gui::endrender() noexcept {
	ImGui::EndFrame();
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}
	const auto result = device->Present(0, 0, 0, 0);

	//handle lost of d3d9 devic
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		resetDevice();
	}
}
void gui::render() noexcept {
	ImGui::SetNextWindowPos({ 0,0 });
	ImGui::SetNextWindowSize({ width, height });
	ImGui::Begin("HADES", &exit, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	mainMenu();

	ImGui::End();
}