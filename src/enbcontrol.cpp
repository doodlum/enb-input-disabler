#include "enbcontrol.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Handle ENB callback
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ENBControl::Update()
{
	auto controlMap = RE::ControlMap::GetSingleton();

	auto IsEditorActive = ENBSDK::enbGetState(ENBSDK::ENBState_IsEditorActive);

	if (controlMap)
		controlMap->GetRuntimeData().ignoreKeyboardMouse = IsEditorActive;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Called by ENBSeries. Please do not read or set parameters outside of it
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void WINAPI ENBSDK::CallbackFunction(ENBCallbackType calltype)
{
	if (calltype == ENBCallback_BeginFrame) ENBControl::Update();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Get ENBSeries dll and attach all functions to it
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ENBSDK::ENBLinkFunctions()
{
	DWORD cb = 1000 * sizeof(HMODULE);
	DWORD cbNeeded = 0;
	HMODULE enbmodule = NULL;
	HMODULE hmodules[1000];
	HANDLE hproc = GetCurrentProcess();

	for (long i = 0; i < 1000; i++) {
		hmodules[i] = NULL;
	}

	if (EnumProcessModules(hproc, hmodules, cb, &cbNeeded)) {
		long count = cbNeeded / sizeof(HMODULE);
		for (long i = 0; i < count; i++) {
			if (hmodules[i] == NULL)
				break;
			void* func = (void*)GetProcAddress(hmodules[i], "ENBGetSDKVersion");
			if (func) {
				enbmodule = hmodules[i];
				break;
			}
		}
	}

	if (!enbmodule)
		return;

	enbGetSDKVersion = (_ENBGetSDKVersion)GetProcAddress(enbmodule, "ENBGetSDKVersion");
	enbSetCallbackFunction = (_ENBSetCallbackFunction)GetProcAddress(enbmodule, "ENBSetCallbackFunction");
	enbGetState = (_ENBStateType)GetProcAddress(enbmodule, "ENBGetState");

	enbSetCallbackFunction(CallbackFunction);
}
