//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX		(1)					// �v���C���[�̐�

#define	PLAYER_SIZE		(6.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	XMFLOAT3		pos;		// �|���S���̈ʒu
	XMFLOAT3		rot;		// �|���S���̌���(��])
	XMFLOAT3		scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���b�N�X

	BOOL			load;
	BOOL			use;
	BOOL			jump;
	DX11_MODEL		model;		// ���f�����

	float			spd;		// �ړ��X�s�[�h
	float			dir;		// ����
	float			size;		// �����蔻��̑傫��
	float           sp;			// �U���p�̃|�C���g
	int				shadowIdx;	// �e��Index
	int				jumpCnt;	// �W�����v�J�E���^�[
	int				hp;			// �q�b�g�|�C���g

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float			time;		// ���`��ԗp
	int				tblNo;		// �s���f�[�^�̃e�[�u���ԍ�
	int				tblMax;		// ���̃e�[�u���̃f�[�^��

	BOOL			anim;       // �A�j���[�V�����̍Đ�����

	int				pose;		// �v���C���[�̎p��
	int				tool;		// �����i

	// �e�́ANULL�A�q���͐e�̃A�h���X������(�Z�[�u�����[�h�̎��́������C�����鎖)
	PLAYER* parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);

float GetDistance(XMFLOAT3 mpos, XMFLOAT3 ypos);

float Lerp(float a, float b, float t);