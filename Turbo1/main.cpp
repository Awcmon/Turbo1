#include <iostream>
#define WINVER 0x0500
#include <windows.h>
#include <vector>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unordered_map>

using namespace std;

class Key {
public:
	Key(short _key, short _vkey, short _mouse);
	void press(); //press the key once
	bool isDown(); //is vk held down?
	bool isPressed();
private:
	bool keyDown;
	short key; //hardware scancode (for sending input)
	short vkey; //virtual keycode (for reading input, so I can just use GetAsyncKeyState)
	short mouse;
};

Key::Key(short _key, short _vkey, short _mouse)
{
	key = _key;
	vkey = _vkey;
	mouse = _mouse;
	keyDown = false;
}

void Key::press()
{
	if (mouse == 0)
	{
		//Structure for the keyboard event
		INPUT ip;

		//Set up the INPUT structure
		ip.type = INPUT_KEYBOARD;
		ip.ki.time = 0;
		ip.ki.wVk = 0; //We're doing scan codes instead
		ip.ki.dwExtraInfo = 0;

		//This lets you do a hardware scan instead of a virtual keypress
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
		ip.ki.wScan = key;  //Set a unicode character to use

							//Send the press
		SendInput(1, &ip, sizeof(INPUT));

		//delay before releasing
		//Sleep(4 + (rand() % 1));

		//keyup
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		//wait before sending the next key
		Sleep(30 + (rand() % 1));
	}
	else
	{
		INPUT ip;
		bool ShouldAuto = false;

		ip.type = INPUT_MOUSE;
		ip.mi.time = 0;
		ip.mi.dwExtraInfo = 0;
		ip.mi.dx = 0;
		ip.mi.dy = 0;

		if (mouse == 1)
		{
			ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE;
			SendInput(1, &ip, sizeof(INPUT));

			ip.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE;
			SendInput(1, &ip, sizeof(INPUT));
		}
		else if (mouse == 2)
		{
			ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_MOVE;
			SendInput(1, &ip, sizeof(INPUT));

			ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MOVE;
			SendInput(1, &ip, sizeof(INPUT));
		}

		Sleep(30);
	}
}

bool Key::isDown()
{
	return GetAsyncKeyState(vkey);
}

bool Key::isPressed()
{
	if (GetAsyncKeyState(vkey))
	{
		if (!keyDown)
		{
			keyDown = true;
			return true;
		}
		keyDown = true; //just in case
	}
	else
	{
		keyDown = false;
		return false;
	}
}

int main()
{
	srand(time(NULL));

	std::unordered_map<std::string, Key> keyMap;

	Key* turbo = NULL;

	std::vector<Key> keyVec;
	
	Key activate(0, VK_CAPITAL, 0);

	keyVec.push_back(Key(0, VK_LBUTTON, 1)); //lmb
	keyVec.push_back(Key(0, VK_RBUTTON, 2)); //rmb
	keyVec.push_back(Key(0x10, 0x51, 0)); //q
	keyVec.push_back(Key(0x11, 0x57, 0)); //w
	keyVec.push_back(Key(0x12, 0x45, 0)); //e
	keyVec.push_back(Key(0x13, 0x52, 0)); //r
	keyVec.push_back(Key(0x20, 0x44, 0)); //d
	keyVec.push_back(Key(0x21, 0x46, 0)); //f

	while (1)
	{
		if (turbo == NULL) //find a key
		{
			for (unsigned i = 0; i < keyVec.size(); i++)
			{
				if (keyVec[i].isPressed() && activate.isDown())
				{
					turbo = &keyVec[i];
					break;
				}
				
			}
		}
		else
		{
			if (activate.isDown())
			{
				turbo->press();
			}
			else
			{
				turbo = NULL;
			}
		}
	}

	return 0;
}
