#include "InputHandler.h"



InputHandler::InputHandler()
{
	//Empty for now
}


InputHandler::~InputHandler()
{
	//Empty for now
}

void InputHandler::InstallHooks()
{

	if (!(hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseCallback, NULL, 0))) {
		throw("Mouse hook error");
		/*
		* we should probably throw an error here or something
		* wakarimasen lol
		*/
	}
	if (!(hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardCallback, NULL, 0))) {
		throw("Keyboard hook error");
		/*
		* we should probably throw an error here or something
		* wakarimasen lol
		*/
	}
}

void InputHandler::InitializeInputHandlersThread()
{
	std::thread mouseEventLoopThrd(&InputHandler::mouseMsgProcessor, this);
	std::thread keyboardEventLoopThrd(&InputHandler::keyboardMsgProcessor, this);
}

void InputHandler::UninstallHook()
{
	UnhookWindowsHookEx(hMouseHook);
	UnhookWindowsHookEx(hKeyboardHook);
}

int InputHandler::Messsages()
{
	while (msg.message != WM_QUIT) { //while we do not close our application
		if (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam; //return the messages
}

void InputHandler::mouseMsgProcessor()
{
	mouseEvent me;
	while (1) {
		// lock mutex
		// while condition youre waiting for: cv.wait()
		// do operation
		// unlock mutex

		std::unique_lock<std::mutex> lock(mtx);
		while (mouseQueue.empty()) {
			cv.wait(lock);
		}

		me = mouseQueue.front();
		mouseQueue.pop();
		lock.unlock();

		if (me == LM_DOWN) {

		}
		else if (me == LM_UP) {
			//end firing
		}
		else if (me == RM_DOWN) {
			//this or that
		}
		else if (me == RM_UP) {
			//the other
		}
		else
		{
			//Once again WAKARIMASEN LOL
		}
	};
}

void InputHandler::keyboardMsgProcessor()
{
	keyboardEvent ke;
	while (1) {
		// lock mutex
		// while condition youre waiting for: cv.wait()
		// do operation
		// unlock mutex

		std::unique_lock<std::mutex> lock(mtx);
		while (keyboardQueue.empty()) {
			cv.wait(lock);
		}

		ke = keyboardQueue.front();
		keyboardQueue.pop();
		lock.unlock();

		if (ke == INS_DOWN) {
		}
		else if (ke == INS_UP) {
			//no use
		}
		else if (ke == DEL_DOWN) {
		}
		else if (ke == DEL_UP) {
			//the other
		}
		else
		{
			//Once again WAKARIMASEN LOL
		}
	};
}

LRESULT InputHandler::mouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
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
			Instance().mouseQueue.push(LM_UP);
			Instance().cv.notify_all();
		}break;
		case WM_LBUTTONDOWN: {
			Instance().mouseQueue.push(LM_DOWN);
			Instance().cv.notify_all();
		}break;
		case WM_RBUTTONDOWN: {
			Instance().mouseQueue.push(RM_DOWN);
			Instance().cv.notify_all();
		}break;
		case WM_RBUTTONUP: {
			Instance().mouseQueue.push(RM_UP);
			Instance().cv.notify_all();
		}break;

		}
		Instance().mtx.unlock();

	}
	return CallNextHookEx(InputHandler::Instance().hMouseHook, nCode, wParam, lParam);
}

LRESULT InputHandler::keyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT * pKeyboardStruct = (KBDLLHOOKSTRUCT *)lParam; // WH_MOUSE_LL struct
	if (nCode == 0) { // we have information in wParam/lParam ? If yes, let's check it:
		Instance().mtx.lock();

		if ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)) {
			switch (pKeyboardStruct->vkCode) {

			case VK_INSERT: {//                                                  These need to be updated to keyboard codes or something needs to be done here
				Instance().keyboardQueue.push(INS_DOWN);
				Instance().cv.notify_all();
			}break;
			case VK_DELETE: {
				Instance().keyboardQueue.push(DEL_DOWN);
				Instance().cv.notify_all();
			}break;

			}

		}
		else if ((wParam == WM_SYSKEYUP) || (wParam == WM_KEYUP)) {
			switch (pKeyboardStruct->vkCode) {

			case VK_INSERT: {//                                                  These need to be updated to keyboard codes or something needs to be done here
				Instance().keyboardQueue.push(INS_UP);
				Instance().cv.notify_all();
			}break;
			case VK_DELETE: {
				Instance().keyboardQueue.push(DEL_DOWN);
				Instance().cv.notify_all();
			}break;

			}

		}

		Instance().mtx.unlock();

	}
	return CallNextHookEx(InputHandler::Instance().hKeyboardHook, nCode, wParam, lParam);
}
