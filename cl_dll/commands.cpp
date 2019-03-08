//========= Copyright © 1996-2002, ??? LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "wrect.h"
#include "cl_dll.h"
#include "cvardef.h"
#include <SDL2/SDL.h>
#include <string>

typedef struct cmd_function_s
{
	struct cmd_function_s *next;
	char *name;
	void(*function)(void);
	int flags;
}
cmd_function_t;

extern cl_enginefunc_t gEngfuncs;

void Command_SDLInfo(void)
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	gEngfuncs.Con_Printf("Compiled with SDL ver %d.%d.%d ...\n",
		compiled.major, compiled.minor, compiled.patch);
	gEngfuncs.Con_Printf("Linked against SDL ver %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
}

void Command_Find(void)
{
	if (gEngfuncs.Cmd_Argc() < 1)
	{
		gEngfuncs.Con_Printf("Syntax: find <string>\nFind looks for the substring and prints out any cvars or console commands that have that string in them.\n");
		return;
	}

	std::string needle = gEngfuncs.Cmd_Argv(1);

	//
	// Look through all the console variables first. 
	for (cvar_t *currentCVar = gEngfuncs.GetFirstCvarPtr();
		currentCVar->next != nullptr;
		currentCVar = currentCVar->next)
	{
		//
		// Copy into a std:string
		std::string name(currentCVar->name);

		if (name.find(needle) != std::string::npos)
		{
			gEngfuncs.Con_Printf("\"%s\" = \"%s\"\n", currentCVar->name, currentCVar->string);
			
			std::string output;
			output += (currentCVar->flags & FCVAR_ARCHIVE) ? "archive " : "";
			output += (currentCVar->flags & FCVAR_USERINFO) ? "userinfo " : "";
			output += (currentCVar->flags & FCVAR_SERVER) ? "server " : "";
			output += (currentCVar->flags & FCVAR_EXTDLL) ? "external dll " : "";
			output += (currentCVar->flags & FCVAR_CLIENTDLL) ? "client dll " : "";
			output += (currentCVar->flags & FCVAR_PROTECTED) ? "protected " : "";
			output += (currentCVar->flags & FCVAR_SPONLY) ? "singleplayer " : "";
			output += (currentCVar->flags & FCVAR_PRINTABLEONLY) ? "printable only " : "";
			output += (currentCVar->flags & FCVAR_UNLOGGED) ? "unlogged " : "";
			output += (currentCVar->flags & FCVAR_NOEXTRAWHITEPACE) ? "no extra whitespace " : "";

			gEngfuncs.Con_Printf("%s\n", output.c_str());
		}
	}


	for (cmd_function_t *cmd = (cmd_function_t*)gEngfuncs.GetFirstCmdFunctionHandle();
		cmd != nullptr;
		cmd = cmd->next)
	{
		std::string name(cmd->name);

		if (name.find(needle) != std::string::npos)
		{
			gEngfuncs.Con_Printf("%s %d\n", name.c_str(), cmd->flags);
		}
	}
}

void RegisterCommands(void)
{
	gEngfuncs.pfnAddCommand("sdl_info", Command_SDLInfo);
	gEngfuncs.pfnAddCommand("find", Command_Find);
}