//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : �r�R�@�G��
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "meshfield.h"
#include "renderer.h"
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX		(1)				// �e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11Buffer					*g_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@

static ID3D11ShaderResourceView		*texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							texNo;				// �e�N�X�`���ԍ�

static XMFLOAT3		posField;								// �|���S���\���ʒu�̒��S���W
static XMFLOAT3		rotField;								// �|���S���̉�]�p

static int			nNumBlockXField, nNumBlockZField;	// �u���b�N��
static int			nNumVertexField;						// �����_��	
static int			nNumVertexIndexField;					// ���C���f�b�N�X��
static int			nNumPolygonField;						// ���|���S����
static float		fBlockSizeXField, fBlockSizeZField;	// �u���b�N�T�C�Y

static char* textureName[] = {
	"data/TEXTURE/field004.jpg",
	"data/TEXTURE/field002.jpg",
};


// �g�̏���

static VERTEX_3D	*g_Vertex = NULL;

// �g�̍��� = sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
static XMFLOAT3		center;					// �g�̔����ꏊ
static float		time = 0.0f;				// �g�̌o�ߎ���
static float		wave_frequency  = 2.0f;	// �g�̎��g��
static float		wave_correction = 0.02f;	// �g�̋����␳
static float		wave_amplitude  = 20.0f;	// �g�̐U��


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(XMFLOAT3 pos, XMFLOAT3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	// �|���S���\���ʒu�̒��S���W��ݒ�
	posField = pos;

	rotField = rot;

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			textureName[i],
			NULL,
			NULL,
			&texture[i],
			NULL);
	}

	texNo = 0;

	// �u���b�N���̐ݒ�
	nNumBlockXField = nNumBlockX;
	nNumBlockZField = nNumBlockZ;

	// ���_���̐ݒ�
	nNumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	nNumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	nNumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	fBlockSizeXField = fBlockSizeX;
	fBlockSizeZField = fBlockSizeZ;


	// ���_�����������ɍ���Ă����i�g�ׁ̈j
	// �g�̏���
	// �g�̍��� = sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
	g_Vertex = new VERTEX_3D[nNumVertexField];
	center = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �g�̔����ꏊ
	time = 0.0f;								// �g�̌o�ߎ���(�{�Ɓ[�Ƃœ����O���ɂȂ�)
	wave_frequency = 1.0f;					// �g�̎��g���i�㉺�^���̑����j
	wave_correction = 0.02f;					// �g�̋����␳�i�ς��Ȃ��Ă��ǂ��Ǝv���j
	wave_amplitude = 30.0f;					// �g�̐U��(�g�̍���)

	for (int z = 0; z < (nNumBlockZField + 1); z++)
	{
		for (int x = 0; x < (nNumBlockXField + 1); x++)
		{
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.x = -(nNumBlockXField / 2.0f) * fBlockSizeXField + x * fBlockSizeXField;
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.y = 0.0f;
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.z = (nNumBlockZField / 2.0f) * fBlockSizeZField - z * fBlockSizeZField;

			float dx = g_Vertex[z * (nNumBlockXField + 1) + x].Position.x - center.x;
			float dz = g_Vertex[z * (nNumBlockXField + 1) + x].Position.z - center.z;

			// �g��̒��S�_����̋����𓾂�
			float len = (float)sqrt(dx * dx + dz * dz);

			// �g�̍������Asin�֐��œ���
			// �g�̍����@= sin( -�o�ߎ��� * ���g�� + ���� * �����␳ ) * �U��
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.y = sinf(-time * wave_frequency + len * wave_correction) * wave_amplitude;


			// �@���̐ݒ�
			g_Vertex[z * (nNumBlockXField + 1) + x].Normal = XMFLOAT3(0.0f, 1.0, 0.0f);

			// ���ˌ��̐ݒ�
			g_Vertex[z * (nNumBlockXField + 1) + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			float texSizeX = 1.0f;
			float texSizeZ = 1.0f;
			g_Vertex[z * (nNumBlockXField + 1) + x].TexCoord.x = texSizeX * x;
			g_Vertex[z * (nNumBlockXField + 1) + x].TexCoord.y = texSizeZ * z;
		}

	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * nNumVertexField;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �C���f�b�N�X�o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(unsigned short) * nNumVertexIndexField;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_IndexBuffer);


	{//���_�o�b�t�@�̒��g�𖄂߂�

		// ���_�o�b�t�@�ւ̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

		memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*nNumVertexField);

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		// �C���f�b�N�X�o�b�t�@�̃|�C���^���擾
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		unsigned short *pIdx = (unsigned short*)msr.pData;

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < nNumBlockZField; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (nNumBlockXField + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (nNumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxZ < (nNumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (nNumBlockXField + 1) + nNumBlockXField;
				nCntIdx++;
			}
		}

		GetDeviceContext()->Unmap(g_IndexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	// �C���f�b�N�X�o�b�t�@�̉��
	if (g_IndexBuffer) {
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer) {
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (texture[i])
		{
			texture[i]->Release();
			texture[i] = NULL;
		}
	}

	if (g_Vertex)
	{
		delete[] g_Vertex;
		g_Vertex = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
	// �g�̏���
	float dt = 0.03f;

	for (int z = 0; z < nNumBlockZField; z++)
	{
		for (int x = 0; x < nNumBlockXField; x++)
		{
			float dx = g_Vertex[z * (nNumBlockXField + 1) + x].Position.x - center.x;
			float dz = g_Vertex[z * (nNumBlockXField + 1) + x].Position.z - center.z;

			// �g��̒��S�_����̋����𓾂�
			float len = (float)sqrt(dx * dx + dz * dz);

			// �g�̍������Asin�֐��œ���
			g_Vertex[z * (nNumBlockXField + 1) + x].Position.y = 0.0f;
		}

	}
	time += dt;


	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* pVtx = (VERTEX_3D*)msr.pData;

	// �S���_���𖈉�㏑�����Ă���̂�DX11�ł͂��̕�����������ł�
	memcpy(pVtx, g_Vertex, sizeof(VERTEX_3D)*nNumVertexField);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[texNo]);


	XMMATRIX mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(rotField.x, rotField.y, rotField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(posField.x, posField.y, posField.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);


	// �|���S���̕`��
	GetDeviceContext()->DrawIndexed(nNumVertexIndexField, 0, 0);
}



BOOL RayHitField(XMFLOAT3 pos, XMFLOAT3 *HitPosition, XMFLOAT3 *Normal)
{
	XMFLOAT3 start = pos;
	XMFLOAT3 end = pos;

	// ���_���������̃e�X�g
	if (start.x == 0.0f)
	{
		start.x += 1.0f;
		end.x += 1.0f;
	}
	if (start.z == 0.0f)
	{
		start.z -= 1.0f;
		end.z -= 1.0f;
	}

	// �����ォ��A�Y�h�[���Ɖ��փ��C���΂�
	start.y += 100.0f;
	end.y -= 1000.0f;

	// ����������������ׂɑS�����ł͂Ȃ��āA���W����|���S��������o���Ă���
	float fz = (nNumBlockXField / 2.0f) * fBlockSizeXField;
	float fx = (nNumBlockZField / 2.0f) * fBlockSizeZField;
	int sz = (int)((-start.z+fz) / fBlockSizeZField);
	int sx = (int)(( start.x+fx) / fBlockSizeXField);
	int ez = sz + 1;
	int ex = sx + 1;

	if ((sz < 0) || (sz > nNumBlockZField - 1) ||
		(sx < 0) || (sx > nNumBlockXField - 1))
	{
		*Normal = {0.0f, 1.0f, 0.0f};
		return FALSE;
	}


	// �K�v�����������J��Ԃ�
	for (int z = sz; z < ez; z++)
	{
		for (int x = sx; x < ex; x++)
		{
			XMFLOAT3 p0 = g_Vertex[z * (nNumBlockXField + 1) + x].Position;
			XMFLOAT3 p1 = g_Vertex[z * (nNumBlockXField + 1) + (x + 1)].Position;
			XMFLOAT3 p2 = g_Vertex[(z + 1) * (nNumBlockXField + 1) + x].Position;
			XMFLOAT3 p3 = g_Vertex[(z + 1) * (nNumBlockXField + 1) + (x + 1)].Position;

			// �O�p�|���S��������Q�����̓����蔻��
			BOOL ans = RayCast(p0, p2, p1, start, end, HitPosition, Normal);
			if (ans)
			{
				return TRUE;
			}

			ans = RayCast(p1, p2, p3, start, end, HitPosition, Normal);
			if (ans)
			{
				return TRUE;
			}
		}
	}


	return FALSE;
}
