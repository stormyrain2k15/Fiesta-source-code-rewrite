// Client/Engine/Main.cpp
// WinMain entry. Constructs ClientApp and runs it.
#include "ClientApp.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    fiesta::ClientApp app;
    if (!app.Init(hInst, "Fiesta")) return 1;
    app.ConnectLogin("127.0.0.1", 9010);
    return app.Run();
}
