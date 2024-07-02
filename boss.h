//=============================================================================
//
// エネミーモデル処理 [boss.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_MAX		(1)			  // エネミーの数
#define	BOSS_SIZE_X		(120.0f)	  // 当たり判定の大きさ
#define	BOSS_SIZE_Y		(200.0f)
#define	BOSS_SIZE_Z		(200.0f)
#define BOSS_SIZE		(50.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BOSS
{
	XMFLOAT4X4	mtxWorld;	// ワールドマトリックス
	XMFLOAT3	pos;		// モデルの位置
	XMFLOAT3	tPos;		// ターゲットのポジション
	XMFLOAT3	rot;		// モデルの向き(回転)
	XMFLOAT3    aRot;		// 攻撃方向
	XMFLOAT3	scl;		// モデルの大きさ(スケール)
	XMFLOAT3	size;		// モデルの当たり判定

	BOOL		use;		// TRUE:使用 / FALSE:未使用
	BOOL		draw;		// TRUE:表示 / FALSE:非表示
	BOOL		state;		// TRUE:移動可能 / FALSE:移動不可能
	BOOL		chase;		// TRUE:追跡中 / FALSE:非追跡中

	float		spd;		// 移動スピード
	float		hp;			// ヒットポイント
	int			condition;	// 状態異常 ( 0:通常 / 1:炎上 / 2:毒 )
	int			mode;		// 攻撃モード
	int			modelNo;

	float		time;		// 時間
	int			tblNo;		// 行動データのテーブル番号
	int			tblMax;		// そのテーブルのデータ数
	int			shadowIdx;	// 影のインデックス番号
};

struct BEZIER
{
	XMFLOAT3 start;
	XMFLOAT3 p1;
	XMFLOAT3 end;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

BOSS* GetBoss(void);

XMFLOAT3 CalculateBezierPoint(const XMFLOAT3& start, const XMFLOAT3& p1, const XMFLOAT3& end, float t);