#include "App.hpp"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[])
{
	(void)argc; (void)argv;
	GApp::Settings settings(argc, argv);
	
	settings.window.width   = 1200; 
	settings.window.height	= 800;
	settings.window.resizable = true;
	return App(settings).run();
	return 1;
}