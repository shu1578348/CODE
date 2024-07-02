//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_MAX		(7)		  // エネミーの数
#define	ENEMY_SIZE_X	(10.0f)	  // 当たり判定の大きさ
#define	ENEMY_SIZE_Y	(10.0f)
#define	ENEMY_SIZE_Z	(20.0f)
#define	ENEMY_SIZE		(10.0f)	  // 当たり判定用(BC用)
#define ENEMY_RANGE		(100.0f)  // エネミーの追尾までの距離

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// モデルの位置
	XMFLOAT3	tPos;		// ターゲットのポジション
	XMFLOAT3	rot;		// モデルの向き(回転)
	XMFLOAT3	scl;		// モデルの大きさ(スケール)
	XMFLOAT3	size;		// 当たり判定の大きさ

	BOOL		use;		// TRUE:使用 / FALSE:未使用
	BOOL		draw;		// TRUE:表示 / FALSE:非表示
	BOOL		state;		// TRUE:移動可能 / FALSE:移動不可能
	BOOL		chase;		// TRUE:追跡中 / FALSE:非追跡中

	float		spd;		// 移動スピード
	float		hp;			// ヒットポイント
	int			condition;	// 状態異常 ( 0:通常 / 1:炎上 / 2:毒 )
	int			modelNo;

	float		time;		// 時間
	int			tblNo;		// 行動データのテーブル番号
	int			tblMax;		// そのテーブルのデータ数
	int			shadowIdx;	// 影のインデックス番号
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY* GetEnemy(void);