//-----------------------------------------------------------------------------
//  InputDevice.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Handle Input -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

//-----------------------------------------------------------------------------
// Name:		NAME
// Description:	DESCRIPTION
//-----------------------------------------------------------------------------
CInputDevice::CInputDevice(CCore *Core)
{
	prv_Core = Core;
	
	for(int keys = 0; keys < 324; keys++)
		prv_Keyboard[keys] = 0;

	for(int buttons = 0; buttons < 8; buttons++)
		prv_MouseB[buttons] = false;
		
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);
}

void CInputDevice::UpdateInput()
{
	SDL_Event* SDLEvent = prv_Core->GetGraphicDevice()->GetSDLEvent();
	prv_TextInput = "";
		
    while(SDL_PollEvent(SDLEvent))
    {
        switch(SDLEvent->type)
        {
            case SDL_KEYDOWN:
			{
				if(SDLEvent->key.keysym.sym >= '0' && SDLEvent->key.keysym.sym <= 'z')
				{
					if(SDLEvent->key.keysym.mod & KMOD_LSHIFT ||
						SDLEvent->key.keysym.mod & KMOD_RSHIFT)
					{
						if(SDLEvent->key.keysym.sym > '9')
							prv_TextInput += toupper(*SDL_GetKeyName(SDLEvent->key.keysym.sym));
						if(SDLEvent->key.keysym.sym == '0') prv_TextInput += "="; 
						if(SDLEvent->key.keysym.sym == '1') prv_TextInput += "!"; 
						if(SDLEvent->key.keysym.sym == '2') prv_TextInput += "\""; 
						if(SDLEvent->key.keysym.sym == '3') prv_TextInput += "§"; 
						if(SDLEvent->key.keysym.sym == '4') prv_TextInput += "$"; 
						if(SDLEvent->key.keysym.sym == '5') prv_TextInput += "%"; 
						if(SDLEvent->key.keysym.sym == '6') prv_TextInput += "&"; 
						if(SDLEvent->key.keysym.sym == '7') prv_TextInput += "/"; 
						if(SDLEvent->key.keysym.sym == '8') prv_TextInput += "("; 
						if(SDLEvent->key.keysym.sym == '9') prv_TextInput += ")";
					} else {
						prv_TextInput += SDL_GetKeyName(SDLEvent->key.keysym.sym);
					}
				}

				if(SDLEvent->key.keysym.mod & KMOD_LSHIFT ||
						SDLEvent->key.keysym.mod & KMOD_RSHIFT)
				{
					if(SDLEvent->key.keysym.sym == '.') prv_TextInput += ":";
					if(SDLEvent->key.keysym.sym == ',') prv_TextInput += ";";
					if(SDLEvent->key.keysym.sym == '+') prv_TextInput += "*";
					if(SDLEvent->key.keysym.sym == '-') prv_TextInput += "_";
					if(SDLEvent->key.keysym.sym == '#') prv_TextInput += "'";
					if(SDLEvent->key.keysym.sym == 223) prv_TextInput += "?";
					if(SDLEvent->key.keysym.sym == '<') prv_TextInput[prv_TextInput.size()-1] = '>';
				} else {
					if(SDLEvent->key.keysym.sym == '.') prv_TextInput += ".";
					if(SDLEvent->key.keysym.sym == ',') prv_TextInput += ",";
					if(SDLEvent->key.keysym.sym == '+') prv_TextInput += "+";
					if(SDLEvent->key.keysym.sym == '-') prv_TextInput += "-";
					if(SDLEvent->key.keysym.sym == '#') prv_TextInput += "#";
				}

				if(SDLEvent->key.keysym.sym == SDLK_SPACE) 		prv_TextInput += " ";
				if(SDLEvent->key.keysym.sym == SDLK_TAB)		prv_TextInput += "    ";
				if(SDLEvent->key.keysym.sym == SDLK_BACKSPACE)	prv_TextInput += '\b';
			
				prv_Keyboard[SDLEvent->key.keysym.sym] = SDLEvent->key.state;
			} break;
				
        	case SDL_KEYUP:
			{
				prv_Keyboard[SDLEvent->key.keysym.sym] = SDLEvent->key.state;
			} break;
			
			case SDL_MOUSEMOTION:
			{
				prv_MouseY = SDLEvent->motion.y;
				prv_MouseX = SDLEvent->motion.x;
			} break;
			
			case SDL_MOUSEBUTTONDOWN:
			{
				if(SDLEvent->button.button-1 < 8)
					prv_MouseB[SDLEvent->button.button-1] = true;
			} break;
			
			case SDL_MOUSEBUTTONUP:
			{
				if(SDLEvent->button.button-1 < 8)
					prv_MouseB[SDLEvent->button.button-1] = false;
			} break;
		}
	}
	SDL_ShowCursor(0);
}

bool CInputDevice::GetKeyState(SDLKey Key)
{
	return prv_Keyboard[Key];
}

bool CInputDevice::GetMouseButtonState(int Button)
{
	return prv_MouseB[Button];
}

void CInputDevice::GetMousePos(int &X, int &Y)
{
	X = prv_MouseX;
	Y = prv_MouseY;
}

string CInputDevice::GetTextInput()
{
	return prv_TextInput;
}


CInputDevice::~CInputDevice()
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(1);
}

};
