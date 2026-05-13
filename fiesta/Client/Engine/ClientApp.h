// Client/Engine/ClientApp.h
// DEPRECATED -- network coordination moved into AccountFrameWork.
// Retained for any tooling that still references it.
// The boot path is: WinMain -> NiApplication::Run() -> ShineApp::Initialize()
//   -> Pgg_kFrameMgr.Start(AccountFrameWorkID)
//   -> AccountFrameWork owns Login/WM/ZoneSession directly.
#ifndef SHINE_CLIENT_ENGINE_CLIENTAPP_H
#define SHINE_CLIENT_ENGINE_CLIENTAPP_H
// See AccountFrameWork.h for the actual network coordination.
#endif
