//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM		(0.0f)		// カメラの初期位置(X座標)
#define	POS_Y_CAM		(5.0f)		// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(38.0f)		// カメラの初期位置(Z座標)

#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						 // ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	 // ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(1.0f)											 // ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										 // ビュー平面のFarZ値

#define	CULL_ANGLE		(XMConvertToRadians(45.0f))						 // ビュー平面の視野角
#define	CULL_ASPECT		((float)16 / (float)9)							 // ビュー平面のアスペクト比	
#define	CULL_NEAR_Z		(0.1f)											 // ビュー平面のNearZ値
#define	CULL_FAR_Z		(2000.0f)										 // ビュー平面のFarZ値

struct CAMERA
{
	XMFLOAT4X4	mtxView;		// ビューマトリックス
	XMFLOAT4X4	mtxInvView;		// ビューマトリックス
	XMFLOAT4X4	mtxProjection;	// プロジェクションマトリックス

	XMFLOAT3	pos;			// カメラの視点(位置)
	XMFLOAT3	at;				// カメラの注視点
	XMFLOAT3	up;				// カメラの上方向ベクトル
	XMFLOAT3	rot;			// カメラの回転

	BOOL		state;			// TRUE:操作可能 / FALSE:操作不可能/イベント中

	float		len;			// カメラの視点と注視点の距離
	float		time;			// 時間
	int			tblNo;			// 行動データのテーブル番号
	int			tblMax;			// そのテーブルのデータ数

};

enum {
	TYPE_FULL_SCREEN,
	TYPE_LEFT_HALF_SCREEN,
	TYPE_RIGHT_HALF_SCREEN,
	TYPE_UP_HALF_SCREEN,
	TYPE_DOWN_HALF_SCREEN,
	TYPE_NONE,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);

CAMERA* GetCamera(void);

void SetViewPort(int type);
int GetViewPortType(void);

void SetCameraAT(XMFLOAT3 pos);
