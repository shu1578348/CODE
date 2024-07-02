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
static BOMB	bomb[BOMB_MAX];	// 爆弾の最大数
static BOOL	load = FALSE;		// モデルの初期化フラグ
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
		bomb[i].rot  = { 0.0f, 0.0f, 0.0f };
		bomb[i].scl  = { 0.8f, 0.8f, 0.8f };
		bomb[i].size = { BOMB_SIZE_X, BOMB_SIZE_Y, BOMB_SIZE_Z };

		bomb[i].modelNo = 0; // 表示モデルの種類

		bomb[i].exTime = EXPLOSION_TIME; // 爆発時間

		bomb[i].exFlag = FALSE; // TRUE:爆発 / FALSE:未爆発

		bomb[i].draw = TRUE;	  // TRUE:表示 / FALSE:非表示
		bomb[i].use  = TRUE;	  // TRUE:使用 / FALSE:未使用
	}

	//-------------------------------------------------------------------------
	// モデルの読み込み
	//-------------------------------------------------------------------------
	{	
		load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_BOMB, &modelBomb);
	}

	//-------------------------------------------------------------------------
	// 位置の設定
	//-------------------------------------------------------------------------
	{
		bomb[0].pos = { -130.0f, 1.0f,  -90.0f };
		bomb[1].pos = { -500.0f, 1.0f,  635.0f };
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBomb(void)
{
	if (load)
	{
		UnloadModel(&modelBomb);

		load = FALSE;
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
		if (bomb[i].use)
		{
			//-------------------------------------------------------------------------
			// 視錐台カリング
			//-------------------------------------------------------------------------
			{
				bomb[i].draw = FrustumCulling(bomb[i].pos, bomb[i].rot, bomb[i].size);
			}

			//-------------------------------------------------------------------------
			// バレットの当たり判定
			//-------------------------------------------------------------------------
			{
				for (int j = 0; j < BULLET_MAX; j++)
				{
					if (bullet[j].use)
					{
						if (CollisionBC(bomb[i].pos, bullet[j].pos, BOMB_SIZE, BULLET_SIZE))
						{
							bullet[j].use = FALSE;

							ReleaseShadow(bullet[j].shadowIdx);

							// ナイフ(炎)なら爆発する
							if (bullet[j].type == 1)
							{
								bomb[i].exFlag = TRUE;
							}
						}
					}
				}
			}

			//-------------------------------------------------------------------------
			// 爆発時の処理
			//-------------------------------------------------------------------------
			{
				if ((bomb[i].exFlag) && (bomb[i].use))
				{
					//-----------------------------------------------------------------
					// 爆発範囲の当たり判定
					//-----------------------------------------------------------------
					for (int k = 0; k < ENEMY_MAX; k++)
					{
						if (CollisionBC(bomb[i].pos, enemy[k].pos, EXPLOSION_RANGE, ENEMY_SIZE))
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

						pos = bomb[i].pos;

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
					bomb[i].exTime--;

				}

				//-----------------------------------------------------------------
				// 爆発の終了処理
				//-----------------------------------------------------------------
				if (bomb[i].exTime < 0)
				{
					bomb[i].exFlag = FALSE;

					bomb[i].use = FALSE;
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
		if (!bomb[i].use)  continue;

		// 非表示
		if (!bomb[i].draw) continue;

		// 非表示
		if (bomb[i].exFlag) continue;

		XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(bomb[i].scl.x, bomb[i].scl.y, bomb[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(bomb[i].rot.x, bomb[i].rot.y + XM_PI, bomb[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(bomb[i].pos.x, bomb[i].pos.y, bomb[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&bomb[i].mtxWorld, mtxWorld);

		// モデル描画
		switch (bomb[i].modelNo)
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
	return &bomb[0];
}