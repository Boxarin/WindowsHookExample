#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <Windows.h>

class InputHandler
{
private:

	std::queue <int> inputQueue;
	std::mutex mtx;
	std::condition_variable cv;

public:

	HHOOK hInputHook;
	MSG msg; // struct with information about all messages in our queue

	InputHandler();
	~InputHandler();

	static InputHandler& Instance() {
		static InputHandler iHandler;
		return iHandler;
	}

	void InstallHook(); // function to install our hook
	void UninstallHook(); // function to uninstall our hook

	int Messsages(); // function to "deal" with our messages 

	void msgProcessor();

	static LRESULT WINAPI inputCallback(int nCode, WPARAM wParam, LPARAM lParam); //callback declaration
};