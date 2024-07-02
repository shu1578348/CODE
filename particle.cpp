//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 荒山　秀磨
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数

#define	PARTICLE_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_SIZE_Y		(40.0f)		// 頂点サイズ
#define	VALUE_MOVE_PARTICLE	(5.0f)		// 移動速度

#define	MAX_PARTICLE		(8000)		// パーティクル最大数

#define	DISP_SHADOW						// 影の表示
 #undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		tPos;			// ターゲット座標
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命
	int             type;			// 0:加算 / 1:減算
	BOOL			use;			// 使用しているかどうか

} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* vertexBuffer = NULL;		// 頂点バッファ

static ID3D11ShaderResourceView* texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							texNo;					// テクスチャ番号

static PARTICLE					aParticle[MAX_PARTICLE];		// パーティクルワーク
static XMFLOAT3					posBase;						// ビルボード発生位置
static float					fWidthBase = 5.0f;			// 基準の幅
static float					fHeightBase = 10.0f;			// 基準の高さ
static float					roty = 0.0f;					// 移動方向
static float					spd = 0.0f;					// 移動スピード

static char* textureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL							load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	// 頂点情報の作成
	MakeVertexParticle();

	// テクスチャ生成
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

	// パーティクルワークの初期化
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
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if (load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (texture[nCntTex] != NULL)
		{
			texture[nCntTex]->Release();
			texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (vertexBuffer != NULL)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}

	load = FALSE;
}

//=============================================================================
// 更新処理
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
			{// 使用中
			aParticle[nCntParticle].pos.x += aParticle[nCntParticle].move.x;
			aParticle[nCntParticle].pos.z += aParticle[nCntParticle].move.z;

			aParticle[nCntParticle].pos.y += aParticle[nCntParticle].move.y;

			aParticle[nCntParticle].move.x += (0.0f - aParticle[nCntParticle].move.x) * 0.015f;
			aParticle[nCntParticle].move.y += (0.0f - aParticle[nCntParticle].move.y) * 0.015f;
			aParticle[nCntParticle].move.z += (0.0f - aParticle[nCntParticle].move.z) * 0.015f;

#ifdef DISP_SHADOW
			if (aParticle[nCntParticle].nIdxShadow != -1)
			{// 影使用中
				float colA;

				// 影の位置設定
				SetPositionShadow(aParticle[nCntParticle].nIdxShadow, XMFLOAT3(aParticle[nCntParticle].pos.x, 0.1f, aParticle[nCntParticle].pos.z));

				// 影の色の設定
				colA = aParticle[nCntParticle].material.Diffuse.w;
				SetColorShadow(aParticle[nCntParticle].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
			}
#endif


			}

			break;

		case 2:

			if (aParticle[nCntParticle].use)
			{// 使用中

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
			{// 使用中の場合のみ処理を行う

			    // パーティクルの移動
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
				// α値設定
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
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;

	PLAYER* player = GetPlayer();

	CAMERA* cam = GetCamera();

	// ライティングを無効に
	SetLightEnable(FALSE);

	// Z比較無し
	SetDepthEnable(FALSE);

	// フォグ無効
	SetFogEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &texture[texNo]);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		switch (aParticle[nCntParticle].type)
		{
		case 0:
		case 2:
		case 3:
			// 加算合成に設定
			SetBlendState(BLEND_MODE_ADD);
			break;

		case 1:
			// 減算合成に設定
			SetBlendState(BLEND_MODE_SUBTRACT);
			break;

		}


		if (aParticle[nCntParticle].use)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			//mtxWorld = XMMatrixInverse(nullptr, mtxView);
			//mtxWorld.r[3].m128_f32[0] = 0.0f;
			//mtxWorld.r[3].m128_f32[1] = 0.0f;
			//mtxWorld.r[3].m128_f32[2] = 0.0f;

			// 処理が速いしお勧め
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];

			// スケールを反映
			mtxScl = XMMatrixScaling(aParticle[nCntParticle].scale.x, aParticle[nCntParticle].scale.y, aParticle[nCntParticle].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(aParticle[nCntParticle].pos.x, aParticle[nCntParticle].pos.y, aParticle[nCntParticle].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			//mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&player[19].parent->mtxWorld));

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(aParticle[nCntParticle].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);

		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(TRUE);

	// フォグ有効
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(PARTICLE_SIZE_X  / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(PARTICLE_SIZE_X  / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(vertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	aParticle[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
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
			// 影の設定
			aParticle[nCntParticle].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
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
