//=============================================================================
//
// TOOL���� [tool.cpp]
// Author : �r�R�@�G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "score.h"
#include "sprite.h"
#include "model.h"
#include "player.h"
#include "hpbar.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)
#define TEXTURE_HEIGHT				(100) 
#define TEXTURE_MAX					(9)		// �e�N�X�`���̐�


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* vertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* texturName[] = {
	"data/TEXTURE/sword.png",
	"data/TEXTURE/torch.png",
	"data/TEXTURE/knife_normal.png",
	"data/TEXTURE/knife_fire.png",
	"data/TEXTURE/knife_poison.png",
	"data/TEXTURE/knife_normal3.png",
	"data/TEXTURE/knife_fire3.png",
	"data/TEXTURE/knife_poison3.png",
	"data/TEXTURE/square.png",
};


static BOOL						use;						// true:�g���Ă���  false:���g�p
static float					w, h;					// ���ƍ���
static XMFLOAT3					pos;						// �|���S���̍��W
static int						texNo;					// �e�N�X�`���ԍ�

static int						tool;			    		//�c�[��

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTool(void)
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
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);


	// �o�[�̏�����
	use = TRUE;
	w = TEXTURE_WIDTH;
	h = TEXTURE_HEIGHT;
	pos = { 50.0f, 130.0f, 0.0f };
	texNo = 0;

	tool = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTool(void)
{
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

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTool(void)
{


#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", pos.x, pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTool(void)
{
	PLAYER * player = GetPlayer();

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

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[8]);

	// TOOL�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = pos.x;		// TOOL�̕\���ʒuX
	float py = pos.y;		// TOOL�̕\���ʒuY
	float pw = w;			// TOOL�̕\����
	float ph = h;			// TOOL�̕\������

	float tw = 1.0f;		// �e�N�X�`���̕�
	float th = 1.0f;		// �e�N�X�`���̍���
	float tx = 0.0f;	    // �e�N�X�`���̍���X���W
	float ty = 0.0f;		// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(vertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

	// ���_�o�b�t�@�ݒ�
	stride = sizeof(VERTEX_3D);
	offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[player[0].tool]);

	// TOOL�̈ʒu��e�N�X�`���[���W�𔽉f
	px = pos.x;			// TOOL�̕\���ʒuX
	py = pos.y;			// TOOL�̕\���ʒuY
	pw = w;				// TOOL�̕\����
	ph = h;				// TOOL�̕\������

	tw = 1.0f;		// �e�N�X�`���̕�
	th = 1.0f;		// �e�N�X�`���̍���
	tx = 0.0f;	    // �e�N�X�`���̍���X���W
	ty = 0.0f;		// �e�N�X�`���̍���Y���W

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(vertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);


}
