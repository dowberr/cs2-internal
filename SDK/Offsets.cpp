#include "Offsets.h"


VOID GameModules() 
{
    Manager->GetOverlayDLL();
    Manager->GetEngineDLL();
    Manager->GetClientDLL();

    // Get the base of those 3 DLL's

    Manager->GetOverlaySize();
    Manager->GetEngineSize();
    Manager->GetClientSize();

    // Get the size of DLL's for the pattern scan.

}

