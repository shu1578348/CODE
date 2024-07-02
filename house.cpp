//=============================================================================
//
// ���f������ [house.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "house.h"
#include "player.h"
#include "particle.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �ǂݍ��ރ��f����
#define	HOUSE_HIGH			"data/MODEL/house_high.obj"	
#define	HOUSE_MIDDLE		"data/MODEL/house_middle.obj"	
#define HOUSE_LOW			"data/MODEL/house_low.obj"

#define HOUSE_SHADOW_SIZE	(0.4f)	// �e�̑傫��
#define HOUSE_OFFSET_Y		(0.0f)	// �Ƃ̉������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static HOUSE g_House[HOUSE_MAX]; // �Ƃ̍ő吔

DX11_MODEL modelHigh;    // �������f�����
DX11_MODEL modelMiddle;  // �����x�̃��f�����
DX11_MODEL modelLow;     // �Ⴂ���f�����

static BOOL	g_Load = FALSE;		// ���������s�������̃t���O

//=============================================================================
// ����������
//=============================================================================
HRESULT InitHouse(void)
{
	//-------------------------------------------------------------------------
	//�@���̏�����
	//-------------------------------------------------------------------------
	for (int i = 0; i < HOUSE_MAX; i++)
	{

		g_House[i].pos = { -200.0f, HOUSE_OFFSET_Y, 300.0f };

		g_House[i].rot = { 0.0f, 0.0f, 0.0f };
		g_House[i].scl = { 2.0f, 2.0f, 2.0f };

		g_House[i].size = { HOUSE_SIZE_X, HOUSE_SIZE_Y, HOUSE_SIZE_Z };

		g_House[i].draw = TRUE;	// TRUE:�\�� / FALSE:��\��
		g_House[i].use  = TRUE;	// TRUE:�g�p / FALSE:���g�p

		g_House[i].modelNo = 0;

	}

	//-------------------------------------------------------------------------
	//�@���f���̓ǂݍ���
	//-------------------------------------------------------------------------
	{
		LoadModel(HOUSE_HIGH, &modelHigh);
		LoadModel(HOUSE_MIDDLE, &modelMiddle);
		LoadModel(HOUSE_LOW, &modelLow);
	}

	//-------------------------------------------------------------------------
	//�@�ʒu�ɐݒ�
	//-------------------------------------------------------------------------
	{
		g_House[0].pos = { -200.0f, HOUSE_OFFSET_Y, 300.0f };
		g_House[1].pos = {    0.0f, HOUSE_OFFSET_Y, 150.0f };
		g_House[2].pos = {  200.0f, HOUSE_OFFSET_Y, 0.0f   };
	}

	g_Load = TRUE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHouse(void)
{
	//-------------------------------------------------------------------------
	// ���f���̉������
	//-------------------------------------------------------------------------
	if (g_Load)
	{
		UnloadModel(&modelHigh);
		UnloadModel(&modelMiddle);
		UnloadModel(&modelLow);

		g_Load = FALSE;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHouse(void)
{
	//-------------------------------------------------------------------------
	// OBJ���̎擾
	//-------------------------------------------------------------------------
	PLAYER* player = GetPlayer();

	//-------------------------------------------------------------------------
	// �\��/��\���̏���
	//-------------------------------------------------------------------------
	for (int i = 0; i < HOUSE_MAX; i++)
	{

		float distance = GetDistance(player[0].pos, g_House[i].pos);

		//-----------------------------------------------------------------------
		// LOD����
		//-----------------------------------------------------------------------
		if (distance < DRAW_RANGE - 400)
		{
			g_House[i].modelNo = 0;
		}
		else if (distance < DRAW_RANGE - 200)
		{
			g_House[i].modelNo = 1;
		}
		else if (distance > DRAW_RANGE)
		{
			g_House[i].modelNo = 2;
		}

		//-------------------------------------------------------------------------
		// �����^�J�����O����
		//-------------------------------------------------------------------------		
		g_House[i].draw = FrustumCulling(g_House[i].pos, g_House[i].rot, g_House[i].size);
		
		
		//-------------------------------------------------------------------------
		// �G�t�F�N�g�̏���(�p�[�e�B�N��)
		//-------------------------------------------------------------------------
		if (g_House[i].draw)
		{
			if (rand() % 1 == 0)
			{
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				int nLife;
				float fSize;

				pos = g_House[i].pos;

				pos.y = 150.0f;

				pos.x -= 30.0f;

				pos.z += 20.0f;

				scale = { 0.6f, 0.6f, 0.6f };

				fAngle = (float)(rand() % 628 - 314) / 150;
				fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 80;

				move.x = sinf(fAngle) * fLength;
				move.y = 1.2f;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 300 + 80;

				fSize = (float)(rand() % 30 + 20);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(0.7f, 0.7f, 0.7f, 0.2f), fSize, fSize, nLife, 0);
			}

		}

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHouse(void)
{

	for (int i = 0; i < HOUSE_MAX; i++)
	{

		if (!g_House[i].use)  continue;

		if (!g_House[i].draw) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_House[i].scl.x, g_House[i].scl.y, g_House[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_House[i].rot.x, g_House[i].rot.y + XM_PI, g_House[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_House[i].pos.x, g_House[i].pos.y, g_House[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_House[i].mtxWorld, mtxWorld);

		// ���f���`��
		
		// LOD����
		switch (g_House[i].modelNo)
		{
		case 0:
			DrawModel(&modelHigh);
			break;

		case 1:
			DrawModel(&modelMiddle);
			break;

		case 2:
			DrawModel(&modelLow);
			break;
		}
		
	}

}

//=============================================================================
// �Ə����擾
//=============================================================================
HOUSE* GetHouse(void)
{
	return &g_House[0];
}

