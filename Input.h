#pragma once
#include "windows.h" 

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

class CInput {
public:
	CInput(void);
	
	bool Arriba(void);
	bool Abajo(void);
	bool Izquierda(void);
	bool Derecha(void);

	~CInput(void);
};
