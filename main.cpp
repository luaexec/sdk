#include "common.h"

SRT A::MAIN( )
{
    while ( !GetModuleHandleA("serverbrowser.dll") )
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

    // init.
    try
    {
    } catch( ERR& x )
    {
        A::CATCH( x.what( ), GetAsyncKeyState(VK_END) );
    }

    // await.
    while ( !GetAsyncKeyState(VK_END) )
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

    FreeLibraryAndExitThread( A::hinstDLL, 0 );
}

SRT A::EXIT( )
{
	return 0;
}

bool APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    // startup.
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        A::hinstDLL = hModule;
        if ( !CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)(A::MAIN), A::hinstDLL, 0, 0 ) )
            return false;
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
        return A::EXIT();

    return true;
}