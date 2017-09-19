#include "InputHandler.h"



InputHandler::InputHandler()
{
	//Empty for now
}


InputHandler::~InputHandler()
{
	//Empty for now
}

void InputHandler::InstallHook()
{
	//doing a mouse hook because its basic af
	if (!(hInputHook = SetWindowsHookEx(WH_MOUSE_LL, inputCallback, NULL, 0))) {
		/*
		* we should probably throw an error here or something
		* wakarimasen lol
		*/
	}
}

void InputHandler::UninstallHook()
{
	UnhookWindowsHookEx(hInputHook);
}

int InputHandler::Messsages()
{
	std::thread inputEventLoopThrd(&InputHandler::msgProcessor, this);
	while (msg.message != WM_QUIT) { //while we do not close our application
		if (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	UninstallHook(); //if we close, let's uninstall our hook
	return (int)msg.wParam; //return the messages
}

void InputHandler::msgProcessor()
{
	//replace int with an enum of input type such as moouse up mouse down etc
	int inputHolder;
	while (1) {
		// lock mutex
		// while condition youre waiting for: cv.wait()
		// do operation
		// unlock mutex

		std::unique_lock<std::mutex> lock(mtx);
		while (inputQueue.empty()) {
			cv.wait(lock);
		}

		inputHolder = inputQueue.front();
		inputQueue.pop();
		lock.unlock();

		//replace int with an enum of input type such as moouse up mouse down etc	

		if (inputHolder == 0) {
			//do something
		}
		else if (inputHolder == 1) {
			//do something
		}
		else if (inputHolder == 2) {
			//do something
		}
		else if (inputHolder == 3) {
			//do something
		}
		else
		{
			//do something that isnt the other things
		}
	};
}

LRESULT InputHandler::inputCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT * pMouseStruct = (MSLLHOOKSTRUCT *)lParam; // WH_MOUSE_LL struct
	if (nCode == 0) { // we have information in wParam/lParam ? If yes, let's check it:
		if (pMouseStruct != NULL) { // Mouse struct contain information?			
									//We dont need this cuz no console?
									//printf_s("Mouse Coordinates: x = %i | y = %i \n", pMouseStruct->pt.x, pMouseStruct->pt.y);
		}

		Instance().mtx.lock();

		switch (wParam) {

		case WM_LBUTTONUP: {
			Instance().inputQueue.push(0);
			Instance().cv.notify_all();
		}break;
		case WM_LBUTTONDOWN: {
			Instance().inputQueue.push(1);
			Instance().cv.notify_all();
		}break;
		case WM_RBUTTONDOWN: {
			Instance().inputQueue.push(2);
			Instance().cv.notify_all();
		}break;
		case WM_RBUTTONUP: {
			Instance().inputQueue.push(3);
			Instance().cv.notify_all();
		}break;

		}
		Instance().mtx.unlock();

	}
	return CallNextHookEx(InputHandler::Instance().hInputHook, nCode, wParam, lParam);
}
