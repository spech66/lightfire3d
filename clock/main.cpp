#include "main.h"

int main(int argc, char** argv)
{
	LF3D::CCore 			Core;
	LF3D::CGraphicDevice 	*GDevice = Core.GetGraphicDevice();
	LF3D::CImageLoader		*ImageLoader = Core.GetImageLoader();
	LF3D::CInputDevice		*InputDevice = Core.GetInputDevice();
	
	int WinX = 640, WinY = 480;
	
	GDevice->Init(1, WinX, WinY, 32);
	GDevice->SetWindowText("Clock");

	ImageLoader->LoadImage("back.tga");
	ImageLoader->LoadImage("digit.tga");
	
	unsigned long FPS = SDL_GetTicks();
	unsigned long FrameTime = SDL_GetTicks();
	unsigned long DeltaTime = 0;
	
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(GDevice->IsRunning())
	{
		DeltaTime = SDL_GetTicks() - FrameTime;
		FrameTime = SDL_GetTicks();
		
		InputDevice->UpdateInput();
		if(InputDevice->GetKeyState(SDLK_ESCAPE)) GDevice->Quit();
				
		GDevice->BeginRender();
		GDevice->SetOrthoView();
		
		ImageLoader->SetTexture("back.tga");
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, WinY);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(WinX, WinY);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(WinX, 0.0f);
		glEnd();
		
		time_t ltime;
		time(&ltime);
		struct tm *today;
		today = localtime(&ltime);
		char TimeH[4];    	strftime(TimeH, 4, "%H", today);
		char TimeM[4];    	strftime(TimeM, 4, "%M", today);
		char TimeS[4];    	strftime(TimeS, 4, "%S", today);
		int Time[4];
		Time[0] = atoi(&TimeH[0]);
		Time[1] = atoi(&TimeH[1]);
		Time[2] = atoi(TimeM);
		Time[3] = atoi(TimeS);

		//Hour 1
		ImageLoader->SetTexture("digit.tga");
		if(TimeH[0] == '1')
		{
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(64.0f, 400.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(64.0f, 432.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(96.0f, 432.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(96.0f, 400.0f);
			glEnd();
		}
		
		if(TimeH[0]== '2')
		{
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(64.0f, 304.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(64.0f, 336.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(96.0f, 336.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(96.0f, 304.0f);
			glEnd();
		}
	
		//Hour 2
		int Val = 8;
		for(int i = 0; i < 4; i++)
		{
			if(Time[1] - Val >= 0)
			{
				Time[1] -= Val;
				float Mod = i * 96.0f;
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(160.0f, 112.0f+Mod);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(160.0f, 144.0f+Mod);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(192.0f, 144.0f+Mod);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(192.0f, 112.0f+Mod);
				glEnd();
			}			
			Val = Val / 2;
		}
		
		//Min 1
		Val = 4;
		for(int i = 0; i < 3; i++)
		{
			if(Time[2] - Val*10 >= 0)
			{
				Time[2] -= Val*10;
				float Mod = (i+1) * 96.0f;
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(256.0f, 112.0f+Mod);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(256.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(288.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(288.0f, 112.0f+Mod);
				glEnd();
			}
			
			Val = Val / 2;
		}

		Val = 8;
		for(int i = 0; i < 4; i++)
		{
			if(Time[2] - Val >= 0)
			{
				Time[2] -= Val;
				float Mod = i * 96.0f;
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(352.0f, 112.0f+Mod);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(352.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(384.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(384.0f, 112.0f+Mod);
				glEnd();
			}
			
			Val = Val / 2;
		}
		
		//Sec 1
		Val = 4;
		for(int i = 0; i < 3; i++)
		{
			if(Time[3] - Val*10 >= 0)
			{
				Time[3] -= Val*10;
				float Mod = (i+1) * 96.0f;
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(448.0f, 112.0f+Mod);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(448.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(480.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(480.0f, 112.0f+Mod);
				glEnd();
			}
			
			Val = Val / 2;
		}

		Val = 8;
		for(int i = 0; i < 4; i++)
		{
			if(Time[3] - Val >= 0)
			{
				Time[3] -= Val;
				float Mod = i * 96.0f;
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(544.0f, 112.0f+Mod);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(544.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(576.0f, 145.0f+Mod);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(576.0f, 112.0f+Mod);
				glEnd();
			}
			
			Val = Val / 2;
		}

		
		GDevice->EndRender();
	}

	return 0;
}
