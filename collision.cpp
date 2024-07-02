//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 荒山 秀磨
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "model.h"
#include "player.h"


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


//=============================================================================
// BBによる当たり判定処理
// Y軸の回転のみ対応させる
// 戻り値：当たってたらTRUE
//=============================================================================
BOOL CollisionBB(XMFLOAT3 mpos, float mw, float mh,
	XMFLOAT3 mrotation,  // モデルの回転角度（X, Y, Z軸）
	XMFLOAT3 ypos, float zw, float zh,
	XMFLOAT3 yrotation)  // ターゲットの回転角度（X, Y, Z軸）
{
	BOOL ans = FALSE;    // 外れをセットしておく

	XMFLOAT3 rotatedMPos;
	XMFLOAT3 rotatedYPos;

	// 座標が中心点なので計算しやすく半分にしている
	mw /= 2;
	mh /= 2;
	zw /= 2;
	zh /= 2;

	// ラジアンに変換
	float mradY = XMConvertToRadians(mrotation.y);
	float yradY = XMConvertToRadians(yrotation.y);

	// モデルの座標をy軸回転させる
	rotatedMPos.x = mpos.x * cosf(mradY) - mpos.z * sinf(mradY);
	rotatedMPos.z = mpos.x * sinf(mradY) + mpos.z * cosf(mradY);

	// ターゲットの座標をy軸回転させる
	rotatedYPos.x = ypos.x * cosf(yradY) - ypos.z * sinf(yradY);
	rotatedYPos.z = ypos.x * sinf(yradY) + ypos.z * cosf(yradY);

	// 回転後のバウンディングボックス(BB)の処理
	if ((rotatedMPos.x + mw > rotatedYPos.x - zw) &&
		(rotatedMPos.x - mw < rotatedYPos.x + zw) &&
		(mpos.y + mh > ypos.y - zh) &&
		(mpos.y - mh < ypos.y + zh) &&
		(rotatedMPos.z + mw > rotatedYPos.z - zw) &&
		(rotatedMPos.z - mw < rotatedYPos.z + zw))
	{
		// 当たった時の処理
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらTRUE
//=============================================================================
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2)
{
	BOOL ans = FALSE;						// 外れをセットしておく

	float len = (r1 + r2) * (r1 + r2);		// 半径を2乗した物
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2点間の距離（2乗した物）
	float lenSq = 0.0f;
	XMStoreFloat(&lenSq, temp);

	// 半径を2乗した物より距離が短い？
	if (len > lenSq)
	{
		ans = TRUE;	// 当たっている
	}

	return ans;
}

//=============================================================================
// 内積(dot)
//=============================================================================
float dotProduct(XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// ダイレクトＸでは、、、
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
#endif

	return(ans);
}

//=============================================================================
// 外積(cross)
//=============================================================================
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// ダイレクトＸでは、、、
	* ret = XMVector3Cross(*v1, *v2);
#endif

}

//=============================================================================
// レイキャスト
// p0, p1, p2　ポリゴンの３頂点
// pos0 始点
// pos1 終点
// hit　交点の返却用
// normal 法線ベクトルの返却用
// 当たっている場合、TRUEを返す
//=============================================================================
BOOL RayCast(XMFLOAT3 xp0, XMFLOAT3 xp1, XMFLOAT3 xp2, XMFLOAT3 xpos0, XMFLOAT3 xpos1, XMFLOAT3* hit, XMFLOAT3* normal)
{
	XMVECTOR	p0 = XMLoadFloat3(&xp0);
	XMVECTOR	p1 = XMLoadFloat3(&xp1);
	XMVECTOR	p2 = XMLoadFloat3(&xp2);
	XMVECTOR	pos0 = XMLoadFloat3(&xpos0);
	XMVECTOR	pos1 = XMLoadFloat3(&xpos1);

	XMVECTOR	nor;	// ポリゴンの法線
	XMVECTOR	vec1;
	XMVECTOR	vec2;
	float		d1, d2;

	{	// ポリゴンの外積をとって法線を求める(この処理は固定物なら予めInit()で行っておくと良い)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		nor = XMVector3Normalize(nor);		// 計算しやすいように法線をノーマライズしておく(このベクトルの長さを１にしている)
		XMStoreFloat3(normal, nor);			// 求めた法線を入れておく
	}

	// ポリゴン平面と線分の内積とって衝突している可能性を調べる（鋭角なら＋、鈍角ならー、直角なら０）
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// 求めたポリゴンの法線と２つのベクトル（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if (((d1 * d2) > 0.0f) || (d1 == 0 && d2 == 0))
		{
			// 当たっている可能性は無い
			return(FALSE);
		}
	}


	{	// ポリゴンと線分の交点を求める
		d1 = (float)fabs(d1);	// 絶対値を求めている
		d2 = (float)fabs(d2);	// 絶対値を求めている
		float a = d1 / (d1 + d2);							// 内分比

		XMVECTOR	vec3 = (1 - a) * vec1 + a * vec2;		// p0から交点へのベクトル
		XMVECTOR	p3 = p0 + vec3;							// 交点
		XMStoreFloat3(hit, p3);								// 求めた交点を入れておく

		{	// 求めた交点がポリゴンの中にあるか調べる

			// ポリゴンの各辺のベクトル
			XMVECTOR	v1 = p1 - p0;
			XMVECTOR	v2 = p2 - p1;
			XMVECTOR	v3 = p0 - p2;

			// 各頂点と交点とのベクトル
			XMVECTOR	v4 = p3 - p1;
			XMVECTOR	v5 = p3 - p2;
			XMVECTOR	v6 = p3 - p0;

			// 外積で各辺の法線を求めて、ポリゴンの法線との内積をとって符号をチェックする
			XMVECTOR	n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(FALSE);	// 当たっていない

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(FALSE);	// 当たっていない

			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(FALSE);	// 当たっていない
		}
	}

	return(TRUE);	// 当たっている！(hitには当たっている交点が入っている。normalには法線が入っている)
}
//=============================================================================
//ボックスによる当たり判定
//=============================================================================
BOOL BoxCollider(XMFLOAT3 size, XMFLOAT3 pos, float rotationY, XMFLOAT3 rayStart, XMFLOAT3 rayEnd, XMFLOAT3* hit, XMFLOAT3* normal)
{
	// Y軸回転の行列を生成
	XMMATRIX rotationMatrix = XMMatrixRotationY(rotationY);

	// ボックスのローカル座標系での頂点定義
	XMFLOAT3 localVertices[8] = {
		{ -size.x / 2, -size.y / 2, -size.z / 2 }, // 左下奥
		{  size.x / 2, -size.y / 2, -size.z / 2 }, // 右下奥
		{ -size.x / 2,  size.y / 2, -size.z / 2 }, // 左上奥
		{  size.x / 2,  size.y / 2, -size.z / 2 }, // 右上奥
		{ -size.x / 2, -size.y / 2,  size.z / 2 }, // 左下前
		{  size.x / 2, -size.y / 2,  size.z / 2 }, // 右下前
		{ -size.x / 2,  size.y / 2,  size.z / 2 }, // 左上前
		{  size.x / 2,  size.y / 2,  size.z / 2 }  // 右上前
	};

	// 回転を適用してワールド座標系での頂点を計算
	XMFLOAT3 worldVertices[8];
	for (int i = 0; i < 8; ++i) {
		XMVECTOR localVertex = XMLoadFloat3(&localVertices[i]);
		XMVECTOR worldVertex = XMVector3TransformCoord(localVertex, rotationMatrix);
		worldVertex = XMVectorAdd(worldVertex, XMLoadFloat3(&pos));
		XMStoreFloat3(&worldVertices[i], worldVertex); // XMVECTORからXMFLOAT3へ変換
	}

	// ボックスの各面を定義するポリゴンの頂点インデックス
	int indices[36] = {
		0, 1, 2, 2, 1, 3, // 後ろ面
		1, 5, 3, 3, 5, 7, // 右面
		5, 4, 7, 7, 4, 6, // 前面
		4, 0, 6, 6, 0, 2, // 左面
		2, 3, 6, 6, 3, 7, // 上面
		4, 5, 0, 0, 5, 1  // 下面
	};

	// 各面に対してレイキャストを行い、交点があるかどうかをチェック
	for (int i = 0; i < 12; ++i) {
		XMFLOAT3 p0 = worldVertices[indices[i * 3]];
		XMFLOAT3 p1 = worldVertices[indices[i * 3 + 1]];
		XMFLOAT3 p2 = worldVertices[indices[i * 3 + 2]];

		if (RayCast(p0, p1, p2, rayStart, rayEnd, hit, normal)) 
		{
			return TRUE; // 当たりあり
		}
	}

	return FALSE; // 当たりなし
}
