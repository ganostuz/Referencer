#include "rfpch.h"
#include "Application.h"

#if defined(_WIN32)
#include <windows.h>
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	Referencer::Application* app = new Referencer::Application();
	app->run();
	delete app;
	return 0;
}
#else
int main(int argc, char** argv)
{
	Referencer::Application* app = new Referencer::Application();
	app->run();
	delete app;
	return 0;
}
#endif