//=============================================================================
//
// タイトル画面処理 [game.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum
{
	VILLAGE, // 村
	DUNGEON_FIRST_FLOOR,	// ダンジョン(1階)
	DUNGEON_SECOND_FLOOR,   // ダンジョン(2階)
	FIELD_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetField(int field);
int GetField(void);

BOOL FrustumCulling(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size);

BOOL FieldHit(XMFLOAT3 newPos, XMFLOAT3 oldPos);