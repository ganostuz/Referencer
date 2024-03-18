#include "rfpch.h"
#include "Application.h"
#include <Windows.h>


#ifdef _WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	Referencer::Application* app = new Referencer::Application();
	app->run();
	delete app;
}
#else
int main(int argc, char** argv)
{
	Referencer::Application* app = new Referencer::Application();
	app->run();
	delete app;
}
#endif