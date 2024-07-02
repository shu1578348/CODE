//=============================================================================
//
// �G�l�~�[���f������ [boss.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BOSS_MAX		(1)			  // �G�l�~�[�̐�
#define	BOSS_SIZE_X		(120.0f)	  // �����蔻��̑傫��
#define	BOSS_SIZE_Y		(200.0f)
#define	BOSS_SIZE_Z		(200.0f)
#define BOSS_SIZE		(50.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BOSS
{
	XMFLOAT4X4	mtxWorld;	// ���[���h�}�g���b�N�X
	XMFLOAT3	pos;		// ���f���̈ʒu
	XMFLOAT3	tPos;		// �^�[�Q�b�g�̃|�W�V����
	XMFLOAT3	rot;		// ���f���̌���(��])
	XMFLOAT3    aRot;		// �U������
	XMFLOAT3	scl;		// ���f���̑傫��(�X�P�[��)
	XMFLOAT3	size;		// ���f���̓����蔻��

	BOOL		use;		// TRUE:�g�p / FALSE:���g�p
	BOOL		draw;		// TRUE:�\�� / FALSE:��\��
	BOOL		state;		// TRUE:�ړ��\ / FALSE:�ړ��s�\
	BOOL		chase;		// TRUE:�ǐՒ� / FALSE:��ǐՒ�

	float		spd;		// �ړ��X�s�[�h
	float		hp;			// �q�b�g�|�C���g
	int			condition;	// ��Ԉُ� ( 0:�ʏ� / 1:���� / 2:�� )
	int			mode;		// �U�����[�h
	int			modelNo;

	float		time;		// ����
	int			tblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int			tblMax;		// ���̃e�[�u���̃f�[�^��
	int			shadowIdx;	// �e�̃C���f�b�N�X�ԍ�
};

struct BEZIER
{
	XMFLOAT3 start;
	XMFLOAT3 p1;
	XMFLOAT3 end;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

BOSS* GetBoss(void);

XMFLOAT3 CalculateBezierPoint(const XMFLOAT3& start, const XMFLOAT3& p1, const XMFLOAT3& end, float t);