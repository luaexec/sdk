#ifndef SDK_COMMON_H
#define SDK_COMMON_H

// standard.
#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <windows.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include <chrono>
#include <array>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>
#include <functional>
#include <map>
#include <deque>
#include <mutex>
#include <future>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

// definitives.
#define NS namespace
#define IL inline
#define FIL __forceinline
#define SRT unsigned long __stdcall
#define ERR std::runtime_error
#define IF(x) if (x)

// hack.
#include "other/other.h"

// attachment.
NS A {
	using namespace std;

    IL HINSTANCE hinstDLL;
    IL bool SUCCESS{ false };

    SRT MAIN();
    SRT EXIT();

    FIL void INIT(const string& title, const function<bool()>& fn)
    {
        if (!fn())
            throw ERR( tfm::format( "A::INIT->(%s).", title ) );
    }

    FIL void CATCH( const string& msg, bool exit )
    {
        if ( !exit )
            this_thread::sleep_for( chrono::milliseconds( 100 ) );
    }
};

#endif