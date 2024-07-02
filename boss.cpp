//=============================================================================
//
// �{�X���� [boss.cpp]
// Author : �r�R �G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "shadow.h"
#include "player.h"
#include "camera.h"
#include "collision.h"
#include "bullet.h"
#include "particle.h"
#include "fade.h"
#include "score.h"
#include "game.h"
#include "bomb.h"
#include "boss.h"
#include "enemybullet.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BOSS_BODY	 "data/MODEL/boss/boss_body.obj"	// �ǂݍ��ރ��f����
#define MODEL_BOSS_HEAD	 "data/MODEL/boss/boss_head.obj"

#define BOSS_HP				(500.0f)		 // �{�X��HP

#define	VALUE_MOVE			(0.1f)			 // �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.1f)	 // ��]��
#define BOSS_SHADOW_SIZE	(0.8f)			 // �e�̑傫��
#define BOSS_OFFSET_Y		(2.0f)			 // �{�X�̑��������킹��

#define PARTICLE_ROTATION   (6.0f)			 // �p�[�e�B�N���̉�]��
#define MODE_CHANGE			(200)			 // �U�����[�h�̐؂�ւ�����
#define CIRCLE_SPEED        (0.5f)           // �~��U�����L����X�s�[�h
#define BLESS_DAMAGE		(1)				 // �u���X�̍U����
#define CIRCLE_DAMAGE		(2)				 // �~��U���̍U����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BOSS boss[BOSS_MAX];	 // �{�X�\����
static BOOL	  load = FALSE;	 // ���������s�������̃t���O
DX11_MODEL	  modelBossBody;	 // ���f�����
DX11_MODEL	  modelBossHead;	 // ���f�����

float pCnt  = 0;	// �p�[�e�B�N���̉�]���x
float pRad  = 0;	// �p�[�e�B�N���̔��a
int clBTime = 0;	// �o���b�g�̘A�ˊԊu
int clCTime = 0;	// mode�̐؂�ւ�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(void)
{
	//-------------------------------------------------------------------------
	// ���̏�����
	//-------------------------------------------------------------------------
	for (int i = 0; i < BOSS_MAX; i++)
	{
		boss[i].pos = XMFLOAT3(175.0f, BOSS_OFFSET_Y, -30.0f);
		boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		boss[i].scl = XMFLOAT3(2.5f, 2.5f, 2.5f);

		boss[i].tPos = XMFLOAT3(175.0f, BOSS_OFFSET_Y, -30.0f);

		boss[i].spd  = 0.0f;		// �ړ��X�s�[�h�N���A
		boss[i].size = {BOSS_SIZE_X, BOSS_SIZE_Y, BOSS_SIZE_Z};	// �����蔻��̑傫��

		boss[i].aRot = XMFLOAT3(0.0f, -0.5f, 0.0f); // �U������
		
		boss[i].mode = 1;		    // �U���̎��

		boss[i].draw  = TRUE;		// TRUE:�\��   / FALSE:��\��
		boss[i].use   = TRUE;		// TRUE:�g�p   / FALSE:���g�p
		boss[i].chase = FALSE;    // TRUE:�ǐՒ� / FALSE:��ǐՒ�

		boss[i].condition = 0;	// �ʏ��Ԃɂ���
		
		boss[i].hp = BOSS_HP;     // �q�b�g�|�C���g�̏�����

		boss[i].state = FALSE;    // ���
		
	}
	//-------------------------------------------------------------------------
	// �����ʒu�̐ݒ�
	//-------------------------------------------------------------------------
	{
		boss[0].pos = XMFLOAT3(-220.0f, BOSS_OFFSET_Y, -0.0f);
	}

	//-------------------------------------------------------------------------
	// ���f���ǂݍ���
	//-------------------------------------------------------------------------
	{
		load = TRUE;

		// ���f���̓ǂݍ���
		LoadModel(MODEL_BOSS_BODY, &modelBossBody);
		LoadModel(MODEL_BOSS_HEAD, &modelBossHead);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{
	if (load)
	{
		UnloadModel(&modelBossBody);
		UnloadModel(&modelBossHead);
		load = FALSE;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
	//-------------------------------------------------------------------------
	// ���̎擾
	//-------------------------------------------------------------------------
	PLAYER * player = GetPlayer();  // �v���C���[
	BULLET * bullet = GetBullet();  // �o���b�g
	CAMERA * cam    = GetCamera();  // �J����

	for (int i = 0; i < BOSS_MAX; i++)
	{

		if (boss[i].use)	  // �g�p����
		{
			// �ړ��O�̍��W��ۑ�
			XMFLOAT3 oldPos = boss[i].pos;
						
			// �e���{�X�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = boss[i].pos;
			pos.y -= (BOSS_OFFSET_Y - 0.1f);
			SetPositionShadow(boss[i].shadowIdx, pos);

			//-------------------------------------------------------------------------------
			// ������J�����O
			//-------------------------------------------------------------------------------
			{
				boss[i].draw = FrustumCulling(boss[i].pos, boss[i].rot, boss[i].size);
			}
		
			//-------------------------------------------------------------------------------
			// �ړ�����
			//-------------------------------------------------------------------------------
			{
				boss[i].spd = VALUE_MOVE;

				XMVECTOR epos = XMLoadFloat3(&boss[i].pos);
				XMVECTOR vec  = XMLoadFloat3(&player[0].pos) - epos;  // �G�l�~�[�ƃv���C���[�̍��������߂Ă���

				// �ڕW�̕���
				boss[i].rot.y = static_cast<float>(atan2(vec.m128_f32[0], vec.m128_f32[2]) - XM_PI);

				boss[i].pos.x -= sinf(boss[i].rot.y) * boss[i].spd;
				boss[i].pos.z -= cosf(boss[i].rot.y) * boss[i].spd;
			}

			//-------------------------------------------------------------------------
			// �U������
			//-------------------------------------------------------------------------
			{
				// �U�����[�h�؂�ւ����Ԃ̌���
				clCTime++;

				pCnt -= PARTICLE_ROTATION; // �p�[�e�B�N����]�p

				// ���[�h�ؑ֏���
				if (clCTime >= MODE_CHANGE)
				{
					boss[i].mode = (boss[i].mode + 1) % 3;

					boss[i].aRot.y = boss[i].rot.y - 0.5f;

					pRad = 0;

					clCTime = 0;
				}

				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				int nLife;
				float fSize;

				switch (boss[i].mode)
				{
				case 0:

					// �o���b�g�U���̃N�[���^�C��
					clBTime++;

					boss[i].aRot.y = boss[i].rot.y;

					if (clBTime >= 10)
					{
						// �o���b�g�ɂ��U��
						SetEnemyBullet(boss[i].pos, boss[i].rot, 0);

						clBTime = 0;
					}

					break;

				case 1:

					// �u���X�̔��˕���
					if (boss[i].aRot.y <= 0.5)
					{
						boss[i].aRot.y += 0.005f;
					}

					for (int j = 0; j <= 10; j++)
					{
						pos = boss[i].pos;

						pos.y +=  80.0f;

						pos.x -= sinf(boss[i].aRot.y) * j;
						pos.z -= cosf(boss[i].aRot.y) * j;

						pos.x -= sinf(boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.5f * j;
						pos.z -= cosf(boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.5f * j;
						pos.y -= cosf(boss[i].aRot.z + pCnt + (XM_PI * 0.2f * j)) * 0.5f * j;

						scale = { 0.25f, 0.25f, 0.25f };

						move.x = -sinf(boss[i].aRot.y) * 8.0f;
						move.z = -cosf(boss[i].aRot.y) * 8.0f;
						move.y = -2.5f;

						nLife = rand() % 100 + 150;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.2f, 1.0f, 1.0f), fSize, fSize, nLife, 2);

						pos = boss[i].pos;

						pos.y = boss[i].pos.y + 80.0f;

						pos.x -= sinf(boss[i].aRot.y) * j;
						pos.z -= cosf(boss[i].aRot.y) * j;

						pos.x -= sinf(boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.6f * j;
						pos.z -= cosf(boss[i].aRot.y + pCnt + (XM_PI * 0.2f * j)) * 0.6f * j;
						pos.y -= cosf(boss[i].aRot.z + pCnt + (XM_PI * 0.2f * j)) * 0.6f * j;

						scale = { 0.15f, 0.15f, 0.15f };

						move.x = -sinf(boss[i].aRot.y) * 8.0f;
						move.z = -cosf(boss[i].aRot.y) * 8.0f;
						move.y = -2.5f;

						nLife = rand() % 100 + 150;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.5f, 1.0f, 1.0f), fSize, fSize, nLife, 2);

					}
					break;

				case 2:

					pRad += CIRCLE_SPEED; // �~���L����

					for (int j = 0; j <= 20; j++)
					{

						pos = boss[i].pos;

						pos.x -= sinf(boss[i].rot.y + pCnt + (XM_PI * 0.1f * j)) * (100.0f + pRad);
						pos.z -= cosf(boss[i].rot.y + pCnt + (XM_PI * 0.1f * j)) * (100.0f + pRad);

						scale = { 0.3f, 0.3f, 0.3f };

						fAngle = (float)(rand() % 628 - 314) / 20;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 8;

						move.x = sinf(fAngle) * fLength;
						move.y = (rand() % 300 / 100.0f + 10) / 10;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 80 + 100;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);

					}

					for (int j = 0; j <= 20; j++)
					{

						pos = boss[i].pos;

						pos.x -= sinf(boss[i].rot.y - pCnt + (XM_PI * 0.1f * j)) * (95.0f + pRad);
						pos.z -= cosf(boss[i].rot.y - pCnt + (XM_PI * 0.1f * j)) * (95.0f + pRad);

						scale = { 0.3f, 0.3f, 0.3f };

						fAngle = (float)(rand() % 628 - 314) / 20;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 8;

						move.x = sinf(fAngle) * fLength;
						move.y = (rand() % 300 / 100.0f + 10) / 20;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 80 + 100;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);

					}

					break;

				}


			}

			//-------------------------------------------------------------------------
			// �U���̓����蔻�菈��
			//-------------------------------------------------------------------------
			{

				XMFLOAT3 aPos;
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				int nLife;
				float fSize;

				switch (boss[i].mode)
				{
				case 0:
					break;

				case 1: // �u���X

					aPos = boss[i].pos;

					aPos.x -= sinf(boss[i].aRot.y - 0.1f) * 230.0f;
					aPos.z -= cosf(boss[i].aRot.y - 0.1f) * 230.0f;

					for (int n = 0; n < PLAYER_MAX; n++)
					{
						if (player[n].use == TRUE)
						{
							if (CollisionBC(aPos, player[0].pos, 40.0f, player[0].size))
							{
								// �v���C���[�ւ̃_���[�W
								player[0].hp -= BLESS_DAMAGE;

								// �ڐG�G�t�F�N�g
								pos = player[0].pos;

								scale = { 0.3f, 0.3f, 0.3f };

								fAngle = (float)(rand() % 628 - 314) * 0.1f;
								fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.25f;

								move.x = sinf(fAngle) * fLength;
								move.y = (rand() % 300 / 100.0f + 10) * 0.1f;
								move.z = cosf(fAngle) * fLength;

								nLife = rand() % 80 + 80;

								fSize = (float)(rand() % 30 + 10);

								// �r���{�[�h�̐ݒ�
								SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);
							}

						}

					}

					break;

				case 2: // �~��U��

					for (int n = 0; n < PLAYER_MAX; n++)
					{
						if (player[n].use == TRUE)
						{
							if (CollisionBC(boss[i].pos, player[0].pos, 100.0f + pRad, player[0].size))
							{
								if (CollisionBC(boss[i].pos, player[0].pos, 90.0f + pRad, player[0].size) == FALSE)
								{

									player[0].hp -= CIRCLE_DAMAGE;

									// �ڐG�G�t�F�N�g
									for (int k = 0; k < 3; k++)
									{
										pos = player[0].pos;

										scale = { 0.3f, 0.3f, 0.3f };

										fAngle = (float)(rand() % 628 - 314) * 0.1f;
										fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.25f;

										move.x = sinf(fAngle) * fLength;
										move.y = (rand() % 300 / 100.0f + 10) * 0.1f;
										move.z = cosf(fAngle) * fLength;

										nLife = rand() % 80 + 80;

										fSize = (float)(rand() % 30 + 10);

										// �r���{�[�h�̐ݒ�
										SetParticle(pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);
									}


								}

							}

						}

					}

					break;

				}

			}

			//-------------------------------------------------------------------------
			// �G�t�F�N�g����(�p�[�e�B�N��)
			//-------------------------------------------------------------------------
			{
				// �ڂ̃G�t�F�N�g
				XMFLOAT3 pos;
				XMFLOAT3 move;
				XMFLOAT3 scale;
				float fAngle, fLength;
				float fSize;
				int nLife;

				pos = boss[i].pos;

				pos.y = 90.0f;

				pos.x -= sinf(boss[i].aRot.y - 0.1f) * 50.0f;
				pos.z -= cosf(boss[i].aRot.y - 0.1f) * 50.0f;

				scale = { 0.25f, 0.25f, 0.25f };
				fAngle = (float)(rand() % 628 - 314) / 50;
				fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 30;

				move.x = sinf(fAngle) * fLength;
				move.y = (rand() % 300 / 100.0f + 10) / 15;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 80 + 80;

				fSize = (float)(rand() % 30 + 10);

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f), fSize, fSize, nLife, 1);

				pos = boss[i].pos;

				pos.y = 90.0f;

				pos.x -= sinf(boss[i].aRot.y + 0.2f) * 50.0f;
				pos.z -= cosf(boss[i].aRot.y + 0.2f) * 50.0f;

				// �r���{�[�h�̐ݒ�
				SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.1f, 1.0f), fSize, fSize, nLife, 1);
			}


			{ // mode0�̂Ƃ��̃u���X�����߂�G�t�F�N�g

				XMFLOAT3 tPos = boss[i].pos;
				tPos.y += 75.0f;
				float radius = 70.0f;
				tPos.x -= sinf(boss[i].aRot.y) * 40.0f;
				tPos.z -= cosf(boss[i].aRot.y) * 40.0f;

				if (boss[i].mode == 0)
				{
					// �A�t�B���ϊ��s��̍쐬
					XMMATRIX transformMatrix = XMMatrixTranslation(tPos.x, tPos.y, tPos.z); // ���s�ړ��s��

					// �����ʒu�̃����_���ȋ���ݒ�
					float u = static_cast<float>(rand()) / RAND_MAX; // [0, 1]
					float v = static_cast<float>(rand()) / RAND_MAX; // [0, 1]
					float theta = 2 * XM_PI * u;  // [0, 2pi]
					float phi = acosf(2 * v - 1); // [0, pi]

					// �ɍ��W�n���璼�����W�n�֕ϊ�
					float x = radius * sinf(phi) * cosf(theta);
					float y = radius * sinf(phi) * sinf(theta);
					float z = radius * cosf(phi);

					// �����ʒu���A�t�B���ϊ����ĖڕW�ʒu���v�Z
					XMVECTOR initialPos = XMVectorSet(x, y, z, 1.0f); // �����ʒu
					initialPos = XMVector4Transform(initialPos, transformMatrix); // �����ʒu��ϊ�

					// �ڕW�ʒu�Ɍ������ړ��x�N�g���̌v�Z
					XMVECTOR direction = XMVectorSubtract(XMLoadFloat3(&tPos), initialPos); // �ڕW�ʒu���珉���ʒu������
					XMVECTOR normalizedDirection = XMVector3Normalize(direction); // ���K��
					XMFLOAT3 move;
					XMStoreFloat3(&move, normalizedDirection);

					// move ��1.5�{�ɂ���
					move.x *= 1.5f;
					move.y *= 1.5f;
					move.z *= 1.5f;

					// ���̑��̃p�[�e�B�N�������̐ݒ�
					XMFLOAT3 scale = { 0.2f, 0.2f, 0.2f };
					int nLife = rand() % 150 + 120;
					float fSize = static_cast<float>(rand() % 30 + 10);

					// �r���{�[�h�̐ݒ�
					SetParticle(XMFLOAT3(initialPos.m128_f32[0], initialPos.m128_f32[1], initialPos.m128_f32[2]), tPos, move, scale, XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f), fSize, fSize, nLife, 3);

				}

			}

			//-------------------------------------------------------------------------
			// ���S���菈��
			//-------------------------------------------------------------------------
			{
				if (boss[i].hp <= 0)
				{
					boss[i].use = FALSE;
					SetFade(FADE_OUT, MODE_RESULT);
					AddScore(1000);
				}
			}
			
		}

	}

#ifdef _DEBUG

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < BOSS_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!boss[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!boss[i].draw) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(boss[i].scl.x, boss[i].scl.y, boss[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(boss[i].rot.x, boss[i].rot.y + XM_PI, boss[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(boss[i].pos.x, boss[i].pos.y, boss[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&boss[i].mtxWorld, mtxWorld);

		// �̂̕`��
		switch (boss[i].modelNo)
		{
		case 0:
			DrawModel(&modelBossBody);
			break;
		}

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(boss[i].scl.x, boss[i].scl.y, boss[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(boss[i].aRot.x, boss[i].aRot.y + XM_PI, boss[i].aRot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(boss[i].pos.x, boss[i].pos.y, boss[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&boss[i].mtxWorld, mtxWorld);

		// ���̕`��
		switch (boss[i].modelNo)
		{
		case 0:
			DrawModel(&modelBossHead);
			break;
		}

	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �{�X�̎擾
//=============================================================================
BOSS* GetBoss()
{
	return &boss[0];
}