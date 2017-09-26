#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <Windows.h>

#include "InputStructs.h"

class InputHandler
{
private:

	std::queue <mouseEvent> mouseQueue;
	std::queue <keyboardEvent> keyboardQueue;
	std::mutex mtx;
	std::condition_variable cv;

public:

	HHOOK hMouseHook, hKeyboardHook;
	MSG msg; // struct with information about all messages in our queue

	InputHandler();
	~InputHandler();

	static InputHandler& Instance() {
		static InputHandler iHandler;
		return iHandler;
	}

	void InstallHooks(); // function to install our hook
	void InitializeInputHandlersThread();//function to initialize the threads we need to handle input
	void UninstallHook(); // function to uninstall our hook

	int Messsages(); // function to "deal" with our messages 

	void mouseMsgProcessor();
	void keyboardMsgProcessor();

	static LRESULT WINAPI mouseCallback(int nCode, WPARAM wParam, LPARAM lParam); //callback declaration
	static LRESULT WINAPI keyboardCallback(int nCode, WPARAM wParam, LPARAM lParam); //callback declaration
};