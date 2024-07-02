//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : �r�R�@�G��
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�

#define	PARTICLE_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_SIZE_Y		(40.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_PARTICLE	(5.0f)		// �ړ����x

#define	MAX_PARTICLE		(8000)		// �p�[�e�B�N���ő吔

#define	DISP_SHADOW						// �e�̕\��
 #undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		tPos;			// �^�[�Q�b�g���W
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nIdxShadow;		// �eID
	int				nLife;			// ����
	int             type;			// 0:���Z / 1:���Z
	BOOL			use;			// �g�p���Ă��邩�ǂ���

} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* vertexBuffer = NULL;		// ���_�o�b�t�@

static ID3D11ShaderResourceView* texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							texNo;					// �e�N�X�`���ԍ�

static PARTICLE					aParticle[MAX_PARTICLE];		// �p�[�e�B�N�����[�N
static XMFLOAT3					posBase;						// �r���{�[�h�����ʒu
static float					fWidthBase = 5.0f;			// ��̕�
static float					fHeightBase = 10.0f;			// ��̍���
static float					roty = 0.0f;					// �ړ�����
static float					spd = 0.0f;					// �ړ��X�s�[�h

static char* textureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL							load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	// ���_���̍쐬
	MakeVertexParticle();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			textureName[i],
			NULL,
			NULL,
			&texture[i],
			NULL);
	}

	texNo = 0;

	// �p�[�e�B�N�����[�N�̏�����
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		aParticle[nCntParticle].tPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		aParticle[nCntParticle].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&aParticle[nCntParticle].material, sizeof(aParticle[nCntParticle].material));
		aParticle[nCntParticle].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		aParticle[nCntParticle].fSizeX = PARTICLE_SIZE_X;
		aParticle[nCntParticle].fSizeY = PARTICLE_SIZE_Y;
		aParticle[nCntParticle].nIdxShadow = -1;
		aParticle[nCntParticle].nLife = 0;
		aParticle[nCntParticle].use = FALSE;
	}

	posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	roty = 0.0f;
	spd = 0.0f;

	load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	if (load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (texture[nCntTex] != NULL)
		{
			texture[nCntTex]->Release();
			texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (vertexBuffer != NULL)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{

		switch (aParticle[nCntParticle].type)
		{
		case 0:
		case 1:

			if (aParticle[nCntParticle].use)
			{// �g�p��
			aParticle[nCntParticle].pos.x += aParticle[nCntParticle].move.x;
			aParticle[nCntParticle].pos.z += aParticle[nCntParticle].move.z;

			aParticle[nCntParticle].pos.y += aParticle[nCntParticle].move.y;

			aParticle[nCntParticle].move.x += (0.0f - aParticle[nCntParticle].move.x) * 0.015f;
			aParticle[nCntParticle].move.y += (0.0f - aParticle[nCntParticle].move.y) * 0.015f;
			aParticle[nCntParticle].move.z += (0.0f - aParticle[nCntParticle].move.z) * 0.015f;

#ifdef DISP_SHADOW
			if (aParticle[nCntParticle].nIdxShadow != -1)
			{// �e�g�p��
				float colA;

				// �e�̈ʒu�ݒ�
				SetPositionShadow(aParticle[nCntParticle].nIdxShadow, XMFLOAT3(aParticle[nCntParticle].pos.x, 0.1f, aParticle[nCntParticle].pos.z));

				// �e�̐F�̐ݒ�
				colA = aParticle[nCntParticle].material.Diffuse.w;
				SetColorShadow(aParticle[nCntParticle].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
			}
#endif


			}

			break;

		case 2:

			if (aParticle[nCntParticle].use)
			{// �g�p��

				if (aParticle[nCntParticle].pos.y <= 1.0f)
				{
					float fAngle, fLength;

					fAngle = (float)(rand() % 628 - 314);
					fLength = (rand() % (int)(10 * 200) / 100.0f - 10);

					aParticle[nCntParticle].pos.y = 0.5f;

					aParticle[nCntParticle].pos.x -= sinf(fAngle) * fLength * 1.2f;
					aParticle[nCntParticle].pos.z -= cosf(fAngle) * fLength * 1.2f;

				}
				else
				{
					aParticle[nCntParticle].pos.x += aParticle[nCntParticle].move.x;
					aParticle[nCntParticle].pos.z += aParticle[nCntParticle].move.z;

					aParticle[nCntParticle].pos.y += aParticle[nCntParticle].move.y;

					aParticle[nCntParticle].move.x += (0.0f - aParticle[nCntParticle].move.x) * 0.015f;
					aParticle[nCntParticle].move.y += (0.0f - aParticle[nCntParticle].move.y) * 0.015f;
					aParticle[nCntParticle].move.z += (0.0f - aParticle[nCntParticle].move.z) * 0.015f;
				}

			}

			break;

		case 3:

			if (aParticle[nCntParticle].use)
			{// �g�p���̏ꍇ�̂ݏ������s��

			    // �p�[�e�B�N���̈ړ�
				aParticle[nCntParticle].pos.x += aParticle[nCntParticle].move.x;
				aParticle[nCntParticle].pos.z += aParticle[nCntParticle].move.z;
				aParticle[nCntParticle].pos.y += aParticle[nCntParticle].move.y;

				aParticle[nCntParticle].move.x += (0.0f - aParticle[nCntParticle].move.x) * 0.015f;
				aParticle[nCntParticle].move.y += (0.0f - aParticle[nCntParticle].move.y) * 0.015f;
				aParticle[nCntParticle].move.z += (0.0f - aParticle[nCntParticle].move.z) * 0.015f;

				float distance = GetDistance(aParticle[nCntParticle].pos, aParticle[nCntParticle].tPos);

				if (distance < 10.1f)
				{
					aParticle[nCntParticle].move.x *= 0.0f;
					aParticle[nCntParticle].move.y *= 0.0f;
					aParticle[nCntParticle].move.z *= 0.0f;

				}

			}

		}

		aParticle[nCntParticle].nLife--;
		if (aParticle[nCntParticle].nLife <= 0)
		{
			aParticle[nCntParticle].use = FALSE;
			ReleaseShadow(aParticle[nCntParticle].nIdxShadow);
			aParticle[nCntParticle].nIdxShadow = -1;
		}
		else
		{

			if (aParticle[nCntParticle].nLife <= 200)
			{
				// ���l�ݒ�
				aParticle[nCntParticle].material.Diffuse.w -= 0.02f;

				aParticle[nCntParticle].scale.x -= aParticle[nCntParticle].scale.x / 80;
				aParticle[nCntParticle].scale.y -= aParticle[nCntParticle].scale.y / 80;
				aParticle[nCntParticle].scale.z -= aParticle[nCntParticle].scale.z / 80;

				if (aParticle[nCntParticle].material.Diffuse.w < 0.0f)
				{
					aParticle[nCntParticle].use = FALSE;
					aParticle[nCntParticle].material.Diffuse.w = 0.0f;
				}
			}

			if (aParticle[nCntParticle].nLife <= 80)
			{
				aParticle[nCntParticle].material.Diffuse.x = aParticle[nCntParticle].material.Diffuse.x - (float)(80 - aParticle[nCntParticle].nLife) / 80 * aParticle[nCntParticle].material.Diffuse.x;
				aParticle[nCntParticle].material.Diffuse.y = aParticle[nCntParticle].material.Diffuse.y - (float)(80 - aParticle[nCntParticle].nLife) / 80 * aParticle[nCntParticle].material.Diffuse.y;
				aParticle[nCntParticle].material.Diffuse.z = aParticle[nCntParticle].material.Diffuse.z - (float)(80 - aParticle[nCntParticle].nLife) / 80 * aParticle[nCntParticle].material.Diffuse.z;
			}

		}

	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;

	PLAYER* player = GetPlayer();

	CAMERA* cam = GetCamera();

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// Z��r����
	SetDepthEnable(FALSE);

	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[texNo]);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		switch (aParticle[nCntParticle].type)
		{
		case 0:
		case 2:
		case 3:
			// ���Z�����ɐݒ�
			SetBlendState(BLEND_MODE_ADD);
			break;

		case 1:
			// ���Z�����ɐݒ�
			SetBlendState(BLEND_MODE_SUBTRACT);
			break;

		}


		if (aParticle[nCntParticle].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			// ������������������
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(aParticle[nCntParticle].scale.x, aParticle[nCntParticle].scale.y, aParticle[nCntParticle].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(aParticle[nCntParticle].pos.x, aParticle[nCntParticle].pos.y, aParticle[nCntParticle].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			//mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&player[19].parent->mtxWorld));

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(aParticle[nCntParticle].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);

		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);

	// �t�H�O�L��
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(PARTICLE_SIZE_X  / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(PARTICLE_SIZE_X  / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(vertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	aParticle[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 tPos,XMFLOAT3 move, XMFLOAT3 scale, XMFLOAT4 col, float fSizeX, float fSizeY, int nLife, int type)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!aParticle[nCntParticle].use)
		{
			aParticle[nCntParticle].pos  = pos;
			aParticle[nCntParticle].tPos = tPos;
			aParticle[nCntParticle].rot   = { 0.0f, 0.0f, 0.0f };
			aParticle[nCntParticle].scale = scale;
			aParticle[nCntParticle].move = move;
			aParticle[nCntParticle].material.Diffuse = col;
			aParticle[nCntParticle].fSizeX = fSizeX;
			aParticle[nCntParticle].fSizeY = fSizeY;
			aParticle[nCntParticle].nLife = nLife;
			aParticle[nCntParticle].use   = TRUE;
			aParticle[nCntParticle].type  = type;


			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			aParticle[nCntParticle].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
			if (aParticle[nCntParticle].nIdxShadow != -1)
			{
				SetColorShadow(aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}
