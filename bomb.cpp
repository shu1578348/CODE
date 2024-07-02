//=============================================================================
//
// ボムの処理 [bomb.cpp]
// Author : 荒山秀磨
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "bomb.h"
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "collision.h"
#include "shadow.h"
#include "particle.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BOMB	"data/MODEL/bomb.obj" // 読み込むモデル名

#define BOMB_SHADOW_SIZE	(0.4f)	  // 影の大きさ
#define BOMB_OFFSET_Y		(0.0f)	  // 爆弾の下をあわせる

#define EXPLOSION_TIME		(30.0f)	  // 爆発時間
#define EXPLOSION_RANGE		(100.0f)  // 爆発範囲
#define EXPLOSION_DAMAGE	(10.0f)   // 爆発によるダメージ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BOMB	g_Bomb[BOMB_MAX];	// 爆弾の最大数
static BOOL	g_Load = FALSE;		// モデルの初期化フラグ
DX11_MODEL modelBomb;		    // モデル情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBomb(void)
{
	//-------------------------------------------------------------------------
	// 情報の初期化
	//-------------------------------------------------------------------------
	for (int i = 0; i < BOMB_MAX; i++)
	{
		g_Bomb[i].rot  = { 0.0f, 0.0f, 0.0f };
		g_Bomb[i].scl  = { 0.8f, 0.8f, 0.8f };
		g_Bomb[i].size = { BOMB_SIZE_X, BOMB_SIZE_Y, BOMB_SIZE_Z };

		g_Bomb[i].modelNo = 0; // 表示モデルの種類

		g_Bomb[i].exTime = EXPLOSION_TIME; // 爆発時間

		g_Bomb[i].exFlag = FALSE; // TRUE:爆発 / FALSE:未爆発

		g_Bomb[i].draw = TRUE;	  // TRUE:表示 / FALSE:非表示
		g_Bomb[i].use  = TRUE;	  // TRUE:使用 / FALSE:未使用
	}

	//-------------------------------------------------------------------------
	// モデルの読み込み
	//-------------------------------------------------------------------------
	{	
		g_Load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_BOMB, &modelBomb);
	}

	//-------------------------------------------------------------------------
	// 位置の設定
	//-------------------------------------------------------------------------
	{
		g_Bomb[0].pos = { -130.0f, 1.0f,  -90.0f };
		g_Bomb[1].pos = { -500.0f, 1.0f,  635.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBomb(void)
{
	if (g_Load)
	{
		UnloadModel(&modelBomb);

		g_Load = FALSE;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBomb(void)
{
	//-------------------------------------------------------------------------
	// 情報の取得
	//-------------------------------------------------------------------------
	BULLET * bullet = GetBullet(); // バレット
	PLAYER * player = GetPlayer(); // プレイヤー
	ENEMY  * enemy  = GetEnemy();  // エネミー

	for (int i = 0; i < BOMB_MAX; i++)
	{
		if (g_Bomb[i].use)
		{
			//-------------------------------------------------------------------------
			// 視錐台カリング
			//-------------------------------------------------------------------------
			{
				g_Bomb[i].draw = FrustumCulling(g_Bomb[i].pos, g_Bomb[i].rot, g_Bomb[i].size);
			}

			//-------------------------------------------------------------------------
			// バレットの当たり判定
			//-------------------------------------------------------------------------
			{
				for (int j = 0; j < BULLET_MAX; j++)
				{
					if (bullet[j].use)
					{
						if (CollisionBC(g_Bomb[i].pos, bullet[j].pos, BOMB_SIZE, BULLET_SIZE))
						{
							bullet[j].use = FALSE;

							ReleaseShadow(bullet[j].shadowIdx);

							// ナイフ(炎)なら爆発する
							if (bullet[j].type == 1)
							{
								g_Bomb[i].exFlag = TRUE;
							}
						}
					}
				}
			}

			//-------------------------------------------------------------------------
			// 爆発時の処理
			//-------------------------------------------------------------------------
			{
				if ((g_Bomb[i].exFlag) && (g_Bomb[i].use))
				{
					//-----------------------------------------------------------------
					// 爆発範囲の当たり判定
					//-----------------------------------------------------------------
					for (int k = 0; k < ENEMY_MAX; k++)
					{
						if (CollisionBC(g_Bomb[i].pos, enemy[k].pos, EXPLOSION_RANGE, ENEMY_SIZE))
						{
							// 敵が燃える
							enemy[k].condition = 1;

							enemy[k].hp -= EXPLOSION_DAMAGE;
						}
					}

					//-----------------------------------------------------------------
					// 爆発エフェクト処理
					//-----------------------------------------------------------------
					for (int l = 0; l < 30; l++)
					{
						XMFLOAT3 pos;
						XMFLOAT3 move;
						XMFLOAT3 scale;
						float fSize;
						float size;
						int nLife;

						pos = g_Bomb[i].pos;

						scale = { 1.0f, 1.0f, 1.0f };

						size = (rand() % 100 + 5.0f) * 0.01f;

						scale = { size, size, size };

						move.x = (300.0f - (rand() % 600)) * 0.015f;
						move.y = (300.0f - (rand() % 600)) * 0.015f;
						move.z = (300.0f - (rand() % 600)) * 0.015f;

						nLife = rand() % 50 + 100;

						fSize = 0.0f;

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);
					}

					//-----------------------------------------------------------------
					// 爆発時間減少
					//-----------------------------------------------------------------
					g_Bomb[i].exTime--;

				}

				//-----------------------------------------------------------------
				// 爆発の終了処理
				//-----------------------------------------------------------------
				if (g_Bomb[i].exTime < 0)
				{
					g_Bomb[i].exFlag = FALSE;

					g_Bomb[i].use = FALSE;
				}

			}

		}

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBomb(void)
{

	for (int i = 0; i < BOMB_MAX; i++)
	{
		// 未使用
		if (!g_Bomb[i].use)  continue;

		// 非表示
		if (!g_Bomb[i].draw) continue;

		// 非表示
		if (g_Bomb[i].exFlag) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Bomb[i].scl.x, g_Bomb[i].scl.y, g_Bomb[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Bomb[i].rot.x, g_Bomb[i].rot.y + XM_PI, g_Bomb[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Bomb[i].pos.x, g_Bomb[i].pos.y, g_Bomb[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Bomb[i].mtxWorld, mtxWorld);

		// モデル描画
		switch (g_Bomb[i].modelNo)
		{
		case 0:
			DrawModel(&modelBomb);
			break;
		}

	}

}

//=============================================================================
// 爆弾の情報を取得
//=============================================================================
BOMB* GetBomb(void)
{
	return &g_Bomb[0];
}