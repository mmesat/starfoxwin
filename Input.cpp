#include "input.h"

CInput::CInput(void)
{
}

bool CInput::Abajo(void)
{
	if(KEYDOWN(VK_DOWN))
	{
		return true;
	}
	return false;
}

bool CInput::Arriba(void)
{
	if(KEYDOWN(VK_UP))
	{
		return true;
	}
	return false;
}

bool CInput::Izquierda(void)
{
	if(KEYDOWN(VK_LEFT))
	{
		return true;
	}
	return false;
}

bool CInput::Derecha(void)
{
	if(KEYDOWN(VK_RIGHT))
	{
		return true;
	}
	return false;
}

CInput::~CInput(void)
{
}
