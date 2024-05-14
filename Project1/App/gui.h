#pragma once
#include "d3d9.h"

namespace gui
{
	//constant window size
	constexpr int width = 1300;
	constexpr int height = 500;
	
	inline bool exit = true;

	//win api window variables
	inline HWND window = nullptr;
	inline WNDCLASSEXA windowClass = { };
	
	//points for window movement
	inline POINTS position = { };

	//direct x state vars
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = {};

	//handle window creation and destruction
	void createHWindow(const char* windowName, const char* className) noexcept;
	void destroyHWindow()noexcept;

	//handle device creation
	bool createDevice();
	void resetDevice();
	void destroDevice();

	//handle imgui creation and destruction
	void createImgui() noexcept;
	void destroyImgui() noexcept;

	void beginRender() noexcept;
	void endrender() noexcept;
	void render() noexcept;

}