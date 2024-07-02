//=============================================================================
//
// �e���ˏ��� [bullet.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_BULLET_MAX	(256)	// �o���b�g��Max��
#define ENEMY_BULLET_SPEED	(4.0f)	// �o���b�g�̈ړ��X�s�[�h
#define ENEMY_BULLET_SIZE	(9.0f)	// �o���b�g�̃T�C�Y
#define ENEMY_BULLET_DELETE (200)	// �o���b�g��������܂ł̎���
#define ENEMY_BULLET_TIME	(40)	// �o���b�g�̘A�ˊԊu

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMYBULLET
{
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// ���f���̈ʒu
	XMFLOAT3	rot;		// ���f���̌���(��])
	XMFLOAT3	scl;		// ���f���̑傫��(�X�P�[��)

	BOOL		use;		// TRUE:�g�p / FALSE:���g�p

	float		spd;		// �ړ��X�s�[�h
	float		dir;		// ����
	float		size;		// �����蔻��̃T�C�Y
	int			type;		// �e�̎��
	int			dTime;		// ������܂ł̎���
	int			shadowIdx;	// �e�̃C���f�b�N�X�ԍ�
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);

void SetEnemyBullet(XMFLOAT3 pos, XMFLOAT3 rot,int type);

ENEMYBULLET* GetEnemyBullet(void);

void EnemyBulletEfect(XMFLOAT3 pos, int type);

