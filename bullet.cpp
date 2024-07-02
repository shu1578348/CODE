//=============================================================================
//
// 弾発射処理 [bullet.cpp]
// Author : 荒山秀磨
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
#include "boss.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BULLET "data/MODEL/dagger.obj"	 // 読み込むモデル名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BULLET g_Bullet[BULLET_MAX];	 // バレット構造体
static BOOL	  g_Load = FALSE;		 // 初期化を行ったかのフラグ
DX11_MODEL	  modelBullet;			 // モデル情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	//-------------------------------------------------------------------------
	// 情報の初期化
	//-------------------------------------------------------------------------
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		g_Bullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		g_Bullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		g_Bullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };

		g_Bullet[nCntBullet].spd = 0.0f;

		g_Bullet[nCntBullet].size = BULLET_SIZE; // 当たり判定の大きさ

		g_Bullet[nCntBullet].type = 1; // 0:通常弾 / 1:火炎弾 / 2:毒弾

		g_Bullet[nCntBullet].use  = FALSE;	// TRUE:使用 / FALSE:未使用
		g_Bullet[nCntBullet].shot = FALSE;	// TRUE:発射 / FALSE:未発射
	}

	//-------------------------------------------------------------------------
	// モデルの読み込み
	//-------------------------------------------------------------------------
	{
		g_Load = TRUE;

		// モデルの読み込み
		LoadModel(MODEL_BULLET, &modelBullet);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	if (g_Load)
	{
		UnloadModel(&modelBullet);
		g_Load = FALSE;
	}	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	//-------------------------------------------------------------------------
	// OBJ情報の取得
	//-------------------------------------------------------------------------
	PLAYER * player = GetPlayer();  // プレイヤー
	ENEMY  * enemy  = GetEnemy();	// エネミー
	HOUSE  * house  = GetHouse();   // 家
	BARREL * barrel = GetBarrel();  // 樽
	BOSS   * boss   = GetBoss();	// ボス	

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use)
		{		
			// 移動前の座標を保存
			XMFLOAT3 oldPos = g_Bullet[i].pos;

			//-----------------------------------------------------------------
			// 発射後の処理
			//-----------------------------------------------------------------
			if (g_Bullet[i].shot)
			{

				//-------------------------------------------------------------
				// 移動処理
				//-------------------------------------------------------------
				{

					// 減速処理
					g_Bullet[i].spd *= 0.997f;

					// 弾の移動処理
					g_Bullet[i].pos.x -= sinf(g_Bullet[i].rot.y) * g_Bullet[i].spd;
					g_Bullet[i].pos.z -= cosf(g_Bullet[i].rot.y) * g_Bullet[i].spd;

					// 消えるまでの時間
					g_Bullet[i].dTime -= 1;

					// 一定時間経つと消える
					if (g_Bullet[i].dTime < 0)
					{
						g_Bullet[i].use = FALSE;
						ReleaseShadow(g_Bullet[i].shadowIdx);
					}

					// マップ外なら消える
					{
						float distance = GetDistance(g_Bullet[i].pos, { 0.0f, 0.0f, 0.0f });

						if (distance > 1200)
						{
							g_Bullet[i].use = TRUE;
							ReleaseShadow(g_Bullet[i].shadowIdx);
						}
					}
				}

				//-------------------------------------------------------------
				// エフェクト処理(パーティクル)
				//-------------------------------------------------------------
				{
					XMFLOAT3 pos;
					XMFLOAT3 move;
					XMFLOAT3 scale;
					float size;
					float fAngle, fLength;
					float fSize;
					int nLife;

					pos = g_Bullet[i].pos;

					for (int j = 0; j < 3; j++)
					{

						switch (g_Bullet[i].type)
						{
						case 0: // 通常弾						

							pos.x += sinf(g_Bullet[i].rot.y) * j;
							pos.z += cosf(g_Bullet[i].rot.y) * j;

							scale = { 0.04f, 0.04f, 0.04f };

							move.x = 0.0f;
							move.y = 0.0f;
							move.z = 0.0f;

							nLife = 200;

							fSize = 0.0f;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f), fSize, fSize, nLife, 0);

							break;

						case 1: // 火炎弾

							scale = { 0.07f, 0.07f, 0.07f };

							fAngle = (float)(rand() % 628 - 314) * 0.01f;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.025f;

							move.x = sinf(fAngle) * fLength;
							move.y = 0.0f;
							move.z = cosf(fAngle) * fLength;

							nLife = rand() % 50 + 60;

							fSize = 0.0f;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

							scale = { 0.03f, 0.03f, 0.03f };

							fAngle = (float)(rand() % 628 - 314) * 0.05f;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.05f;

							move.x = sinf(fAngle) * fLength;
							move.y = 0.0f;
							move.z = cosf(fAngle) * fLength;

							nLife = rand() % 50 + 60;

							fSize = 0.0f;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);

							break;

						case 2: // 毒弾

							size = (rand() % 100 + 5.0f) * 0.0013f;

							scale = { 0.09f, 0.09f, 0.09f };

							scale.x = size;
							scale.y = size;
							scale.z = size;

							fAngle = (float)(rand() % 628 - 314) * 0.005f;
							fLength = (rand() % (int)(10 * 200) / 100.0f - 10) * 0.0125f;

							move.x = sinf(fAngle) * fLength;
							move.y = 0.0f;
							move.z = cosf(fAngle) * fLength;

							nLife = rand() % 50 + 60;

							fSize = (float)(rand() % 30 + 10);

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);

							break;

						}

					}

				}

			}
			//-------------------------------------------------------------
			// 未発射時の処理
			//-------------------------------------------------------------
			else
			{
				// プレイヤーの向きに位置を合わせる
				g_Bullet[i].pos = XMFLOAT3(player[0].pos.x, 27.0f, player[0].pos.z);

				// 入力のあった方向へプレイヤーを向かせて移動させる
				g_Bullet[i].pos.x -= sinf(player[0].rot.y - 0.9f) * 4.0f;
				g_Bullet[i].pos.z -= cosf(player[0].rot.y - 0.9f) * 4.0f;

				// プレイヤーと回転を同じにする
				g_Bullet[i].rot.y = player[0].rot.y;

				// 発射待機状態のパーティクル
				{
					XMFLOAT3 pos;
					XMFLOAT3 move;
					XMFLOAT3 scale;
					float fAngle, fLength;
					float fSize;
					int nLife;

					switch (player[0].tool)
					{
					case 0:
					case 1:
					case 2:

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 5:

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(g_Bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 3:

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.1f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 4:

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						scale = { 0.06f, 0.06f, 0.06f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.08f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.6f), fSize, fSize, nLife, 1);

						scale = { 0.03f, 0.03f, 0.03f };

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.1f, 1.0f), fSize, fSize, nLife, 1);
						}

						break;

					case 6:

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						scale = { 0.06f, 0.06f, 0.06f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.15f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);

						scale = { 0.03f, 0.03f, 0.03f };

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(g_Bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}
						break;

					case 7:

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						scale = { 0.09f, 0.09f, 0.09f };

						fAngle = (float)(rand() % 628 - 314) / 300;
						fLength = (rand() % (int)(10 * 200) / 100.0f - 10) / 150;

						move.x = sinf(fAngle) * fLength;
						move.y = 0.2f;
						move.z = cosf(fAngle) * fLength;

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						// ビルボードの設定
						SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.6f), fSize, fSize, nLife, 1);

						scale = { 0.03f, 0.03f, 0.03f };

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(g_Bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						pos = g_Bullet[i].pos;

						pos.x -= sinf(g_Bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(g_Bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(g_Bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(g_Bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = g_Bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						break;

					}

				}

				// 発射するかどうか
				if (GetKeyboardRelease(DIK_SPACE) || IsButtonReleased(1,BUTTON_B))
				{
					// 弾速
					g_Bullet[i].spd = BULLET_SPEED;

					// 投げる位置
					g_Bullet[i].pos.y -= 10.0f;

					// 発射する
					g_Bullet[i].shot = TRUE;

					// 発射方向
					switch(g_Bullet[i].shotDir)
					{
					case 0:
						break;

					case 1:
						g_Bullet[i].rot.y -= 0.1f;
						break;

					case 2:
						g_Bullet[i].rot.y += 0.1f;
						break;
					}

					// アニメーションを再生する
					player[0].anim = FALSE;

					player[0].pose = 3;

				}

			}

			// 影の位置設定
			SetPositionShadow(g_Bullet[i].shadowIdx, XMFLOAT3(g_Bullet[i].pos.x, 0.1f, g_Bullet[i].pos.z));

			//-------------------------------------------------------------
			// 当たり判定の処理
			//-------------------------------------------------------------
			{
				// 静的オブジェクトの当たり判定
				if (FieldHit(g_Bullet[i].pos, oldPos))
				{
					g_Bullet[i].use = FALSE;
					ReleaseShadow(g_Bullet[i].shadowIdx);

					// 出血エフェクトの無効化
					if (g_Bullet[i].type != 0)
					{
						// 接触エフェクト
						BulletEfect(g_Bullet[i].pos, g_Bullet[i].type);
					}
				}

				switch (GetField())
				{
				case VILLAGE:
						break;

				case DUNGEON_FIRST_FLOOR:
				//---------------------------------------------------------
				// エネミーとの当たり判定
				//---------------------------------------------------------
					for (int j = 0; j < ENEMY_MAX; j++)
					{
						if (g_Bullet[i].shot)
						{
							if (enemy[j].use)
							{
								if (CollisionBC(g_Bullet[i].pos, enemy[j].pos, g_Bullet[i].size, ENEMY_SIZE))
								{
									// 当たったら消える
									g_Bullet[i].use = FALSE;

									// エネミーのHPを減らす
									enemy[j].hp -= 100.0f;

									ReleaseShadow(g_Bullet[i].shadowIdx);

									// 状態異常
									switch (g_Bullet[i].type)
									{
									case 0: // 通常弾
										break;

									case 1: // 火炎弾
										enemy[j].condition = 1;
										break;

									case 2: // 毒弾
										enemy[j].condition = 2;
										break;
									}

									// 接触エフェクト
									BulletEfect(g_Bullet[i].pos, g_Bullet[i].type);

								}

							}

						}

					}

					break;

				case DUNGEON_SECOND_FLOOR:
					if (g_Bullet[i].shot)
					{
						if (boss[0].use)
						{
							if (CollisionBC(g_Bullet[i].pos, boss[0].pos, g_Bullet[i].size, BOSS_SIZE))
							{
								// 当たったら消える
								g_Bullet[i].use = FALSE;

								// エネミーのHPを減らす
								boss[0].hp -= 10.0f;

								ReleaseShadow(g_Bullet[i].shadowIdx);

								// 接触エフェクト
								BulletEfect(g_Bullet[i].pos, g_Bullet[i].type);

							}

						}

					}
							
					break;

				}
				
			}

		}

	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		// TRUE:使用 / FALSE:未使用
		if (!g_Bullet[i].use) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Bullet[i].scl.x, g_Bullet[i].scl.y, g_Bullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Bullet[i].rot.x, g_Bullet[i].rot.y + XM_PI, g_Bullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Bullet[i].pos.x, g_Bullet[i].pos.y, g_Bullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Bullet[i].mtxWorld, mtxWorld);

		// モデル描画
		switch (g_Bullet[i].modelNo)
		{
		case 0:
			DrawModel(&modelBullet);
			break;
		}

	}

}

//=============================================================================
// バレットの発射設定
//=============================================================================
void SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, BOOL shot, int type, int shotDir)
{
	int nIdxBullet = -1;

	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int nCntBullet = 0; nCntBullet < BULLET_MAX; nCntBullet++)
	{
		if (!g_Bullet[nCntBullet].use)	// 未使用状態のバレットを見つける
		{
			g_Bullet[nCntBullet].pos = XMFLOAT3(pos.x, 100.0f, pos.z);	// 座標をセット
			g_Bullet[nCntBullet].rot  = rot;							// 回転をセット
			g_Bullet[nCntBullet].shot = shot;
			g_Bullet[nCntBullet].use = TRUE;							// 使用状態へ変更する
			

			g_Bullet[nCntBullet].dTime = BULLET_DELETE;					// 消えるまでの時間
			g_Bullet[nCntBullet].scl = { 0.2f, 0.2f, 0.2f };			// サイズをもとに戻す

			g_Bullet[nCntBullet].type = type;							// 弾の種類
			g_Bullet[nCntBullet].shotDir = shotDir;						// 弾の発射方向 0:中央 1:左前 2:右前

			// 影の設定
			g_Bullet[nCntBullet].shadowIdx = CreateShadow(g_Bullet[nCntBullet].pos, 0.2f, 0.2f);

			nIdxBullet = nCntBullet;

			break;
		}
	}

}

//=============================================================================
// バレット構造体の先頭アドレスを取得
//=============================================================================
BULLET* GetBullet(void)
{
	return &g_Bullet[0];
}

//=============================================================================
// バレットの接触時エフェクト
//=============================================================================
void BulletEfect(XMFLOAT3 pos, int type)
{

	XMFLOAT3 move;
	XMFLOAT3 scale;
	float fSize;
	int nLife;

	for (int k = 0; k < 250; k++)
	{
		switch (type)
		{
		case 0: // 通常弾

			scale = { 0.08f, 0.08f, 0.08f };

			move.x = (300.0f - (rand() % 600)) / 1500;
			move.y = (300.0f - (rand() % 600)) / 1500;
			move.z = (300.0f - (rand() % 600)) / 1500;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 20);

			// ビルボードの設定
			SetParticle(pos, pos, move, scale, XMFLOAT4(0.1f, 1.0f, 0.3f, 0.6f), fSize, fSize, nLife, 1);
			break;

		case 1: // 炎エフェクト

			scale = { 0.2f, 0.2f, 0.2f };

			move.x = (300.0f - (rand() % 600)) / 800;
			move.y = (300.0f - (rand() % 600)) / 800;
			move.z = (300.0f - (rand() % 600)) / 800;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 20);

			// ビルボードの設定
			SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 0.85f), fSize, fSize, nLife, 0);
			break;

		case 2: // 毒エフェクト

			scale = { 0.3f, 0.3f, 0.3f };

			move.x = (300.0f - (rand() % 600)) / 500;
			move.y = (300.0f - (rand() % 600)) / 500;
			move.z = (300.0f - (rand() % 600)) / 500;

			nLife = rand() % 50 + 60;

			fSize = (float)(rand() % 30 + 20);

			// ビルボードの設定
			SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 0.6f), fSize, fSize, nLife, 1);
			break;
		}

	}
}