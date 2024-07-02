//=============================================================================
//
// �e���ˏ��� [enemybullet.cpp]
// Author : �r�R �G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "shadow.h"
#include "model.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "barrel.h"
#include "house.h"
#include "collision.h"
#include "score.h"
#include "particle.h"
#include "game.h"
#include "enemybullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMYBULLET enemyBullet[ENEMY_BULLET_MAX];	 // �G�l�~�[�o���b�g�\����
static BOOL	  load = FALSE;						 // ���������s�������̃t���O

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemyBullet(void)
{
	//-------------------------------------------------------------------------
	// ���̏�����
	//-------------------------------------------------------------------------
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		enemyBullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		enemyBullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		enemyBullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };

		enemyBullet[nCntBullet].spd = 0.0f;

		enemyBullet[nCntBullet].size = BULLET_SIZE; // �����蔻��̑傫��

		enemyBullet[nCntBullet].type = 1; // 0:�ʏ�e / 1:�Ή��e / 2:�Œe

		enemyBullet[nCntBullet].use  = FALSE;	// TRUE:�g�p / FALSE:���g�p
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemyBullet(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemyBullet(void)
{
	//-------------------------------------------------------------------------
	// OBJ���̎擾
	//-------------------------------------------------------------------------
	PLAYER* player = GetPlayer(); // �v���C���[

	XMFLOAT3 pos;
	XMFLOAT3 oldPos;
	XMFLOAT3 move;
	XMFLOAT3 scale;
	float fAngle, fLength;
	float fSize;
	int nLife;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (enemyBullet[i].use)
		{

			//-------------------------------------------------------------------------
			// �ړ�����
			//-------------------------------------------------------------------------
			{

				// �ړ��O�̍��W��ۑ�
				oldPos = enemyBullet[i].pos;
				
				// �e�̈ړ�����
				enemyBullet[i].pos.x -= sinf(enemyBullet[i].rot.y) * enemyBullet[i].spd;
				enemyBullet[i].pos.z -= cosf(enemyBullet[i].rot.y) * enemyBullet[i].spd;

				// ��������
				enemyBullet[i].spd *= 0.997f;

				enemyBullet[i].dTime -= 1;

				// ��莞�Ԍo�Ə�����
				if (enemyBullet[i].dTime < 0)
				{
					enemyBullet[i].use = FALSE;
					ReleaseShadow(enemyBullet[i].shadowIdx);
				}

			}

			//-------------------------------------------------------------------------
			// �G�t�F�N�g����
			//-------------------------------------------------------------------------
			{

				pos = enemyBullet[i].pos;

				scale = { 0.3f, 0.3f, 0.3f };

				fAngle = (float)(rand() % 628 - 314) / 20;
				fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 8;

				move.x = sinf(fAngle) * fLength;
				move.y = (rand() % 300 / 100.0f + 10) / 100;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 80 + 50;

				fSize = (float)(rand() % 30 + 10);

				// �r���{�[�h�̐ݒ�
				SetParticle(enemyBullet[i].pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);

			}


			//-------------------------------------------------------------------------
			// �v���C���[�Ƃ̓����蔻�菈��
			//-------------------------------------------------------------------------
			{
				if (player[0].use)
				{

					if (CollisionBC(player[0].pos, enemyBullet[i].pos, player[0].size, enemyBullet[i].size))
					{
						// ���������������
						enemyBullet[i].use = FALSE;

						// �G�l�~�[��HP�����炷
						player[0].hp -= 5;

						ReleaseShadow(enemyBullet[i].shadowIdx);

						// �ڐG�G�t�F�N�g
						for (int j = 0; j < 20; j++)
						{
							pos = enemyBullet[i].pos;

							scale = { 0.3f, 0.3f, 0.3f };

							fAngle = (float)(rand() % 628 - 314) / 20;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 5;

							move.x = sinf(fAngle) * fLength;
							move.y = (100.0f - rand() % 200) * 0.01f;
							move.z = cosf(fAngle) * fLength;

							move.x -= sinf(enemyBullet[i].rot.y);
							move.z -= cosf(enemyBullet[i].rot.y);

							nLife = rand() % 80 + 50;

							fSize = (float)(rand() % 30 + 10);

							// �r���{�[�h�̐ݒ�
							SetParticle(enemyBullet[i].pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);
						}

					}

				}

			}

			if (FieldHit(enemyBullet[i].pos, oldPos))
			{
				enemyBullet[i].use = FALSE;

			}

		}

	}

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyBullet(void)
{

}

//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetEnemyBullet(XMFLOAT3 pos, XMFLOAT3 rot,int type)
{
	int nIdxBullet = -1;

	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (!enemyBullet[nCntBullet].use)	// ���g�p��Ԃ̃o���b�g��������
		{
			float len;

			len = (20.0f - rand() % 40) * 0.02f;

			enemyBullet[nCntBullet].spd = ENEMY_BULLET_SPEED;

			enemyBullet[nCntBullet].pos = XMFLOAT3(pos.x, 20.0f, pos.z);	// ���W���Z�b�g
			enemyBullet[nCntBullet].rot  = rot;							// ��]���Z�b�g
			enemyBullet[nCntBullet].rot.y += len;							// ��]���Z�b�g
			enemyBullet[nCntBullet].use = TRUE;							// �g�p��Ԃ֕ύX����	

			enemyBullet[nCntBullet].dTime = ENEMY_BULLET_DELETE;			// ������܂ł̎���
			enemyBullet[nCntBullet].scl = { 0.2f, 0.2f, 0.2f };			// �T�C�Y�����Ƃɖ߂�

			enemyBullet[nCntBullet].type = type;							// �e�̎��

			// �e�̐ݒ�
			enemyBullet[nCntBullet].shadowIdx = CreateShadow(enemyBullet[nCntBullet].pos, 0.2f, 0.2f);

			nIdxBullet = nCntBullet;

			break;
		}
	}

}

//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMYBULLET* GetEnemyBullet(void)
{
	return &enemyBullet[0];
}

//=============================================================================
// �o���b�g�̐ڐG���G�t�F�N�g
//=============================================================================
void EnemyBulletEfect(XMFLOAT3 pos, int type)
{

}