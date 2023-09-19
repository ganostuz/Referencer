#include "rfpch.h"
#include "Application.h"

int main(int argc, char** argv)
{
	Referencer::Application* app = new Referencer::Application();
	app->run();
	delete app;
}