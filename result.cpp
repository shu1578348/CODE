//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : �r�R�@�G��
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(7)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 


#define TEXTURE_SWTCH_W				(SCREEN_WIDTH/3)	// �w�i�T�C�Y
#define TEXTURE_SWTCH_H				(SCREEN_HEIGHT/4)	// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*vertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *texturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg001.jpg",
	"data/TEXTURE/result_logo.png",
	"data/TEXTURE/num.png",
	"data/TEXTURE/bar_white.png",
	"data/TEXTURE/result.png",
	"data/TEXTURE/bg000.jpg",
	"data/TEXTURE/bg001.jpg",
};


static BOOL						use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					w, h;					// ���ƍ���
static XMFLOAT3					pos;						// �|���S���̍��W
static int						texNo;					// �e�N�X�`���ԍ�

static BOOL						load = FALSE;

static SWITCH2	swtch;
static float	alpha;
static BOOL		flag_alpha;

int SwitchResultWait;
BOOL starthoi;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			texturName[i],
			NULL,
			NULL,
			&texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);


	// �ϐ��̏�����
	use   = TRUE;
	w     = TEXTURE_WIDTH;
	h     = TEXTURE_HEIGHT;
	pos   = { w / 2, 50.0f, 0.0f };
	texNo = 0;

	// BGM�Đ�

	// �ϐ��̏�����
	swtch.w = TEXTURE_SWTCH_W;
	swtch.h = TEXTURE_SWTCH_H;//  x				H				Z
	swtch.pos = XMFLOAT3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT * 3 / 5, 0.0f);
	//���摜����
	swtch.texNo = 3;

	swtch.u = 0.0f;
	swtch.v = 0.0f;

	SwitchResultWait = 0;


	starthoi = FALSE;
	//}


	alpha = 1.0f;
	flag_alpha = TRUE;


	load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	if (load == FALSE) return;

	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) || (IsButtonTriggered(1, BUTTON_C)))
	{// Enter��������A�X�e�[�W��؂�ւ���
		SetFade(FADE_OUT, MODE_TITLE);
	}
	// �Q�[���p�b�h�œ��͏���
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}


	if (SwitchResultWait > 50)		//���������80�t���[����V�[���J��
	{
		SetMode(MODE_TITLE);
		SwitchResultWait = 0;
		starthoi = FALSE;
	}

	if (starthoi == TRUE)
	{


		SwitchResultWait++;

	}




#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ��������������
	int number = GetScore();

	{
		// ���U���g�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture[4]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(vertexBuffer, 0.0f, 0.0f, w, h, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

	}

	// �X�R�A�\��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &texture[2]);

		// ��������������
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float pw = 16*4;			// �X�R�A�̕\����
			float ph = 32*4;			// �X�R�A�̕\������
			float px = 600.0f - i*pw;	// �X�R�A�̕\���ʒuX
			float py = 300.0f;			// �X�R�A�̕\���ʒuY

			float tw = 1.0f / 10;		// �e�N�X�`���̕�
			float th = 1.0f / 1;		// �e�N�X�`���̍���
			float tx = x * tw;			// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(vertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}



}




