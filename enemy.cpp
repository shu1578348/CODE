//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "enemy.h"
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

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/enemy.obj"		// �ǂݍ��ރ��f����

#define ENEMY_HP			(500.0f)					// �G�l�~�[��HP

#define	VALUE_MOVE			(1.2f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.1f)				// ��]��
#define ENEMY_SHADOW_SIZE	(0.8f)						// �e�̑傫��
#define ENEMY_OFFSET_Y		(2.0f)						// �G�l�~�[�̑��������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY enemy[ENEMY_MAX]; // �G�l�~�[�\����
static BOOL	  load = FALSE;	 // ���������s�������̃t���O
DX11_MODEL	  modelEnemy;		 // ���f�����

//----------------------------------------------------------------------------
// ���`���
//----------------------------------------------------------------------------
static INTERPOLATION_DATA moveTbl0[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f,  1.57f, 0.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA moveTbl1[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f,  1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
};

static INTERPOLATION_DATA moveTbl2[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y,    0.0f), XMFLOAT3(0.0f, 3.14f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -300.0f), XMFLOAT3(0.0f, 0.0f,  0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA moveTbl3[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
};

static INTERPOLATION_DATA moveTbl4[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA moveTbl5[] = {
	//���W													��]��						�g�嗦			����

	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA moveTbl6[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA* moveTblAdr[] =
{
	moveTbl0,
	moveTbl1,
	moveTbl2,
	moveTbl3,
	moveTbl4,
	moveTbl5,
	moveTbl6,
};

int atClTime = 0;	// �U���Ԋu

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	//-------------------------------------------------------------------------
	// ���̏�����
	//-------------------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].pos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);
		enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		enemy[i].scl = XMFLOAT3(1.5f, 1.5f, 1.5f);

		enemy[i].tPos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);

		enemy[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		enemy[i].size = {ENEMY_SIZE_X, ENEMY_SIZE_Y, ENEMY_SIZE_Z};	// �����蔻��̑傫��
		
		// �_���W����(1�K)�Ȃ�
		if (GetField() == DUNGEON_FIRST_FLOOR)
		{
			XMFLOAT3 pos = enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		}

		enemy[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		enemy[i].use  = TRUE;		// TRUE:�g�p / FALSE:���g�p
		enemy[i].chase = FALSE;    // TRUE:�ǐՒ� / FALSE:��ǐՒ�

		enemy[i].condition = 0;	// �ʏ��Ԃɂ���
		
		enemy[i].hp = ENEMY_HP; // �q�b�g�|�C���g�̏�����

		enemy[i].state = FALSE;   // TRUE:�ړ��\ / FALSE:�ړ��s��
		
		enemy[i].time  = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		enemy[i].tblNo = 0;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[i].tblMax = sizeof(moveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	}
	//-------------------------------------------------------------------------
	// �����ʒu�̐ݒ�
	//-------------------------------------------------------------------------
	{
		enemy[0].pos = XMFLOAT3(-220.0f, ENEMY_OFFSET_Y, -0.0f);
		enemy[1].pos = XMFLOAT3(-120.0f, ENEMY_OFFSET_Y, -100.0f);
		enemy[2].pos = XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 0.0f);
		enemy[3].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f);
		enemy[4].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f);
		enemy[5].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f);
		enemy[6].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f);
	}

	//-------------------------------------------------------------------------
	// �s���e�[�u���̃Z�b�g
	//-------------------------------------------------------------------------
	{
		enemy[0].tblNo = 0;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[0].tblMax = sizeof(moveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		enemy[1].tblNo = 1;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[1].tblMax = sizeof(moveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		enemy[2].tblNo = 2;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[2].tblMax = sizeof(moveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		enemy[3].tblNo = 3;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[3].tblMax = sizeof(moveTbl3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		enemy[4].tblNo = 4;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[4].tblMax = sizeof(moveTbl4) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		enemy[5].tblNo = 5;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[5].tblMax = sizeof(moveTbl5) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		enemy[6].tblNo = 6;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		enemy[6].tblMax = sizeof(moveTbl6) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	}

	//-------------------------------------------------------------------------
	// ���f���ǂݍ���
	//-------------------------------------------------------------------------
	{
		load = TRUE;

		// ���f���̓ǂݍ���
		LoadModel(MODEL_ENEMY, &modelEnemy);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (load)
	{
		UnloadModel(&modelEnemy);
		load = FALSE;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	//-------------------------------------------------------------------------
	// OBJ���̎擾
	//-------------------------------------------------------------------------
	PLAYER* player = GetPlayer(); // �v���C���[
	BULLET* bullet = GetBullet(); // �o���b�g
	CAMERA* cam = GetCamera();    // �J����

	for (int i = 0; i < ENEMY_MAX; i++)
	{

		if (enemy[i].use)	  // �g�p����
		{
			// �ړ��O�̍��W��ۑ�
			XMFLOAT3 oldPos = enemy[i].pos;
						
			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(enemy[i].shadowIdx, pos);

			//-------------------------------------------------------------------------
			// ������J�����O
			//-------------------------------------------------------------------------
			{
				enemy[i].draw = FrustumCulling(enemy[i].pos, enemy[i].rot, enemy[i].size);
			}

			//-------------------------------------------------------------------------
			// �ړ�����
			//-------------------------------------------------------------------------
			{

				float distance = GetDistance(player[0].pos, enemy[i].pos);

				// �߂Â�����ǐՂ���
				if (distance < 100)
				{
					enemy[i].chase = TRUE;
				}
				else
				{
					enemy[i].chase = FALSE;
				}

				// �_���[�W���󂯂���v���C���[��ǐՂ���
				if (enemy[i].hp != ENEMY_HP)
				{
					enemy[i].chase = TRUE;
				}

				if (enemy[i].chase)
				{
					enemy[i].tPos = player[0].pos;
				}

				if (!enemy[i].chase)
				{
					// ���`��Ԃ̏���
					{
						int nowNo = (int)enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
						int maxNo = enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
						int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
						INTERPOLATION_DATA* tbl = moveTblAdr[enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
						float nowTime = enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

						Pos *= nowTime;		// ���݂̈ړ��ʂ��v�Z���Ă���

						// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
						XMStoreFloat3(&enemy[i].tPos, nowPos + Pos);

						// frame���g�Ď��Ԍo�ߏ���������
						enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
						if ((int)enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
						{
							enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
						}
					}
				}

				// �G�̈ړ����x
				enemy[i].spd = VALUE_MOVE;

				XMVECTOR epos = XMLoadFloat3(&enemy[i].pos);
				XMVECTOR vec = XMLoadFloat3(&enemy[i].tPos) - epos;                   //�G�l�~�[�ƃv���C���[�̍��������߂Ă���

				// �ڕW�̕���
				const float targetDir = static_cast<float>(atan2(vec.m128_f32[0], vec.m128_f32[2]) - XM_PI);;

				// ���݂̕�������ڕW�̕����ւ̊p�x�������߂�
				float angleDifference = targetDir - enemy[i].rot.y;

				// �p�x���𐳊m�Ȕ͈͂ɒ���
				while (angleDifference > XM_PI)
				{
					angleDifference -= 2 * XM_PI;
				}

				while (angleDifference < -XM_PI)
				{
					angleDifference += 2 * XM_PI;
				}

				// ��Ԃ��g�p����g_Player.dir��ڕW�̕����ɋ߂Â���
				enemy[i].rot.y = Lerp(enemy[i].rot.y, enemy[i].rot.y + angleDifference, VALUE_ROTATE);

				// �v���C���[�̕��Ɉړ�
				enemy[i].pos.x -= sinf(enemy[i].rot.y) * enemy[i].spd;
				enemy[i].pos.z -= cosf(enemy[i].rot.y) * enemy[i].spd;

				enemy[i].spd *= 0.0f;

			}
			//-------------------------------------------------------------------------
			// �����蔻�菈��
			//-------------------------------------------------------------------------
			{

				atClTime++;

				// �v���C���[�Ƃ̓����蔻��(�U��)
				for (int j = 0; j < PLAYER_MAX; j++)
				{

					if (player[j].use)
					{
						if (CollisionBC(enemy[i].pos, player[j].pos, ENEMY_SIZE, player[j].size))
						{

							if (atClTime >= 120)
							{
								player[j].hp -= 10;

								atClTime = 0;
							}

							enemy[i].pos = oldPos;

							break;
						}
					}
				}

				// ���e�Ƃ̓����蔻��
				{
					BOMB* bomb = GetBomb();
					for (int k = 0; k < BOMB_MAX; k++)
					{
						if (bomb[k].use == TRUE)
						{
							BOOL ans = CollisionBC(enemy[k].pos, bomb[k].pos, 10.0f, 10.0f);

							if (ans == TRUE)
							{
								// �|�W�V���������Ƃɖ߂�
								enemy[k].pos = oldPos;

								break;
							}
						}
					}
				}

				// �t�B�[���h�Ƃ̓����蔻��
				if (FieldHit(enemy[i].pos, oldPos))
				{
					enemy[i].pos = oldPos;
				}

			}

			//-------------------------------------------------------------------------
			// ��Ԉُ� / �G�t�F�N�g����(�p�[�e�B�N��)
			//-------------------------------------------------------------------------
			{

				// �`��͈͓����ǂ���
				if (enemy[i].draw)
				{
					// ��Ԉُ�
					switch (enemy[i].condition)
					{
					case 0: // �ʏ�
						break;

					case 1: // ����

						// HP�����炷
						enemy[i].hp -= 1.0f;

						XMFLOAT3 pos;
						XMFLOAT3 move;
						XMFLOAT3 scale;
						float fAngle, fLength;
						int nLife;
						float fSize;

						pos = enemy[i].pos;

						pos.y += 6.0f;

						scale = { 0.2f, 0.2f, 0.2f };

						fAngle = (float)(rand() % 628 - 314) / 100;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 40;

						move.x = sinf(fAngle) * fLength;
						move.y = (rand() % 300 / 100.0f + 10) / 50;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 50;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);

						break;

					case 2: // ��

						// HP�����炷
						enemy[i].hp -= 2.0f;

						pos = enemy[i].pos;

						pos.y += 12.0f;

						scale = { 0.3f, 0.3f, 0.3f };

						move.x = (300.0f - (rand() % 600)) / 800;
						move.y = (300.0f - (rand() % 600)) / 800;
						move.z = (300.0f - (rand() % 600)) / 800;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 20);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.8f), fSize, fSize, nLife, 1);

						break;
					}

				}

			}


			//-------------------------------------------------------------------------
			// ���S���菈��
			//-------------------------------------------------------------------------
			{
				if (enemy[i].hp <= 0)
				{
					AddScore(100);

					enemy[i].use = FALSE;
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
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!enemy[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!enemy[i].draw) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(enemy[i].scl.x, enemy[i].scl.y, enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(enemy[i].rot.x, enemy[i].rot.y + XM_PI, enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(enemy[i].pos.x, enemy[i].pos.y, enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&enemy[i].mtxWorld, mtxWorld);

		switch (enemy[i].modelNo)
		{
		case 0:
			DrawModel(&modelEnemy);
			break;
		}

	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY* GetEnemy()
{
	return &enemy[0];
}