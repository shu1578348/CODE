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
static DUNGEON	dungeon[DUNGEON_MAX];			 // �_���W�����̍ő吔

//=============================================================================
// ����������
//=============================================================================
HRESULT InitDungeon(void)
{

	for (int i = 0; i < DUNGEON_MAX; i++)
	{
		LoadModel(MODEL_DUNGEON, &dungeon[i].model);
		dungeon[i].load = TRUE;

		dungeon[i].rot = { 0.0f, 0.0f, 0.0f };
		dungeon[i].scl = { 1.0f, 1.0f, 1.0f };

		dungeon[i].size = { DUNGEON_SIZE_X, DUNGEON_SIZE_Y, DUNGEON_SIZE_Z };

		dungeon[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		dungeon[i].use  = TRUE;		// TRUE:�g�p / FALSE:���g�p

	}

	// �_���W�����̕\���ʒu
	{
		dungeon[0].pos = { -220.0f, DUNGEON_OFFSET_Y,  -200.0f };
		dungeon[0].rot = { 0.0f, XM_PI, 0.0f };
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
		if (dungeon[i].load)
		{
			UnloadModel(&dungeon[i].model);

			dungeon[i].load = FALSE;
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
		dungeon[i].draw = FrustumCulling(dungeon[i].pos, dungeon[i].rot, dungeon[i].size);

		//-------------------------------------------------------------------------
		// �t�B�[���h�̕ύX
		//-------------------------------------------------------------------------
		PLAYER* player = GetPlayer();

		XMFLOAT3 pos = dungeon[i].pos;

		pos.x -= sinf(dungeon[i].rot.y) * 30.0f;
		pos.z -= cosf(dungeon[i].rot.y) * 30.0f;

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
		if (!dungeon[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!dungeon[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(dungeon[i].scl.x, dungeon[i].scl.y, dungeon[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(dungeon[i].rot.x, dungeon[i].rot.y + XM_PI, dungeon[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(dungeon[i].pos.x, dungeon[i].pos.y, dungeon[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&dungeon[i].mtxWorld, mtxWorld);

		// ���f���`��
		DrawModel(&dungeon[i].model);

	}

}

//=============================================================================
// �_���W�����̏����擾
//=============================================================================
DUNGEON* GetDungeon(void)
{
	return &dungeon[0];
}