//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_MAX		(7)		  // �G�l�~�[�̐�
#define	ENEMY_SIZE_X	(10.0f)	  // �����蔻��̑傫��
#define	ENEMY_SIZE_Y	(10.0f)
#define	ENEMY_SIZE_Z	(20.0f)
#define	ENEMY_SIZE		(10.0f)	  // �����蔻��p(BC�p)
#define ENEMY_RANGE		(100.0f)  // �G�l�~�[�̒ǔ��܂ł̋���

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// ���f���̈ʒu
	XMFLOAT3	tPos;		// �^�[�Q�b�g�̃|�W�V����
	XMFLOAT3	rot;		// ���f���̌���(��])
	XMFLOAT3	scl;		// ���f���̑傫��(�X�P�[��)
	XMFLOAT3	size;		// �����蔻��̑傫��

	BOOL		use;		// TRUE:�g�p / FALSE:���g�p
	BOOL		draw;		// TRUE:�\�� / FALSE:��\��
	BOOL		state;		// TRUE:�ړ��\ / FALSE:�ړ��s�\
	BOOL		chase;		// TRUE:�ǐՒ� / FALSE:��ǐՒ�

	float		spd;		// �ړ��X�s�[�h
	float		hp;			// �q�b�g�|�C���g
	int			condition;	// ��Ԉُ� ( 0:�ʏ� / 1:���� / 2:�� )
	int			modelNo;

	float		time;		// ����
	int			tblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;		// ���̃e�[�u���̃f�[�^��
	int			shadowIdx;	// �e�̃C���f�b�N�X�ԍ�
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);