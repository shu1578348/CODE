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
static ENEMY g_Enemy[ENEMY_MAX]; // �G�l�~�[�\����
static BOOL	  g_Load = FALSE;	 // ���������s�������̃t���O
DX11_MODEL	  modelEnemy;		 // ���f�����

//----------------------------------------------------------------------------
// ���`���
//----------------------------------------------------------------------------
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f,  1.57f, 0.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  0.0f),	 XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA g_MoveTbl1[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3( -20.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f, -1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
	{ XMFLOAT3(-220.0f, ENEMY_OFFSET_Y,  -100.0f), XMFLOAT3(0.0f,  1.57f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 120 },
};

static INTERPOLATION_DATA g_MoveTbl2[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y,    0.0f), XMFLOAT3(0.0f, 3.14f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, -300.0f), XMFLOAT3(0.0f, 0.0f,  0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA g_MoveTbl3[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(1.0f, 1.0f, 1.0f), 150 },
};

static INTERPOLATION_DATA g_MoveTbl4[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA g_MoveTbl5[] = {
	//���W													��]��						�g�嗦			����

	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA g_MoveTbl6[] = {
	//���W													��]��						�g�嗦			����
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
	{ XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT3(0.0f, 0.0f, 0.0f), 150 },
};

static INTERPOLATION_DATA* g_MoveTblAdr[] =
{
	g_MoveTbl0,
	g_MoveTbl1,
	g_MoveTbl2,
	g_MoveTbl3,
	g_MoveTbl4,
	g_MoveTbl5,
	g_MoveTbl6,
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
		g_Enemy[i].pos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.5f, 1.5f, 1.5f);

		g_Enemy[i].tPos = XMFLOAT3(175.0f, ENEMY_OFFSET_Y, -30.0f);

		g_Enemy[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = {ENEMY_SIZE_X, ENEMY_SIZE_Y, ENEMY_SIZE_Z};	// �����蔻��̑傫��
		
		// �_���W����(1�K)�Ȃ�
		if (GetField() == DUNGEON_FIRST_FLOOR)
		{
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);
		}

		g_Enemy[i].draw = FALSE;	// TRUE:�\�� / FALSE:��\��
		g_Enemy[i].use  = TRUE;		// TRUE:�g�p / FALSE:���g�p
		g_Enemy[i].chase = FALSE;    // TRUE:�ǐՒ� / FALSE:��ǐՒ�

		g_Enemy[i].condition = 0;	// �ʏ��Ԃɂ���
		
		g_Enemy[i].hp = ENEMY_HP; // �q�b�g�|�C���g�̏�����

		g_Enemy[i].state = FALSE;   // TRUE:�ړ��\ / FALSE:�ړ��s��
		
		g_Enemy[i].time  = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	}
	//-------------------------------------------------------------------------
	// �����ʒu�̐ݒ�
	//-------------------------------------------------------------------------
	{
		g_Enemy[0].pos = XMFLOAT3(-220.0f, ENEMY_OFFSET_Y, -0.0f);
		g_Enemy[1].pos = XMFLOAT3(-120.0f, ENEMY_OFFSET_Y, -100.0f);
		g_Enemy[2].pos = XMFLOAT3(-300.0f, ENEMY_OFFSET_Y, 0.0f);
		g_Enemy[3].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 520.0f);
		g_Enemy[4].pos = XMFLOAT3(-620.0f, ENEMY_OFFSET_Y, 750.0f);
		g_Enemy[5].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 750.0f);
		g_Enemy[6].pos = XMFLOAT3(-380.0f, ENEMY_OFFSET_Y, 520.0f);
	}

	//-------------------------------------------------------------------------
	// �s���e�[�u���̃Z�b�g
	//-------------------------------------------------------------------------
	{
		g_Enemy[0].tblNo = 0;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[1].tblNo = 1;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[1].tblMax = sizeof(g_MoveTbl1) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[2].tblNo = 2;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[2].tblMax = sizeof(g_MoveTbl2) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[3].tblNo = 3;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[3].tblMax = sizeof(g_MoveTbl3) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[4].tblNo = 4;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[4].tblMax = sizeof(g_MoveTbl4) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[5].tblNo = 5;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[5].tblMax = sizeof(g_MoveTbl5) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[6].tblNo = 6;		// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[6].tblMax = sizeof(g_MoveTbl6) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	}

	//-------------------------------------------------------------------------
	// ���f���ǂݍ���
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;

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
	if (g_Load)
	{
		UnloadModel(&modelEnemy);
		g_Load = FALSE;
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

		if (g_Enemy[i].use)	  // �g�p����
		{
			// �ړ��O�̍��W��ۑ�
			XMFLOAT3 oldPos = g_Enemy[i].pos;
						
			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			//-------------------------------------------------------------------------
			// ������J�����O
			//-------------------------------------------------------------------------
			{
				g_Enemy[i].draw = FrustumCulling(g_Enemy[i].pos, g_Enemy[i].rot, g_Enemy[i].size);
			}

			//-------------------------------------------------------------------------
			// �ړ�����
			//-------------------------------------------------------------------------
			{

				float distance = GetDistance(player[0].pos, g_Enemy[i].pos);

				// �߂Â�����ǐՂ���
				if (distance < 100)
				{
					g_Enemy[i].chase = TRUE;
				}
				else
				{
					g_Enemy[i].chase = FALSE;
				}

				// �_���[�W���󂯂���v���C���[��ǐՂ���
				if (g_Enemy[i].hp != ENEMY_HP)
				{
					g_Enemy[i].chase = TRUE;
				}

				if (g_Enemy[i].chase)
				{
					g_Enemy[i].tPos = player[0].pos;
				}

				if (!g_Enemy[i].chase)
				{
					// ���`��Ԃ̏���
					{
						int nowNo = (int)g_Enemy[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
						int maxNo = g_Enemy[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
						int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
						INTERPOLATION_DATA* tbl = g_MoveTblAdr[g_Enemy[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
						float nowTime = g_Enemy[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

						Pos *= nowTime;		// ���݂̈ړ��ʂ��v�Z���Ă���

						// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
						XMStoreFloat3(&g_Enemy[i].tPos, nowPos + Pos);

						// frame���g�Ď��Ԍo�ߏ���������
						g_Enemy[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
						if ((int)g_Enemy[i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
						{
							g_Enemy[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
						}
					}
				}

				// �G�̈ړ����x
				g_Enemy[i].spd = VALUE_MOVE;

				XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
				XMVECTOR vec = XMLoadFloat3(&g_Enemy[i].tPos) - epos;                   //�G�l�~�[�ƃv���C���[�̍��������߂Ă���

				// �ڕW�̕���
				const float targetDir = static_cast<float>(atan2(vec.m128_f32[0], vec.m128_f32[2]) - XM_PI);;

				// ���݂̕�������ڕW�̕����ւ̊p�x�������߂�
				float angleDifference = targetDir - g_Enemy[i].rot.y;

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
				g_Enemy[i].rot.y = Lerp(g_Enemy[i].rot.y, g_Enemy[i].rot.y + angleDifference, VALUE_ROTATE);

				// �v���C���[�̕��Ɉړ�
				g_Enemy[i].pos.x -= sinf(g_Enemy[i].rot.y) * g_Enemy[i].spd;
				g_Enemy[i].pos.z -= cosf(g_Enemy[i].rot.y) * g_Enemy[i].spd;

				g_Enemy[i].spd *= 0.0f;

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
						if (CollisionBC(g_Enemy[i].pos, player[j].pos, ENEMY_SIZE, player[j].size))
						{

							if (atClTime >= 120)
							{
								player[j].hp -= 10;

								atClTime = 0;
							}

							g_Enemy[i].pos = oldPos;

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
							BOOL ans = CollisionBC(g_Enemy[k].pos, bomb[k].pos, 10.0f, 10.0f);

							if (ans == TRUE)
							{
								// �|�W�V���������Ƃɖ߂�
								g_Enemy[k].pos = oldPos;

								break;
							}
						}
					}
				}

				// �t�B�[���h�Ƃ̓����蔻��
				if (FieldHit(g_Enemy[i].pos, oldPos))
				{
					g_Enemy[i].pos = oldPos;
				}

			}

			//-------------------------------------------------------------------------
			// ��Ԉُ� / �G�t�F�N�g����(�p�[�e�B�N��)
			//-------------------------------------------------------------------------
			{

				// �`��͈͓����ǂ���
				if (g_Enemy[i].draw)
				{
					// ��Ԉُ�
					switch (g_Enemy[i].condition)
					{
					case 0: // �ʏ�
						break;

					case 1: // ����

						// HP�����炷
						g_Enemy[i].hp -= 1.0f;

						XMFLOAT3 pos;
						XMFLOAT3 move;
						XMFLOAT3 scale;
						float fAngle, fLength;
						int nLife;
						float fSize;

						pos = g_Enemy[i].pos;

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
						g_Enemy[i].hp -= 2.0f;

						pos = g_Enemy[i].pos;

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
				if (g_Enemy[i].hp <= 0)
				{
					AddScore(100);

					g_Enemy[i].use = FALSE;
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
		if (!g_Enemy[i].use)  continue;

		// TRUE:�\�� / FALSE:��\��
		if (!g_Enemy[i].draw) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);

		switch (g_Enemy[i].modelNo)
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
	return &g_Enemy[0];
}