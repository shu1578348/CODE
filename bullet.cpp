//=============================================================================
//
// �e���ˏ��� [bullet.cpp]
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
#include "boss.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BULLET "data/MODEL/dagger.obj"	 // �ǂݍ��ރ��f����

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BULLET bullet[BULLET_MAX];	 // �o���b�g�\����
static BOOL	  hoad = FALSE;		 // ���������s�������̃t���O
DX11_MODEL	  modelBullet;			 // ���f�����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	//-------------------------------------------------------------------------
	// ���̏�����
	//-------------------------------------------------------------------------
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		bullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		bullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		bullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };

		bullet[nCntBullet].spd = 0.0f;

		bullet[nCntBullet].size = BULLET_SIZE; // �����蔻��̑傫��

		bullet[nCntBullet].type = 1; // 0:�ʏ�e / 1:�Ή��e / 2:�Œe

		bullet[nCntBullet].use  = FALSE;	// TRUE:�g�p / FALSE:���g�p
		bullet[nCntBullet].shot = FALSE;	// TRUE:���� / FALSE:������
	}

	//-------------------------------------------------------------------------
	// ���f���̓ǂݍ���
	//-------------------------------------------------------------------------
	{
		hoad = TRUE;

		// ���f���̓ǂݍ���
		LoadModel(MODEL_BULLET, &modelBullet);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	if (hoad)
	{
		UnloadModel(&modelBullet);
		hoad = FALSE;
	}	
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	//-------------------------------------------------------------------------
	// OBJ���̎擾
	//-------------------------------------------------------------------------
	PLAYER * player = GetPlayer();  // �v���C���[
	ENEMY  * enemy  = GetEnemy();	// �G�l�~�[
	HOUSE  * house  = GetHouse();   // ��
	BARREL * barrel = GetBarrel();  // �M
	BOSS   * boss   = GetBoss();	// �{�X	

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (bullet[i].use)
		{		
			// �ړ��O�̍��W��ۑ�
			XMFLOAT3 oldPos = bullet[i].pos;

			//-----------------------------------------------------------------
			// ���ˌ�̏���
			//-----------------------------------------------------------------
			if (bullet[i].shot)
			{

				//-------------------------------------------------------------
				// �ړ�����
				//-------------------------------------------------------------
				{

					// ��������
					bullet[i].spd *= 0.997f;

					// �e�̈ړ�����
					bullet[i].pos.x -= sinf(bullet[i].rot.y) * bullet[i].spd;
					bullet[i].pos.z -= cosf(bullet[i].rot.y) * bullet[i].spd;

					// ������܂ł̎���
					bullet[i].dTime -= 1;

					// ��莞�Ԍo�Ə�����
					if (bullet[i].dTime < 0)
					{
						bullet[i].use = FALSE;
						ReleaseShadow(bullet[i].shadowIdx);
					}

					// �}�b�v�O�Ȃ������
					{
						float distance = GetDistance(bullet[i].pos, { 0.0f, 0.0f, 0.0f });

						if (distance > 1200)
						{
							bullet[i].use = TRUE;
							ReleaseShadow(bullet[i].shadowIdx);
						}
					}
				}

				//-------------------------------------------------------------
				// �G�t�F�N�g����(�p�[�e�B�N��)
				//-------------------------------------------------------------
				{
					XMFLOAT3 pos;
					XMFLOAT3 move;
					XMFLOAT3 scale;
					float size;
					float fAngle, fLength;
					float fSize;
					int nLife;

					pos = bullet[i].pos;

					for (int j = 0; j < 3; j++)
					{

						switch (bullet[i].type)
						{
						case 0: // �ʏ�e						

							pos.x += sinf(bullet[i].rot.y) * j;
							pos.z += cosf(bullet[i].rot.y) * j;

							scale = { 0.04f, 0.04f, 0.04f };

							move.x = 0.0f;
							move.y = 0.0f;
							move.z = 0.0f;

							nLife = 200;

							fSize = 0.0f;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f), fSize, fSize, nLife, 0);

							break;

						case 1: // �Ή��e

							scale = { 0.07f, 0.07f, 0.07f };

							fAngle = (float)(rand() % 628 - 314) * 0.01f;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.025f;

							move.x = sinf(fAngle) * fLength;
							move.y = 0.0f;
							move.z = cosf(fAngle) * fLength;

							nLife = rand() % 50 + 60;

							fSize = 0.0f;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

							scale = { 0.03f, 0.03f, 0.03f };

							fAngle = (float)(rand() % 628 - 314) * 0.05f;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.05f;

							move.x = sinf(fAngle) * fLength;
							move.y = 0.0f;
							move.z = cosf(fAngle) * fLength;

							nLife = rand() % 50 + 60;

							fSize = 0.0f;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

							break;

						case 2: // �Œe

							size = (rand() % 100 + 5.0f) * 0.0013f;

							scale = { 0.09f, 0.09f, 0.09f };

							scale.x = size;
							scale.y = size;
							scale.z = size;

							fAngle = (float)(rand() % 628 - 314) * 0.005f;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.0125f;

							move.x = sinf(fAngle) * fLength;
							move.y = 0.0f;
							move.z = cosf(fAngle) * fLength;

							nLife = rand() % 50 + 60;

							fSize = (float)(rand() % 30 + 10);

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);

							break;

						}

					}

				}

			}
			//-------------------------------------------------------------
			// �����ˎ��̏���
			//-------------------------------------------------------------
			else
			{
				// �v���C���[�̌����Ɉʒu�����킹��
				bullet[i].pos = XMFLOAT3(player[0].pos.x, 27.0f, player[0].pos.z);

				// ���͂̂����������փv���C���[���������Ĉړ�������
				bullet[i].pos.x -= sinf(player[0].rot.y - 0.9f) * 4.0f;
				bullet[i].pos.z -= cosf(player[0].rot.y - 0.9f) * 4.0f;

				// �v���C���[�Ɖ�]�𓯂��ɂ���
				bullet[i].rot.y = player[0].rot.y;

				// ���ˑҋ@��Ԃ̃p�[�e�B�N��
				{
					XMFLOAT3 pos;
					XMFLOAT3 move;
					XMFLOAT3 scale;
					float fAngle, fLength;
					float fSize;
					int nLife;

					switch (player[0].tool)
					{
					case 0:
					case 1:
					case 2:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 5:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 3:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.1f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 4:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.06f, 0.06f, 0.06f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.08f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.6f), fSize, fSize, nLife, 1);

						scale = { 0.03f, 0.03f, 0.03f };

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.1f, 1.0f), fSize, fSize, nLife, 1);
						}

						break;

					case 6:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.06f, 0.06f, 0.06f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.15f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);

						scale = { 0.03f, 0.03f, 0.03f };

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}
						break;

					case 7:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.09f, 0.09f, 0.09f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.2f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// �r���{�[�h�̐ݒ�
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.6f), fSize, fSize, nLife, 1);

						scale = { 0.03f, 0.03f, 0.03f };

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// �r���{�[�h�̐ݒ�
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						break;

					}

				}

				// ���˂��邩�ǂ���
				if (GetKeyboardRelease(DIK_SPACE) || IsButtonReleased(1,BUTTON_B))
				{
					// �e��
					bullet[i].spd = BULLET_SPEED;

					// ������ʒu
					bullet[i].pos.y -= 10.0f;

					// ���˂���
					bullet[i].shot = TRUE;

					// ���˕���
					switch(bullet[i].shotDir)
					{
					case 0:
						break;

					case 1:
						bullet[i].rot.y -= 0.1f;
						break;

					case 2:
						bullet[i].rot.y += 0.1f;
						break;
					}

					// �A�j���[�V�������Đ�����
					player[0].anim = FALSE;

					player[0].pose = 3;

				}

			}

			// �e�̈ʒu�ݒ�
			SetPositionShadow(bullet[i].shadowIdx, XMFLOAT3(bullet[i].pos.x, 0.1f, bullet[i].pos.z));

			//-------------------------------------------------------------
			// �����蔻��̏���
			//-------------------------------------------------------------
			{
				// �ÓI�I�u�W�F�N�g�̓����蔻��
				if (FieldHit(bullet[i].pos, oldPos))
				{
					bullet[i].use = FALSE;
					ReleaseShadow(bullet[i].shadowIdx);

					// �o���G�t�F�N�g�̖�����
					if (bullet[i].type != 0)
					{
						// �ڐG�G�t�F�N�g
						BulletEfect(bullet[i].pos, bullet[i].type);
					}
				}

				switch (GetField())
				{
				case VILLAGE:
						break;

				case DUNGEON_FIRST_FLOOR:
				//---------------------------------------------------------
				// �G�l�~�[�Ƃ̓����蔻��
				//---------------------------------------------------------
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						if (bullet[i].shot)
						{
							if (enemy[j].use)
							{
								if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].size, ENEMY_SIZE))
								{
									// ���������������
									bullet[i].use = FALSE;

									// �G�l�~�[��HP�����炷
									enemy[j].hp -= 100.0f;

									ReleaseShadow(bullet[i].shadowIdx);

									// ��Ԉُ�
									switch (bullet[i].type)
									{
									case 0: // �ʏ�e
										break;

									case 1: // �Ή��e
										enemy[j].condition = 1;
										break;

									case 2: // �Œe
										enemy[j].condition = 2;
										break;
									}

									// �ڐG�G�t�F�N�g
									BulletEfect(bullet[i].pos, bullet[i].type);

								}

							}

						}

					}

					break;

				case DUNGEON_SECOND_FLOOR:
					if (bullet[i].shot)
					{
						if (boss[0].use)
						{
							if (CollisionBC(bullet[i].pos, boss[0].pos, bullet[i].size, BOSS_SIZE))
							{
								// ���������������
								bullet[i].use = FALSE;

								// �G�l�~�[��HP�����炷
								boss[0].hp -= 10.0f;

								ReleaseShadow(bullet[i].shadowIdx);

								// �ڐG�G�t�F�N�g
								BulletEfect(bullet[i].pos, bullet[i].type);

							}

						}

					}
							
					break;

				}
				
			}

		}

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		// TRUE:�g�p / FALSE:���g�p
		if (!bullet[i].use) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(bullet[i].scl.x, bullet[i].scl.y, bullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(bullet[i].rot.x, bullet[i].rot.y + XM_PI, bullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(bullet[i].pos.x, bullet[i].pos.y, bullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&bullet[i].mtxWorld, mtxWorld);

		// ���f���`��
		switch (bullet[i].modelNo)
		{
		case 0:
			DrawModel(&modelBullet);
			break;
		}

	}

}

//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, BOOL shot, int type, int shotDir)
{
	int nIdxBullet = -1;

	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (!bullet[nCntBullet].use)	// ���g�p��Ԃ̃o���b�g��������
		{
			bullet[nCntBullet].pos = XMFLOAT3(pos.x, 100.0f, pos.z);	// ���W���Z�b�g
			bullet[nCntBullet].rot  = rot;							// ��]���Z�b�g
			bullet[nCntBullet].shot = shot;
			bullet[nCntBullet].use = TRUE;							// �g�p��Ԃ֕ύX����
			

			bullet[nCntBullet].dTime = BULLET_DELETE;					// ������܂ł̎���
			bullet[nCntBullet].scl = { 0.2f, 0.2f, 0.2f };			// �T�C�Y�����Ƃɖ߂�

			bullet[nCntBullet].type = type;							// �e�̎��
			bullet[nCntBullet].shotDir = shotDir;						// �e�̔��˕��� 0:���� 1:���O 2:�E�O

			// �e�̐ݒ�
			bullet[nCntBullet].shadowIdx = CreateShadow(bullet[nCntBullet].pos, 0.2f, 0.2f);

			nIdxBullet = nCntBullet;

			break;
		}
	}

}

//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================
BULLET* GetBullet(void)
{
	return &bullet[0];
}

//=============================================================================
// �o���b�g�̐ڐG���G�t�F�N�g
//=============================================================================
void BulletEfect(XMFLOAT3 pos, int type)
{

	XMFLOAT3 move;
	XMFLOAT3 scale;
	float fSize;
	int nLife;

	for (int k = 0; k < 250; k++)
	{
		switch (type)
		{
		case 0: // �ʏ�e

			scale = { 0.08f, 0.08f, 0.08f };

			move.x = (300.0f - (rand() % 600)) / 1500;
			move.y = (300.0f - (rand() % 600)) / 1500;
			move.z = (300.0f - (rand() % 600)) / 1500;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 20);

			// �r���{�[�h�̐ݒ�
			SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.3f, 0.6f), fSize, fSize, nLife, 1);
			break;

		case 1: // ���G�t�F�N�g

			scale = { 0.2f, 0.2f, 0.2f };

			move.x = (300.0f - (rand() % 600)) / 800;
			move.y = (300.0f - (rand() % 600)) / 800;
			move.z = (300.0f - (rand() % 600)) / 800;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 20);

			// �r���{�[�h�̐ݒ�
			SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);
			break;

		case 2: // �ŃG�t�F�N�g

			scale = { 0.3f, 0.3f, 0.3f };

			move.x = (300.0f - (rand() % 600)) / 500;
			move.y = (300.0f - (rand() % 600)) / 500;
			move.z = (300.0f - (rand() % 600)) / 500;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 20);

			// �r���{�[�h�̐ݒ�
			SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.6f), fSize, fSize, nLife, 1);
			break;
		}

	}
}