
//
// hud_devtools 
// 
// Assorted tools for developers. 
//

#include "hud.h"
#include "cl_util.h"

#include <sstream>
#include <cstdio>

int CHudDevTools::Init(void)
{
	gHUD.AddHudElem(this);

	m_cvarShowPos = CVAR_CREATE("cl_showpos", "1", FCVAR_CLIENTDLL);

	//
	// Enable us. 
	m_iFlags |= HUD_ACTIVE;

	return 1;
}

int CHudDevTools::VidInit(void)
{
	// Load Sprites here. 
	// We have none.

	return 1;
}

int CHudDevTools::Draw(float flTime)
{
	if (m_cvarShowPos->value)
	{	
		//
		// Height (in pixels) that we should draw the next line. Each code segment should update it. 
		int lineStart = 0;

		{
			Vector viewAngles = gHUD.m_vecAngles;

			std::string output = "Pitch: ";
			output += std::to_string(viewAngles.x);
			output += ", Yaw: ";
			output += std::to_string(viewAngles.y);
			output += ", Roll: ";
			output += std::to_string(viewAngles.z);

			int textWidth = 0;
			int textHeight = 0;
			GetConsoleStringSize(output.c_str(), &textWidth, &textHeight);
			//
			// Currently right aligned, though we could trivially have these centered. 
			DrawConsoleString(ScreenWidth - textWidth, lineStart, output.c_str());

			// Update for the next draw. 
			lineStart += textHeight;
		}

		{
			Vector position = gHUD.m_vecOrigin;

			std::string output = "Pos: ";
			output += std::to_string(position.x);
			output += ", ";
			output += std::to_string(position.y);
			output += ", ";
			output += std::to_string(position.z);

			int textWidth = 0;
			int textHeight = 0;
			GetConsoleStringSize(output.c_str(), &textWidth, &textHeight);
			DrawConsoleString(ScreenWidth - textWidth, lineStart, output.c_str());

			// Update for the next draw. 
			lineStart += textHeight; 
		}

		{
			Vector vecVel;

			VectorSubtract(m_vecPrevOrigin, gHUD.m_vecOrigin, vecVel);

			std::string output = "Vel: ";
			output += std::to_string(vecVel.Length() * 100);

			int textWidth = 0;
			int textHeight = 0;
			GetConsoleStringSize(output.c_str(), &textWidth, &textHeight);
			//
			// Currently right aligned, though we could trivially have these centered. 
			DrawConsoleString(ScreenWidth - textWidth, lineStart, output.c_str());

			// Update for the next draw. 
			lineStart += textHeight;

			m_vecPrevOrigin = gHUD.m_vecOrigin;
		}
		
	}

	return 1;
}