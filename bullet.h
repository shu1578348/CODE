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
#define BULLET_MAX		(256)	// �o���b�g��Max��
#define BULLET_SPEED	(4.0f)	// �o���b�g�̈ړ��X�s�[�h
#define BULLET_SIZE		(8.0f)	// �o���b�g�̃T�C�Y
#define BULLET_DELETE   (100)	// �o���b�g��������܂ł̎���
#define BULLET_TIME		(40)	// �o���b�g�̘A�ˊԊu

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BULLET
{
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// ���f���̈ʒu
	XMFLOAT3	rot;		// ���f���̌���(��])
	XMFLOAT3	scl;		// ���f���̑傫��(�X�P�[��)

	BOOL		use;		// TRUE:�g�p / FALSE:���g�p
	BOOL		shot;		// TRUE:���� / FALSE:������

	float		spd;		// �ړ��X�s�[�h
	float		dir;		// ����
	float		size;		// �����蔻��̃T�C�Y
	int			shotDir;	// ���˕��� 0:���� 1:���O 2:�E�O
	int			modelNo;	// ���f���̎��
	int			type;		// �e�̎��
	int			dTime;		// ������܂ł̎���
	int			shadowIdx;	// �e�̃C���f�b�N�X�ԍ�
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, BOOL shot ,int type, int shotDir);

BULLET* GetBullet(void);

void BulletEfect(XMFLOAT3 pos, int type);

