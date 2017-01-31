#pragma once

#ifdef OMULBOMBATORLIB_EXPORTS 
#define OMUL_BOMBATOR_LIB_API __declspec(dllexport)   
#else  
#define OMUL_BOMBATOR_LIB_API __declspec(dllimport)   
#endif

#include "BomberData.h"
#include "BombData.h"
#include "BlockData.h"
#include "PlayerData.h"
#include "UserData.h"
#include "ID.h"

#include <vector>
#include <memory>

OMUL_BOMBATOR_LIB_API typedef std::shared_ptr<PlayerData> PlayerPtr;

OMUL_BOMBATOR_LIB_API typedef std::vector<std::pair<ID, BomberData>> BomberVector;
OMUL_BOMBATOR_LIB_API typedef std::shared_ptr<BomberVector> BombersPtr;

OMUL_BOMBATOR_LIB_API typedef std::vector<std::pair<ID, BombData>> BombVector;
OMUL_BOMBATOR_LIB_API typedef std::shared_ptr<BombVector> BombsPtr;

OMUL_BOMBATOR_LIB_API typedef std::vector<std::pair<ID, BlockData>> BlockVector;
OMUL_BOMBATOR_LIB_API typedef std::shared_ptr<BlockVector> BlocksPtr;

OMUL_BOMBATOR_LIB_API typedef std::vector<std::pair<ID, UserData>> UsersVector;
OMUL_BOMBATOR_LIB_API typedef std::shared_ptr<UsersVector> UsersPtr;
