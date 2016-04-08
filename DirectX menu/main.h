#pragma once

#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>


// MY HEADERS
#include "renderManager.h"
#include "D3Menu.h"

#pragma comment(lib,"Dwmapi.lib")
// GLOBAL VARIABLES
extern rManager			rMGR;
extern D3Menu			d3;
