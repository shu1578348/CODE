//=============================================================================
//
// 弾発射処理 [enemybullet.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "shadow.h"
#include "model.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "barrel.h"
#include "house.h"
#include "collision.h"
#include "score.h"
#include "particle.h"
#include "game.h"
#include "enemybullet.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMYBULLET g_EnemyBullet[ENEMY_BULLET_MAX];	 // エネミーバレット構造体
static BOOL	  g_Load = FALSE;						 // 初期化を行ったかのフラグ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemyBullet(void)
{
	//-------------------------------------------------------------------------
	// 情報の初期化
	//-------------------------------------------------------------------------
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		g_EnemyBullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		g_EnemyBullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		g_EnemyBullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };

		g_EnemyBullet[nCntBullet].spd = 0.0f;

		g_EnemyBullet[nCntBullet].size = BULLET_SIZE; // 当たり判定の大きさ

		g_EnemyBullet[nCntBullet].type = 1; // 0:通常弾 / 1:火炎弾 / 2:毒弾

		g_EnemyBullet[nCntBullet].use  = FALSE;	// TRUE:使用 / FALSE:未使用
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemyBullet(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemyBullet(void)
{
	//-------------------------------------------------------------------------
	// OBJ情報の取得
	//-------------------------------------------------------------------------
	PLAYER* player = GetPlayer(); // プレイヤー

	XMFLOAT3 pos;
	XMFLOAT3 oldPos;
	XMFLOAT3 move;
	XMFLOAT3 scale;
	float fAngle, fLength;
	float fSize;
	int nLife;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_EnemyBullet[i].use)
		{

			//-------------------------------------------------------------------------
			// 移動処理
			//-------------------------------------------------------------------------
			{

				// 移動前の座標を保存
				oldPos = g_EnemyBullet[i].pos;
				
				// 弾の移動処理
				g_EnemyBullet[i].pos.x -= sinf(g_EnemyBullet[i].rot.y) * g_EnemyBullet[i].spd;
				g_EnemyBullet[i].pos.z -= cosf(g_EnemyBullet[i].rot.y) * g_EnemyBullet[i].spd;

				// 減速処理
				g_EnemyBullet[i].spd *= 0.997f;

				g_EnemyBullet[i].dTime -= 1;

				// 一定時間経つと消える
				if (g_EnemyBullet[i].dTime < 0)
				{
					g_EnemyBullet[i].use = FALSE;
					ReleaseShadow(g_EnemyBullet[i].shadowIdx);
				}

			}

			//-------------------------------------------------------------------------
			// エフェクト処理
			//-------------------------------------------------------------------------
			{

				pos = g_EnemyBullet[i].pos;

				scale = { 0.3f, 0.3f, 0.3f };

				fAngle = (float)(rand() % 628 - 314) / 20;
				fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 8;

				move.x = sinf(fAngle) * fLength;
				move.y = (rand() % 300 / 100.0f + 10) / 100;
				move.z = cosf(fAngle) * fLength;

				nLife = rand() % 80 + 50;

				fSize = (float)(rand() % 30 + 10);

				// ビルボードの設定
				SetParticle(g_EnemyBullet[i].pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);

			}


			//-------------------------------------------------------------------------
			// プレイヤーとの当たり判定処理
			//-------------------------------------------------------------------------
			{
				if (player[0].use)
				{

					if (CollisionBC(player[0].pos, g_EnemyBullet[i].pos, player[0].size, g_EnemyBullet[i].size))
					{
						// 当たったら消える
						g_EnemyBullet[i].use = FALSE;

						// エネミーのHPを減らす
						player[0].hp -= 5;

						ReleaseShadow(g_EnemyBullet[i].shadowIdx);

						// 接触エフェクト
						for (int j = 0; j < 20; j++)
						{
							pos = g_EnemyBullet[i].pos;

							scale = { 0.3f, 0.3f, 0.3f };

							fAngle = (float)(rand() % 628 - 314) / 20;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 5;

							move.x = sinf(fAngle) * fLength;
							move.y = (100.0f - rand() % 200) * 0.01f;
							move.z = cosf(fAngle) * fLength;

							move.x -= sinf(g_EnemyBullet[i].rot.y);
							move.z -= cosf(g_EnemyBullet[i].rot.y);

							nLife = rand() % 80 + 50;

							fSize = (float)(rand() % 30 + 10);

							// ビルボードの設定
							SetParticle(g_EnemyBullet[i].pos, pos, move, scale, XMFLOAT4(0.0f, 0.3f, 1.0f, 0.85f), fSize, fSize, nLife, 0);
						}

					}

				}

			}

			if (FieldHit(g_EnemyBullet[i].pos, oldPos))
			{
				g_EnemyBullet[i].use = FALSE;

			}

		}

	}

	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemyBullet(void)
{

}

//=============================================================================
// バレットの発射設定
//=============================================================================
void SetEnemyBullet(XMFLOAT3 pos, XMFLOAT3 rot,int type)
{
	int nIdxBullet = -1;

	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (!g_EnemyBullet[nCntBullet].use)	// 未使用状態のバレットを見つける
		{
			float len;

			len = (20.0f - rand() % 40) * 0.02f;

			g_EnemyBullet[nCntBullet].spd = ENEMY_BULLET_SPEED;

			g_EnemyBullet[nCntBullet].pos = XMFLOAT3(pos.x, 20.0f, pos.z);	// 座標をセット
			g_EnemyBullet[nCntBullet].rot  = rot;							// 回転をセット
			g_EnemyBullet[nCntBullet].rot.y += len;							// 回転をセット
			g_EnemyBullet[nCntBullet].use = TRUE;							// 使用状態へ変更する	

			g_EnemyBullet[nCntBullet].dTime = ENEMY_BULLET_DELETE;			// 消えるまでの時間
			g_EnemyBullet[nCntBullet].scl = { 0.2f, 0.2f, 0.2f };			// サイズをもとに戻す

			g_EnemyBullet[nCntBullet].type = type;							// 弾の種類

			// 影の設定
			g_EnemyBullet[nCntBullet].shadowIdx = CreateShadow(g_EnemyBullet[nCntBullet].pos, 0.2f, 0.2f);

			nIdxBullet = nCntBullet;

			break;
		}
	}

}

//=============================================================================
// バレット構造体の先頭アドレスを取得
//=============================================================================
ENEMYBULLET* GetEnemyBullet(void)
{
	return &g_EnemyBullet[0];
}

//=============================================================================
// バレットの接触時エフェクト
//=============================================================================
void EnemyBulletEfect(XMFLOAT3 pos, int type)
{

}