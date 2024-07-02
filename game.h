//=============================================================================
//
// �^�C�g����ʏ��� [game.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum
{
	VILLAGE, // ��
	DUNGEON_FIRST_FLOOR,	// �_���W����(1�K)
	DUNGEON_SECOND_FLOOR,   // �_���W����(2�K)
	FIELD_MAX
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetField(int field);
int GetField(void);

BOOL FrustumCulling(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size);

BOOL FieldHit(XMFLOAT3 newPos, XMFLOAT3 oldPos);