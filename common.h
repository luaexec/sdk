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
#define ERR std::runtime_error&
#define IF(x) if (x)

// attachment.
NS A {
    IL HINSTANCE hinstDLL;
    IL bool SUCCESS{ false };

    SRT MAIN();
    SRT EXIT();

    FIL void INIT(const std::string& title, const std::function<bool()>& fn)
    {
        if (!fn())
            throw std::runtime_error( tfm::format( "A::INIT->(%s).", title ) );
    }

    FIL void CATCH( const std::string& msg, bool exit )
    {
        if ( !exit )
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
};

#endif