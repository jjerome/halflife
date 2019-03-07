//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include"vgui_int.h"
#include<VGUI_Label.h>
#include<VGUI_BorderLayout.h>
#include<VGUI_LineBorder.h>
#include<VGUI_SurfaceBase.h>
#include<VGUI_TextEntry.h>
#include<VGUI_ActionSignal.h>
#include<string.h>
#include <string>
#include "hud.h"
#include "cl_util.h"
#include "camera.h"
#include "kbutton.h"
#include "cvardef.h"
#include "usercmd.h"
#include "const.h"
#include "camera.h"
#include "in_defs.h"
#include "vgui_TeamFortressViewport.h"
#include "vgui_ControlConfigPanel.h"

namespace
{

class TexturePanel : public Panel , public ActionSignal
{
private:
	int _bindIndex;
	TextEntry* _textEntry;
public:
	TexturePanel() : Panel(0,0,256,276)
	{
		_bindIndex=2700;
		_textEntry=new TextEntry("????",0,0,128,20);
		_textEntry->setParent(this);
		_textEntry->addActionSignal(this);

		requestFocus();
	}
public:
	virtual bool isWithin(int x,int y)
	{
		return _textEntry->isWithin(x,y);
	}
public:
	virtual void actionPerformed(Panel* panel)
	{
		char buf[256];
		_textEntry->getText(0,buf,256);
		sscanf(buf,"%d",&_bindIndex);
	}
protected:
	virtual void paintBackground()
	{
			Panel::paintBackground();
			
			int wide,tall;
			getPaintSize(wide,tall);
		
			drawSetColor(0,0,255,0);
			drawSetTexture(_bindIndex);
			drawTexturedRect(0,19,257,257);
	}

};

}

using namespace vgui;

void VGui_ViewportPaintBackground(int extents[4])
{
	gEngfuncs.VGui_ViewportPaintBackground(extents);
}

void* VGui_GetPanel()
{
	return (Panel*)gEngfuncs.VGui_GetPanel();
}

std::string GenerateIndent(int count)
{
	std::string output;

	for (int i = 0; i < count; ++i)
	{
		output += '\t';
	}

	return output;
}

void VGui_DumpPanelInfo(Panel* panel, int indentIdx)
{
	for (int i = 0; i < panel->getChildCount(); ++i) {
		Panel* child = panel->getChild(i);

		std::string indent = GenerateIndent(indentIdx);

		gEngfuncs.Con_Printf("%s[%d]: GetChildCount: %d, IsEnabled: %s, IsVisible: %s\n", indent.c_str(), i, child->getChildCount(), (child->isEnabled() ? "Yes" : "No"), (child->isVisible() ? "Yes" : "No"));

		if (child->getChildCount() > 0) {
			VGui_DumpPanelInfo(child, indentIdx + 1);
		}
	}
}

void Command_VGuiDumpPanels(void)
{
	auto root = (Panel*)VGui_GetPanel();

	if (!root)
	{
		gEngfuncs.Con_Printf("Failed to get root panel. VGUI sucks, I guess.\n");
		return;
	}

	gEngfuncs.Con_Printf("Dumping VGUI Panel Info...\n");
	
	VGui_DumpPanelInfo(root, 0);
}

void Command_VGuiShowPanel(void)
{
	if (gEngfuncs.Cmd_Argc() < 1)
	{
		gEngfuncs.Con_Printf("Syntax: vgui_showpanel <integer>\n");
		return;
	}

	int requestedIdx = atoi(gEngfuncs.Cmd_Argv(1));
	Panel* root = (Panel*)VGui_GetPanel();

	if (root)
	{
		if (requestedIdx > root->getChildCount() || requestedIdx < 0)
		{
			gEngfuncs.Con_Printf("Requested panel number is out of bounds. [0,%d]\n", root->getChildCount());
			return;
		}

		Panel* child = root->getChild(requestedIdx);

		if (!child)
		{
			gEngfuncs.Con_Printf("Failed to get child. VGUI sucks, I guess.\n");
			return;
		}

		child->setEnabled(true);
		child->setVisible(true);
	}
	else
	{
		gEngfuncs.Con_Printf("Failed to get root panel. VGUI sucks, I guess.\n");
		return;
	}
}

void VGui_Startup()
{
	Panel* root=(Panel*)VGui_GetPanel();
	root->setBgColor(128,128,0,255);
	//root->setNonPainted(false);
	//root->setBorder(new LineBorder());
	root->setLayout(new BorderLayout(0));

	
	//root->getSurfaceBase()->setEmulatedCursorVisible(true);

	if (gViewPort != NULL)
	{
//		root->removeChild(gViewPort);

		// free the memory
//		delete gViewPort;
//		gViewPort = NULL;

		gViewPort->Initialize();
	}
	else
	{
		gViewPort = new TeamFortressViewport(0,0,root->getWide(),root->getTall());
		gViewPort->setParent(root);
	}

	
	/*TexturePanel* texturePanel=new TexturePanel();
	texturePanel->setParent(gViewPort);*/
	
	gEngfuncs.pfnAddCommand("vgui_dumppanels", Command_VGuiDumpPanels);
	gEngfuncs.pfnAddCommand("vgui_showpanel", Command_VGuiShowPanel);
}

void VGui_Shutdown()
{
	delete gViewPort;
	gViewPort = NULL;
}






