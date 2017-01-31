#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include "UserNetworkData.h"
#include "Command.h"

struct OMUL_BOMBATOR_LIB_API UserCommands
{
	UserCommands();

	Command::CMD commands;
	UserNetworkData networkData;
};