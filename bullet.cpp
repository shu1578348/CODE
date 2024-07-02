//=============================================================================
//
// 弾発射処理 [bullet.cpp]
// Author : 荒山 秀磨
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
static BULLET bullet[BULLET_MAX];	 // バレット構造体
static BOOL	  hoad = FALSE;		 // 初期化を行ったかのフラグ
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
		bullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		bullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		bullet[nCntBullet].scl = { 1.0f, 1.0f, 1.0f };

		bullet[nCntBullet].spd = 0.0f;

		bullet[nCntBullet].size = BULLET_SIZE; // 当たり判定の大きさ

		bullet[nCntBullet].type = 1; // 0:通常弾 / 1:火炎弾 / 2:毒弾

		bullet[nCntBullet].use  = FALSE;	// TRUE:使用 / FALSE:未使用
		bullet[nCntBullet].shot = FALSE;	// TRUE:発射 / FALSE:未発射
	}

	//-------------------------------------------------------------------------
	// モデルの読み込み
	//-------------------------------------------------------------------------
	{
		hoad = TRUE;

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
	if (hoad)
	{
		UnloadModel(&modelBullet);
		hoad = FALSE;
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
		if (bullet[i].use)
		{		
			// 移動前の座標を保存
			XMFLOAT3 oldPos = bullet[i].pos;

			//-----------------------------------------------------------------
			// 発射後の処理
			//-----------------------------------------------------------------
			if (bullet[i].shot)
			{

				//-------------------------------------------------------------
				// 移動処理
				//-------------------------------------------------------------
				{

					// 減速処理
					bullet[i].spd *= 0.997f;

					// 弾の移動処理
					bullet[i].pos.x -= sinf(bullet[i].rot.y) * bullet[i].spd;
					bullet[i].pos.z -= cosf(bullet[i].rot.y) * bullet[i].spd;

					// 消えるまでの時間
					bullet[i].dTime -= 1;

					// 一定時間経つと消える
					if (bullet[i].dTime < 0)
					{
						bullet[i].use = FALSE;
						ReleaseShadow(bullet[i].shadowIdx);
					}

					// マップ外なら消える
					{
						float distance = GetDistance(bullet[i].pos, { 0.0f, 0.0f, 0.0f });

						if (distance > 1200)
						{
							bullet[i].use = TRUE;
							ReleaseShadow(bullet[i].shadowIdx);
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

					pos = bullet[i].pos;

					for (int j = 0; j < 3; j++)
					{

						switch (bullet[i].type)
						{
						case 0: // 通常弾						

							pos.x += sinf(bullet[i].rot.y) * j;
							pos.z += cosf(bullet[i].rot.y) * j;

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
				bullet[i].pos = XMFLOAT3(player[0].pos.x, 27.0f, player[0].pos.z);

				// 入力のあった方向へプレイヤーを向かせて移動させる
				bullet[i].pos.x -= sinf(player[0].rot.y - 0.9f) * 4.0f;
				bullet[i].pos.z -= cosf(player[0].rot.y - 0.9f) * 4.0f;

				// プレイヤーと回転を同じにする
				bullet[i].rot.y = player[0].rot.y;

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

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 5:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						scale = { 0.03f, 0.03f, 0.03f };

						nLife = rand() % 50 + 60;

						fSize = (float)(rand() % 30 + 10);

						for (int k = 0; k <= 10; k++)
						{

							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 3:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

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
							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						break;

					case 4:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

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
							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.1f, 1.0f), fSize, fSize, nLife, 1);
						}

						break;

					case 6:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

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

							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(1.0f, 0.3f, 0.0f, 1.0f), fSize, fSize, nLife, 0);
						}
						break;

					case 7:

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

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

							pos.x -= sinf(bullet[i].rot.y) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y - 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y - 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

							move = { 0.0f,0.0f,0.0f };

							nLife = 10;

							// ビルボードの設定
							SetParticle(pos, pos, move, scale, XMFLOAT4(0.2f, 0.4f, 0.0f, 1.0f), fSize, fSize, nLife, 1);
						}

						pos = bullet[i].pos;

						pos.x -= sinf(bullet[i].rot.y) * 1.5f;
						pos.z -= cosf(bullet[i].rot.y) * 1.5f;

						for (int k = 0; k <= 10; k++)
						{
							pos.x -= sinf(bullet[i].rot.y + 0.1f) * 10.0f;
							pos.z -= cosf(bullet[i].rot.y + 0.1f) * 10.0f;

							pos.y = bullet[i].pos.y - 10.0f;

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
					bullet[i].spd = BULLET_SPEED;

					// 投げる位置
					bullet[i].pos.y -= 10.0f;

					// 発射する
					bullet[i].shot = TRUE;

					// 発射方向
					switch(bullet[i].shotDir)
					{
					case 0:
						break;

					case 1:
						bullet[i].rot.y -= 0.1f;
						break;

					case 2:
						bullet[i].rot.y += 0.1f;
						break;
					}

					// アニメーションを再生する
					player[0].anim = FALSE;

					player[0].pose = 3;

				}

			}

			// 影の位置設定
			SetPositionShadow(bullet[i].shadowIdx, XMFLOAT3(bullet[i].pos.x, 0.1f, bullet[i].pos.z));

			//-------------------------------------------------------------
			// 当たり判定の処理
			//-------------------------------------------------------------
			{
				// 静的オブジェクトの当たり判定
				if (FieldHit(bullet[i].pos, oldPos))
				{
					bullet[i].use = FALSE;
					ReleaseShadow(bullet[i].shadowIdx);

					// 出血エフェクトの無効化
					if (bullet[i].type != 0)
					{
						// 接触エフェクト
						BulletEfect(bullet[i].pos, bullet[i].type);
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
						if (bullet[i].shot)
						{
							if (enemy[j].use)
							{
								if (CollisionBC(bullet[i].pos, enemy[j].pos, bullet[i].size, ENEMY_SIZE))
								{
									// 当たったら消える
									bullet[i].use = FALSE;

									// エネミーのHPを減らす
									enemy[j].hp -= 100.0f;

									ReleaseShadow(bullet[i].shadowIdx);

									// 状態異常
									switch (bullet[i].type)
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
									BulletEfect(bullet[i].pos, bullet[i].type);

								}

							}

						}

					}

					break;

				case DUNGEON_SECOND_FLOOR:
					if (bullet[i].shot)
					{
						if (boss[0].use)
						{
							if (CollisionBC(bullet[i].pos, boss[0].pos, bullet[i].size, BOSS_SIZE))
							{
								// 当たったら消える
								bullet[i].use = FALSE;

								// エネミーのHPを減らす
								boss[0].hp -= 10.0f;

								ReleaseShadow(bullet[i].shadowIdx);

								// 接触エフェクト
								BulletEfect(bullet[i].pos, bullet[i].type);

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
		if (!bullet[i].use) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(bullet[i].scl.x, bullet[i].scl.y, bullet[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(bullet[i].rot.x, bullet[i].rot.y + XM_PI, bullet[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(bullet[i].pos.x, bullet[i].pos.y, bullet[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&bullet[i].mtxWorld, mtxWorld);

		// モデル描画
		switch (bullet[i].modelNo)
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
		if (!bullet[nCntBullet].use)	// 未使用状態のバレットを見つける
		{
			bullet[nCntBullet].pos = XMFLOAT3(pos.x, 100.0f, pos.z);	// 座標をセット
			bullet[nCntBullet].rot  = rot;							// 回転をセット
			bullet[nCntBullet].shot = shot;
			bullet[nCntBullet].use = TRUE;							// 使用状態へ変更する
			

			bullet[nCntBullet].dTime = BULLET_DELETE;					// 消えるまでの時間
			bullet[nCntBullet].scl = { 0.2f, 0.2f, 0.2f };			// サイズをもとに戻す

			bullet[nCntBullet].type = type;							// 弾の種類
			bullet[nCntBullet].shotDir = shotDir;						// 弾の発射方向 0:中央 1:左前 2:右前

			// 影の設定
			bullet[nCntBullet].shadowIdx = CreateShadow(bullet[nCntBullet].pos, 0.2f, 0.2f);

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
	return &bullet[0];
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