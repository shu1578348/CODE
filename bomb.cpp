//=============================================================================
//
// �{���̏��� [bomb.cpp]
// Author : �r�R�G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "bomb.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "collision.h"
#include "shadow.h"
#include "particle.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BOMB	"data/MODEL/bomb.obj" // �ǂݍ��ރ��f����

#define BOMB_SHADOW_SIZE	(0.4f)	  // �e�̑傫��
#define BOMB_OFFSET_Y		(0.0f)	  // ���e�̉������킹��

#define EXPLOSION_TIME		(30.0f)	  // ��������
#define EXPLOSION_RANGE		(100.0f)  // �����͈�
#define EXPLOSION_DAMAGE	(10.0f)   // �����ɂ��_���[�W

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BOMB	g_Bomb[BOMB_MAX];	// ���e�̍ő吔
static BOOL	g_Load = FALSE;		// ���f���̏������t���O
DX11_MODEL modelBomb;		    // ���f�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBomb(void)
{
	//-------------------------------------------------------------------------
	// ���̏�����
	//-------------------------------------------------------------------------
	for (int i = 0; i < BOMB_MAX; i++)
	{
		g_Bomb[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Bomb[i].scl  = { 0.8f, 0.8f, 0.8f };
		g_Bomb[i].size = { BOMB_SIZE_X, BOMB_SIZE_Y, BOMB_SIZE_Z };

		g_Bomb[i].modelNo = 0; // �\�����f���̎��

		g_Bomb[i].exTime = EXPLOSION_TIME; // ��������

		g_Bomb[i].exFlag = FALSE; // TRUE:���� / FALSE:������

		g_Bomb[i].draw = TRUE;	  // TRUE:�\�� / FALSE:��\��
		g_Bomb[i].use  = TRUE;	  // TRUE:�g�p / FALSE:���g�p
	}

	//-------------------------------------------------------------------------
	// ���f���̓ǂݍ���
	//-------------------------------------------------------------------------
	{	
		g_Load = TRUE;

		// ���f���̓ǂݍ���
		LoadModel(MODEL_BOMB, &modelBomb);
	}

	//-------------------------------------------------------------------------
	// �ʒu�̐ݒ�
	//-------------------------------------------------------------------------
	{
		g_Bomb[0].pos = { -130.0f, 1.0f,  -90.0f };
		g_Bomb[1].pos = { -500.0f, 1.0f,  635.0f };
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBomb(void)
{
	if (g_Load)
	{
		UnloadModel(&modelBomb);

		g_Load = FALSE;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBomb(void)
{
	//-------------------------------------------------------------------------
	// ���̎擾
	//-------------------------------------------------------------------------
	BULLET * bullet = GetBullet(); // �o���b�g
	PLAYER * player = GetPlayer(); // �v���C���[
	ENEMY  * enemy  = GetEnemy();  // �G�l�~�[

	for (int i = 0; i < BOMB_MAX; i++)
	{
		if (g_Bomb[i].use)
		{
			//-------------------------------------------------------------------------
			// ������J�����O
			//-------------------------------------------------------------------------
			{
				g_Bomb[i].draw = FrustumCulling(g_Bomb[i].pos, g_Bomb[i].rot, g_Bomb[i].size);
			}

			//-------------------------------------------------------------------------
			// �o���b�g�̓����蔻��
			//-------------------------------------------------------------------------
			{
				for (int j = 0; j < BULLET_MAX; j++)
				{
					if (bullet[j].use)
					{
						if (CollisionBC(g_Bomb[i].pos, bullet[j].pos, BOMB_SIZE, BULLET_SIZE))
						{
							bullet[j].use = FALSE;

							ReleaseShadow(bullet[j].shadowIdx);

							// �i�C�t(��)�Ȃ甚������
							if (bullet[j].type == 1)
							{
								g_Bomb[i].exFlag = TRUE;
							}
						}
					}
				}
			}

			//-------------------------------------------------------------------------
			// �������̏���
			//-------------------------------------------------------------------------
			{
				if ((g_Bomb[i].exFlag) && (g_Bomb[i].use))
				{
					//-----------------------------------------------------------------
					// �����͈͂̓����蔻��
					//-----------------------------------------------------------------
					for (int k = 0; k < ENEMY_MAX; k++)
					{
						if (CollisionBC(g_Bomb[i].pos, enemy[k].pos, EXPLOSION_RANGE, ENEMY_SIZE))
						{
							// �G���R����
							enemy[k].condition = 1;

							enemy[k].hp -= EXPLOSION_DAMAGE;
						}
					}

					//-----------------------------------------------------------------
					// �����G�t�F�N�g����
					//-----------------------------------------------------------------
					for (int l = 0; l < 30; l++)
					{
						XMFLOAT3 pos;
						XMFLOAT3 move;
						XMFLOAT3 scale;
						float fSize;
						float size;
						int nLife;

						pos = g_Bomb[i].pos;

						scale = { 1.0f, 1.0f, 1.0f };

						size = (rand() % 100 + 5.0f) * 0.01f;

						scale = { size, size, size };

						move.x = (300.0f - (rand() % 600)) * 0.015f;
						move.y = (300.0f - (rand() % 600)) * 0.015f;
						move.z = (300.0f - (rand() % 600)) * 0.015f;

						nLife = rand() % 50 + 100;

						fSize = 0.0f;

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);
					}

					//-----------------------------------------------------------------
					// �������Ԍ���
					//-----------------------------------------------------------------
					g_Bomb[i].exTime--;

				}

				//-----------------------------------------------------------------
				// �����̏I������
				//-----------------------------------------------------------------
				if (g_Bomb[i].exTime < 0)
				{
					g_Bomb[i].exFlag = FALSE;

					g_Bomb[i].use = FALSE;
				}

			}

		}

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBomb(void)
{

	for (int i = 0; i < BOMB_MAX; i++)
	{
		// ���g�p
		if (!g_Bomb[i].use)  continue;

		// ��\��
		if (!g_Bomb[i].draw) continue;

		// ��\��
		if (g_Bomb[i].exFlag) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Bomb[i].scl.x, g_Bomb[i].scl.y, g_Bomb[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Bomb[i].rot.x, g_Bomb[i].rot.y + XM_PI, g_Bomb[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Bomb[i].pos.x, g_Bomb[i].pos.y, g_Bomb[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Bomb[i].mtxWorld, mtxWorld);

		// ���f���`��
		switch (g_Bomb[i].modelNo)
		{
		case 0:
			DrawModel(&modelBomb);
			break;
		}

	}

}

//=============================================================================
// ���e�̏����擾
//=============================================================================
BOMB* GetBomb(void)
{
	return &g_Bomb[0];
}