//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : �r�R�@�G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(1)				// �e�N�X�`���̐�

#define	FADE_RATE					(0.02f)			// �t�F�[�h�W��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*vertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *texturName[TEXTURE_MAX] = {
	"data/TEXTURE/fade_black.png",
};

static BOOL			use;				// TRUE:�g���Ă���  FALSE:���g�p
static float		w, h;			// ���ƍ���
static XMFLOAT3		pos;				// �|���S���̍��W
static int			texNo;			// �e�N�X�`���ԍ�

FADE				fade = FADE_IN;	// �t�F�[�h�̏��
int					modeNext;			// ���̃��[�h
XMFLOAT4			color;			// �t�F�[�h�̃J���[�i���l�j

static BOOL			load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFade(void)
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


	// �v���C���[�̏�����
	use   = TRUE;
	w    = TEXTURE_WIDTH;
	h     = TEXTURE_HEIGHT;
	pos   = { 0.0f, 0.0f, 0.0f };
	texNo = 0;

	fade  = FADE_IN;
	color = { 1.0, 0.0, 0.0, 1.0 };

	load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
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
void UpdateFade(void)
{

	if (fade != FADE_NONE)
	{// �t�F�[�h������
		if (fade == FADE_OUT)
		{	
			//-----------------------------------------------------------------
			// �t�F�[�h�A�E�g����
			//-----------------------------------------------------------------
			color.w += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���
			if (color.w >= 1.0f)
			{
				// ���Ă���Ȃ�S���~�߂�
				StopSound();

				// �t�F�[�h�C�������ɐ؂�ւ�
				color.w = 1.0f;
				SetFade(FADE_IN, modeNext);

				// ���[�h��ݒ�
				SetMode(modeNext);
			}

		}
		else if (fade == FADE_IN)
		{
			//-------------------------------------------------------------------
			// �t�F�[�h�C������
			//-------------------------------------------------------------------
			color.w -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
			if (color.w <= 0.0f)
			{
				// �t�F�[�h�����I��
				color.w = 0.0f;
				SetFade(FADE_NONE, modeNext);
			}

		}
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	// PrintDebugProc("\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFade(void)
{
	if (fade == FADE_NONE) return;	// �t�F�[�h���Ȃ��̂Ȃ�`�悵�Ȃ�

	// ���Z�����ɐݒ�
	//SetBlendState(BLEND_MODE_ADD);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &texture[texNo]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		//SetVertex(0.0f, 0.0f, SCREEN_WIDTH, TEXTURE_WIDTH, 0.0f, 0.0f, 1.0f, 1.0f);
		SetSpriteColor(vertexBuffer, SCREEN_WIDTH/2, TEXTURE_WIDTH/2, SCREEN_WIDTH, TEXTURE_WIDTH, 0.0f, 0.0f, 1.0f, 1.0f,
			color);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


}


//=============================================================================
// �t�F�[�h�̏�Ԑݒ�
//=============================================================================
void SetFade(FADE nFade, int nextMode)
{
	fade = nFade;
	modeNext = nextMode;
}

//=============================================================================
// �t�F�[�h�̏�Ԏ擾
//=============================================================================
FADE GetFade(void)
{
	return fade;
}



