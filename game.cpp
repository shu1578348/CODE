//=============================================================================
//
// �^�C�g����ʏ��� [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"
#include "camera.h"
#include "model.h"
#include "input.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "tree.h"
#include "particle.h"
#include "bullet.h"
#include "score.h"
#include "house.h"
#include "barrel.h"
#include "fence.h"
#include "stall.h"
#include "dungeon.h"
#include "sky.h"
#include "bomb.h"
#include "fade.h"
#include "ui.h"
#include "boss.h"
#include "enemybullet.h"
#include "speartrap.h"
#include "carriage.h"
#include "box.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
int	g_Field;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	// �t�B�[���h�̏�����
	g_Field = VILLAGE;

	// �ŏ��̃��[�h���Z�b�g
	SetField(g_Field); // ������SetField�̂܂�

	return S_OK;

}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{

	// �I���̃��[�h���Z�b�g
	SetField(FIELD_MAX);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{

	PLAYER* player = GetPlayer();

	ENEMY* enemy = GetEnemy();

	switch (g_Field)
	{
	case VILLAGE:
		// UI�̍X�V����
		UpdateUi();

		// �n�ʏ����̍X�V
		UpdateMeshField();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �Ƃ̍X�V����
		UpdateHouse();

		// �M�̍X�V����
		UpdateBarrel();

		// ��̍X�V����
		UpdateFence();

		// ����̍X�V����
		UpdateStall();

		// �n�Ԃ̍X�V����
		UpdateCarriage();

		// �ؔ��̍X�V����
		UpdateBox();

		// �_���W�����̍X�V����
		UpdateDungeon();

		// ��̍X�V����
		UpdateSky();

		// �؂̍X�V����
		UpdateTree();

		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		// �e�̍X�V����
		UpdateBullet();

		// �e�̍X�V����
		UpdateShadow();

		// 1�K�w�ڂɓ���Ƃ��̏���
		if (GetKeyboardTrigger(DIK_0))
		{

			SetField(DUNGEON_FIRST_FLOOR);
		}


		break;

	case DUNGEON_FIRST_FLOOR:

		// UI�̍X�V����
		UpdateUi();

		// �Ǐ����̍X�V
		UpdateMeshWall();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �G�l�~�[�̍X�V����
		UpdateEnemy();

		// ���e�̍X�V����
		UpdateBomb();

		// ���̍X�V����
		//UpdateTrap();

		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		// �e�̍X�V����
		UpdateBullet();

		// �e�̍X�V����
		UpdateShadow();

		// ���U���g�ɉ�ʐ؂�ւ��p
		if(player[0].hp <= 0)
		{

			SetFade(FADE_OUT, MODE_RESULT);
		}

		// 2�K�w�ڂɓ���Ƃ��̏���
		if (player[0].pos.z >= 1030.0f)
		{

			SetField(DUNGEON_SECOND_FLOOR);
		}

		// 2�K�w�ڂɓ���Ƃ��̏���
		if (GetKeyboardTrigger(DIK_0))
		{

			SetField(DUNGEON_SECOND_FLOOR);
		}


		break;

	case DUNGEON_SECOND_FLOOR:
		// UI�̍X�V����
		UpdateUi();

		// �Ǐ����̍X�V
		UpdateMeshWall();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �{�X�̍X�V����
		UpdateBoss();

		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		// �e�̍X�V����
		UpdateBullet();

		// �G�̒e�̍X�V����
		UpdateEnemyBullet();

		// �e�̍X�V����
		UpdateShadow();

		// ���U���g�ɉ�ʐ؂�ւ��p
		if (player[0].hp <= 0)
		{

			SetFade(FADE_OUT, MODE_RESULT);
		}

		// 2�K�w�ڂɓ���Ƃ��̏���
		if (GetKeyboardTrigger(DIK_0))
		{

			SetField(VILLAGE);
		}

		break;
	}

#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{

	switch (g_Field)
	{
	case VILLAGE:

		// ��̕`�揈��
		DrawSky();

		// �n�ʂ̕`�揈��
		DrawMeshField();

		// �؂̕`�揈��
		DrawTree();

		// �e�̕`�揈��
		DrawShadow();

		// �Ƃ̕`�揈��
		DrawHouse();

		// �M�̕`�揈��
		DrawBarrel();

		// ��̕`�揈��
		DrawFence();

		// ����̕`�揈��
		DrawStall();

		// �n�Ԃ̕`�揈��
		DrawCarriage();

		// �ؔ��̕`�揈��
		DrawBox();


		// �_���W�����̕`�揈��
		DrawDungeon();

		// �e�̕`�揈��
		DrawBullet();

		// �v���C���[�̕`�揈��
		DrawPlayer();

		// �p�[�e�B�N���̕`�揈��
		DrawParticle();

		// 2DUI�̕`�揈��
		DrawUi();

		break;

	case DUNGEON_FIRST_FLOOR:

		// �e�̕`�揈��
		DrawShadow();

		// ���e�̕`�揈��
		DrawBomb();

		// ���̕`�揈��
		//DrawTrap();

		// �e�̕`�揈��
		DrawBullet();

		// �G�l�~�[�̕`�揈��
		DrawEnemy();

		// �v���C���[�̕`�揈��
		DrawPlayer();

		// �ǂ̕`�揈��
		DrawMeshWall();

		// �p�[�e�B�N���̕`�揈��
		DrawParticle();

		// 2DUI�̕`�揈��
		DrawUi();

		break;

	case DUNGEON_SECOND_FLOOR:
		// �e�̕`�揈��
		DrawShadow();

		// �e�̕`�揈��
		DrawBullet();

		// �G�̒e�̍X�V����
		DrawEnemyBullet();

		// �{�X�̕`�揈��
		DrawBoss();

		// �v���C���[�̕`�揈��
		DrawPlayer();

		// �ǂ̕`�揈��
		DrawMeshWall();

		// �p�[�e�B�N���̕`�揈��
		DrawParticle();

		// 2DUI�̕`�揈��
		DrawUi();

		break;
	}

}

//=============================================================================
// �t�B�[���h�̐ݒ�
//=============================================================================
void SetField(int field)
{
	// UI�̏I������
	UninitUi();

	// �G�̒e�̏���
	UninitEnemyBullet();

	// �e�̏I������
	UninitBullet();

	// �؂̏I������
	UninitTree();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �Ƃ̏I������
	UninitHouse();

	// �M�̏I������
	UninitBarrel();

	// ���e�̏I������
	UninitBomb();

	// ���̏I������
	//UninitTrap();

	// ��̏I������
	UninitFence();

	// ����̏I������
	UninitStall();

	// ����̏I������
	UninitCarriage();

	// �ؔ��̏I������
	UninitBox();

	// �_���W�����̏I������
	UninitDungeon();

	// ��̏I������
	UninitSky();

	// �{�X�̏I������
	UninitBoss();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	// ���̃t�B�[���h���Z�b�g���Ă���
	g_Field = field;

	switch (g_Field)
	{
	case VILLAGE:
		// �J�����̏�����
		InitCamera();

		// UI�̏�����
		InitUi();

		// �t�B�[���h�̏�����
		InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 200, 200, 20.0f, 20.0f);

		// �e�̏���������
		InitShadow();

		// �v���C���[�̏�����
		InitPlayer();

		// �Ƃ̏�����
		InitHouse();

		// �M�̏�����
		InitBarrel();

		// ��̏�����
		InitFence();

		// ����̏�����
		InitStall();

		// �n�Ԃ̏�����
		InitCarriage();

		// �ؔ��̏�����
		InitBox();

		// �_���W�����̏�����
		InitDungeon();

		// ��̏�����
		InitSky();

		// �p�[�e�B�N���̏�����
		InitParticle();

		// �؂𐶂₷
		InitTree();

		// �e�̏�����
		InitBullet();
		break;

	case DUNGEON_FIRST_FLOOR:
		// �J�����̏�����
		InitCamera();

		// UI�̏�����
		InitUi();

		// �ǂ̏�����  
		{		// �ŏ��̒ʘH
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-100.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 80.0f, -480.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 2, 40.0f, 40.0f);
		}

		{ // �ŏ��̕���
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -80.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				 XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 12, 12, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 80.0f, -80.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 12, 12, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-300.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-20.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(60.0f, 0.0f, -80.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 12, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 160.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
		}

		{ // 2�ڂ̒ʘH
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 120.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 80.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 80.0f, 120.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 440.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 18, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 320.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 320.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 80.0f, 320.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
		}

		{ // 2�ڂ̕���
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 640.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 8, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 80.0f, 640.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 8, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-460.0f, 0.0f, 480.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 800.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3�ڂ̒ʘH
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, 920.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 80.0f, 920.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3�ڂ̕���
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 1240.0f), XMFLOAT3(XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 80.0f, 1240.0f), XMFLOAT3(-XM_PI * 0.50f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 10, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-500.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-380.0f, 0.0f, 1440.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 10, 2, 40.0f, 40.0f);
		}

		// ��(�����p�̔�����)
		{		// �ŏ��̒ʘH
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-100.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 2, 2, 40.0f, 40.0f);
		}

		{ // �ŏ��̕���
			InitMeshWall(XMFLOAT3(-300.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-20.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(60.0f, 0.0f, -80.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 12, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 160.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 16, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);
		}

		{// 2�ڂ̒ʘH
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 80.0f), XMFLOAT3(0.0f,0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-660.0f, 0.0f, 440.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 18, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 320.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
		}

		{ // 2�ڂ̕���
			InitMeshWall(XMFLOAT3(-460.0f, 0.0f, 480.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 640.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-540.0f, 0.0f, 800.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3�ڂ̒ʘH
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-340.0f, 0.0f, 920.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 6, 2, 40.0f, 40.0f);
		}

		{ // 3�ڂ̕���
			InitMeshWall(XMFLOAT3(-500.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-260.0f, 0.0f, 1040.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 4, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-580.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, 1240.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-380.0f, 0.0f, 1440.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), 10, 2, 40.0f, 40.0f);		
		}

		// �e�̏���������
		InitShadow();

		// �v���C���[�̏�����
		InitPlayer();

		// �G�l�~�[�̏�����
		InitEnemy();

		// ���e�̏�����
		InitBomb();

		// ���̏�����
		//InitTrap();

		// �p�[�e�B�N���̏�����
		InitParticle();

		// �e�̏�����
		InitBullet();
		break;

	case DUNGEON_SECOND_FLOOR:
		// �J�����̏�����
		InitCamera();

		// UI�̏�����
		InitUi();

		// �ǂ̏�����  
		{		// �ŏ��̒ʘH
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-100.0f, 0.0f, -480.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 80.0f, -480.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-180.0f, 0.0f, -480.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 8, 2, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, -640.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 2, 40.0f, 40.0f);
		}

		{ // �ŏ��̕���
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f,  -40.0f), XMFLOAT3(XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 14, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 240.0f, -40.0f), XMFLOAT3(-XM_PI * 0.50f, 0.0f, -XM_PI * 0.50f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 14, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-300.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(20.0f, 0.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 6, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 80.0f, -320.0f), XMFLOAT3(0.0f, -XM_PI, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 2, 4, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(140.0f, 0.0f, -40.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-140.0f, 0.0f, 240.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 6, 40.0f, 40.0f);
			InitMeshWall(XMFLOAT3(-420.0f, 0.0f, -40.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 14, 6, 40.0f, 40.0f);
		}

		// �e�̏���������
		InitShadow();

		// �v���C���[�̏�����
		InitPlayer();

		// �{�X�̏�����
		InitBoss();

		// �p�[�e�B�N���̏�����
		InitParticle();

		// �e�̏�����
		InitBullet();

		// �G�̒e�̏�����
		InitEnemyBullet();

		break;

	}

}

//=============================================================================
// �t�B�[���h�̎擾
//=============================================================================
int GetField(void)
{
	return g_Field;
}

//=============================================================================
// ������J�����O
//=============================================================================
BOOL FrustumCulling(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 size)
{
	CAMERA* cam = GetCamera();

	// �����𒲂ׂ�
	if (GetDistance(cam[0].pos, pos) >= CULL_FAR_Z)
	{
		return FALSE;
	}

	// �o�E���f�B���O�{�b�N�X���쐬
	BoundingOrientedBox box;
	box.Center = pos;
	box.Extents = size;
	XMVECTOR orientation = XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMStoreFloat4(&box.Orientation, orientation);

	// �t���X�^�����쐬
	BoundingFrustum frustum;
	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&cam[0].pos), XMLoadFloat3(&cam[0].at), XMLoadFloat3(&cam[0].up));
	XMMATRIX projection = XMMatrixPerspectiveFovLH(CULL_ANGLE, CULL_ASPECT, CULL_NEAR_Z, CULL_FAR_Z);
	BoundingFrustum::CreateFromMatrix(frustum, projection);
	frustum.Transform(frustum, XMMatrixInverse(nullptr, view));

	// �t���X�^���ƃo�E���f�B���O�{�b�N�X���ڐG���Ă��邩�𔻒�
	if (frustum.Intersects(box))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}

//=============================================================================
// �t�B�[���h�̓����蔻��(�ÓI�I�u�W�F�N�g)�̎擾
//=============================================================================
BOOL FieldHit(XMFLOAT3 newPos, XMFLOAT3 oldPos)
{

	switch (GetField())
	{
	case VILLAGE:

		// ��Ƃ̓����蔻��
		{
			FENCE* fence = GetFence();

			for (int n = 0; n < FENCE_MAX; ++n)
			{

				float distance = GetDistance(newPos, fence[n].pos);

				if (distance < 100.0f)
				{
					if (fence[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(fence[n].size, fence[n].pos, fence[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}

		}

		// �ƂƂ̓����蔻��
		{
			HOUSE* house = GetHouse();

			for (int n = 0; n < HOUSE_MAX; ++n)
			{
				
				float distance = GetDistance(newPos, house[n].pos);

				if (distance < 100.0f)
				{

					if (house[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(house[n].size, house[n].pos, house[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}

		}

		// �M�Ƃ̓����蔻��
		{
			BARREL* barrel = GetBarrel();
			for (int m = 0; m < BARREL_MAX; m++)
			{
				if (barrel[m].use == TRUE)
				{
					if (CollisionBC(newPos, barrel[m].pos, 10.0f, BARREL_SIZE)) 
					{
						return TRUE;
					}
				}
			}
		}

		// ����Ƃ̓����蔻��
		{
			STALL* stall = GetStall();

			for (int n = 0; n < STALL_MAX; ++n)
			{

				float distance = GetDistance(newPos, stall[n].pos);

				if (distance < 100.0f)
				{

					if (stall[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(stall[n].size, stall[n].pos, stall[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}
		}

		// �n�ԂƂ̓����蔻��
		{
			CARRIAGE* carriage = GetCarriage();

			for (int n = 0; n < CARRIAGE_MAX; ++n)
			{

				float distance = GetDistance(newPos, carriage[n].pos);

				if (distance < 100.0f)
				{

					if (carriage[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider(carriage[n].size, carriage[n].pos, carriage[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}
		}

		// �_���W�����Ƃ̓����蔻��
		{
			DUNGEON* dungeon = GetDungeon();

			for (int n = 0; n < DUNGEON_MAX; ++n)
			{

				float distance = GetDistance(newPos, dungeon[n].pos);

				if (distance < 100.0f)
				{

					if (dungeon[n].use == TRUE)
					{
						XMFLOAT3 hitPoint;
						XMFLOAT3 hitNormal;
						BOOL collisionDetected = BoxCollider( dungeon[n].size, dungeon[n].pos, dungeon[n].rot.y, oldPos, newPos, &hitPoint, &hitNormal);

						if (collisionDetected)
						{
							return TRUE;
						}
					}

				}

			}
		}

		break;

	case DUNGEON_FIRST_FLOOR:

		if (newPos.z <= -635.0f)
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x >= -110.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x <= -173.0f))
		{
			return TRUE;
		}

		if (newPos.x >= 54.0f)
		{
			return TRUE;
		}

		if ((newPos.z <= 85) && (newPos.x <= -415.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= 149.0f) && (newPos.z <= 488.0f)) && (newPos.x >= -590.0f))
		{
			return TRUE;
		}

		if (newPos.x <= -649.0f)
		{
			return TRUE;
		}

		if (((newPos.z >= 781.0f) && (newPos.z <= 1050.0f)) && (newPos.x <= -411.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= 480.0f) && (newPos.z <= 1050.0f)) && (newPos.x >= -350.0f))
		{
			return TRUE;
		}

		if ((newPos.z >= 1025.0f) && (newPos.x <= -570.0f))
		{
			return TRUE;
		}

		if ((newPos.z >= 1025.0f) && (newPos.x >= -190.0f))
		{
			return TRUE;
		}

		if ((newPos.z >= 1430.0f))
		{
			return TRUE;
		}

		break;

	case DUNGEON_SECOND_FLOOR:

		if (newPos.z <= -635.0f)
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x >= -110.0f))
		{
			return TRUE;
		}

		if (((newPos.z >= -640.0f) && (newPos.z <= -315.0f)) && (newPos.x <= -173.0f))
		{
			return TRUE;
		}

		if (newPos.x <= -410.0f)
		{
			return TRUE;
		}

		if (newPos.x >= 125.0f)
		{
			return TRUE;
		}

		if (newPos.z >= 230.0f)
		{
			return TRUE;
		}

		break;
	}

	return FALSE;

}