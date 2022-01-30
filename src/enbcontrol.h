#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>

namespace ENBControl {

	void Update();
}

namespace ENBSDK {

	enum ENBCallbackType : long
	{
		ENBCallback_EndFrame = 1,    //called at the end of frame, before displaying result on the screen
		ENBCallback_BeginFrame = 2,  //called after frame was displayed, time between end and begin frame may be big enough to execute something heavy in separate thread
		ENBCallback_PreSave = 3,     //called before user trying to save config, useful for restoring original parameters
		ENBCallback_PostLoad = 4,    //called when parameters are created and loaded, useful for saving original parameters
		//v1001:
		ENBCallback_OnInit = 5,     //called when mod is initialized completely, but nothing yet processed, all new resources must be created by plugin
		ENBCallback_OnExit = 6,     //called when game or mod are about to close, all new resources must be deleted by plugin
		ENBCallback_PreReset = 7,   //called when game or mod destroy internal objects (display mode changes for example), need to destroy all objects in plugin. may not be called ever, but must be handled in some similar way to OnExit
		ENBCallback_PostReset = 8,  //called when game or mod re-create internal objects (after display mode changes for example), need to create all objects in plugin again, including pointers to interfaces of d3d. may not be called ever, but must be handled in some similar way to OnInit

		ENBCallback_FORCEDWORD = 0x7fffffff  //unused
	};

	//v1001:
	enum ENBStateType : long
	{
		ENBState_IsEditorActive = 1,      //is mod editor windows are opened
		ENBState_IsEffectsWndActive = 2,  //is shader effects window of mod editor opened
		ENBState_CursorPosX = 3,          //position of editor cursor (may not be the same as game cursor)
		ENBState_CursorPosY = 4,          //position of editor cursor (may not be the same as game cursor)
		ENBState_MouseLeft = 5,           //mouse key state boolean pressed or not
		ENBState_MouseRight = 6,          //mouse key state boolean pressed or not
		ENBState_MouseMiddle = 7,         //mouse key state boolean pressed or not

		ENBState_FORCEDWORD = 0x7fffffff  //unused
	};

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//exported functions from ENBSeries library
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Returns version of SDK used by the ENBSeries, 1000 means 1.0, 1001 means 1.01, etc
	//Guaranteed compatibility for all Xxxx versions only, for example 1025 will work with sdk version 1000-1025,
	//2025 will work with sdk version 2000-2025, etc. In best cases it's equal to ENBSDKVERSION
	typedef long (*_ENBGetSDKVersion)();

	//Assign callback function which is executed by ENBSeries at certain moments. This helps to bypass potential bugs
	//and increase performance. Argument calltype must be used to select proper action.
	//void WINAPI	CallbackFunction(ENBCallbackType calltype); //example function
	typedef void(WINAPI* ENBCallbackFunction)(ENBCallbackType calltype);  //declaration of callback function
	typedef void (*_ENBSetCallbackFunction)(ENBCallbackFunction func);

	//Receive various states of the mod
	//Return boolean or indexed values, depending from which state requested
	typedef long (*_ENBStateType)(ENBStateType state);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	_ENBGetSDKVersion enbGetSDKVersion = NULL;
	_ENBSetCallbackFunction enbSetCallbackFunction = NULL;
	_ENBStateType enbGetState = NULL;

	void ENBLinkFunctions();
	void WINAPI CallbackFunction(ENBCallbackType calltype);

}
