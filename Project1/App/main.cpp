#include "gui.h"
#include <thread>

int __stdcall wWinMain(HINSTANCE instace, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{
	//create gui
	gui::createHWindow("HADES", "hades class");
	gui::createDevice();
	gui::createImgui();
	while (gui::exit)
	{
		gui::beginRender();
		gui::render();
		gui::endrender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	//destroy gui
	gui::destroyImgui();
	gui::destroDevice();
	gui::destroyHWindow();
	
	return EXIT_SUCCESS;
}
