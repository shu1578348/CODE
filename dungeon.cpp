//=============================================================================
//
// ���f������ [dungeon.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "dungeon.h"
#include "player.h"
#include "game.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_DUNGEON		"data/MODEL/dungeon.obj"			// �ǂݍ��ރ��f����

#define DUNGEON_SHADOW_SIZE	  (1.0f)						// �e�̑傫��
#define DUNGEON_OFFSET_Y      (0.0f)						// �_���W�����̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static DUNGEON	g_Dungeon[DUNGEON_MAX];			 // �_���W�����̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDungeon(void)
{

	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		LoadModel(MODEL_DUNGEON, &g_Dungeon[i].model);
		g_Dungeon[i].load = TRUE;

		g_Dungeon[i].rot = { 0.0f, 0.0f, 0.0f };
		g_Dungeon[i].scl = { 1.0f, 1.0f, 1.0f };

		g_Dungeon[i].size = { DUNGEON_SIZE_X, DUNGEON_SIZE_Y, DUNGEON_SIZE_Z };

		g_Dungeon[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		g_Dungeon[i].use  = TRUE;		// TRUE:�g�p / FALSE:���g�p

	}

	// �_���W�����̕\���ʒu
	{
		g_Dungeon[0].pos = { -220.0f, DUNGEON_OFFSET_Y,  -200.0f };
		g_Dungeon[0].rot = { 0.0f, XM_PI, 0.0f };
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitDungeon(void)
{
	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		// ���f���̉������
		if (g_Dungeon[i].load)
		{
			UnloadModel(&g_Dungeon[i].model);

			g_Dungeon[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDungeon(void)
{
	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		//-------------------------------------------------------------------------
		// ������J�����O����
		//-------------------------------------------------------------------------
		g_Dungeon[i].draw = FrustumCulling(g_Dungeon[i].pos, g_Dungeon[i].rot, g_Dungeon[i].size);

		//-------------------------------------------------------------------------
		// �t�B�[���h�̕ύX
		//-------------------------------------------------------------------------
		PLAYER* player = GetPlayer();

		XMFLOAT3 pos = g_Dungeon[i].pos;

		pos.x -= sinf(g_Dungeon[i].rot.y) * 30.0f;
		pos.z -= cosf(g_Dungeon[i].rot.y) * 30.0f;

		if (CollisionBC(pos, player[0].pos, 30.0f, player[0].size))
		{
			// SetField(DUNGEON_FIRST_FLOOR);
			SetField(DUNGEON_FIRST_FLOOR);
		}

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDungeon(void)
{

	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!g_Dungeon[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Dungeon[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Dungeon[i].scl.x, g_Dungeon[i].scl.y, g_Dungeon[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Dungeon[i].rot.x, g_Dungeon[i].rot.y + XM_PI, g_Dungeon[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Dungeon[i].pos.x, g_Dungeon[i].pos.y, g_Dungeon[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Dungeon[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&g_Dungeon[i].model);

	}

}

//=============================================================================
// �_���W�����̏����擾
//=============================================================================
DUNGEON* GetDungeon(void)
{
	return &g_Dungeon[0];
}