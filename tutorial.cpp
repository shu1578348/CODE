//=============================================================================
//
//�@�`���[�g���A����ʏ��� [tutorial.cpp]
// Author : 
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(6)				// �e�N�X�`���̐�

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
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* texturName[TEXTURE_MAX] = {
	"data/TEXTURE/bg001.jpg",
	"data/TEXTURE/Tutorial_logo.png",
	"data/TEXTURE/num.png",
	"data/TEXTURE/bar_white.png",
	"data/TEXTURE/tutorial.jpg",
	"data/TEXTURE/bg.jpg",
};


static BOOL						use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					w, h;					// ���ƍ���
static XMFLOAT3					pos;						// �|���S���̍��W
static int						texNo;					// �e�N�X�`���ԍ�

static BOOL						g_Load = FALSE;

static SWITCH1	g_swtch;
static float	alpha;
static BOOL		flag_alpha;

int SwitchTutorialWait;
BOOL starthoi1;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(void)
{
	ID3D11Device* pDevice = GetDevice();

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
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	use = TRUE;
	w = TEXTURE_WIDTH;
	h = TEXTURE_HEIGHT;
	pos = { w / 2, 50.0f, 0.0f };
	texNo = 0;


	// �ϐ��̏�����
	g_swtch.w = TEXTURE_SWTCH_W;
	g_swtch.h = TEXTURE_SWTCH_H;//  x				H				Z
	g_swtch.pos = XMFLOAT3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT * 3 / 5, 0.0f);
	//���摜����
	g_swtch.texNo = 3;

	g_swtch.u = 0.0f;
	g_swtch.v = 0.0f;

	SwitchTutorialWait = 0;


	starthoi1 = FALSE;
	//}


	alpha = 1.0f;
	flag_alpha = TRUE;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{

	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(1,BUTTON_C))
	{// Enter��������A�X�e�[�W��؂�ւ���
		SetFade(FADE_OUT, MODE_GAME);
	}


	if (SwitchTutorialWait > 50)		//���������80�t���[����V�[���J��
	{
		SetMode(MODE_GAME);
		SwitchTutorialWait = 0;
		starthoi1 = FALSE;
	}

	if (starthoi1 == TRUE)
	{


		SwitchTutorialWait++;

	}




#ifdef _DEBUG	// �f�o�b�O����\������

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//�w�i
	{
		// ���U���g�̔w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &texture[4]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, w, h, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

	}

}