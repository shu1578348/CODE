//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : �r�R �G��
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "model.h"
#include "player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// BB�ɂ�铖���蔻�菈��
// Y���̉�]�̂ݑΉ�������
// �߂�l�F�������Ă���TRUE
//=============================================================================
BOOL CollisionBB(XMFLOAT3 mpos, float mw, float mh,
	XMFLOAT3 mrotation,  // ���f���̉�]�p�x�iX, Y, Z���j
	XMFLOAT3 ypos, float zw, float zh,
	XMFLOAT3 yrotation)  // �^�[�Q�b�g�̉�]�p�x�iX, Y, Z���j
{
	BOOL ans = FALSE;    // �O����Z�b�g���Ă���

	XMFLOAT3 rotatedMPos;
	XMFLOAT3 rotatedYPos;

	// ���W�����S�_�Ȃ̂Ōv�Z���₷�������ɂ��Ă���
	mw /= 2;
	mh /= 2;
	zw /= 2;
	zh /= 2;

	// ���W�A���ɕϊ�
	float mradY = XMConvertToRadians(mrotation.y);
	float yradY = XMConvertToRadians(yrotation.y);

	// ���f���̍��W��y����]������
	rotatedMPos.x = mpos.x * cosf(mradY) - mpos.z * sinf(mradY);
	rotatedMPos.z = mpos.x * sinf(mradY) + mpos.z * cosf(mradY);

	// �^�[�Q�b�g�̍��W��y����]������
	rotatedYPos.x = ypos.x * cosf(yradY) - ypos.z * sinf(yradY);
	rotatedYPos.z = ypos.x * sinf(yradY) + ypos.z * cosf(yradY);

	// ��]��̃o�E���f�B���O�{�b�N�X(BB)�̏���
	if ((rotatedMPos.x + mw > rotatedYPos.x - zw) &&
		(rotatedMPos.x - mw < rotatedYPos.x + zw) &&
		(mpos.y + mh > ypos.y - zh) &&
		(mpos.y - mh < ypos.y + zh) &&
		(rotatedMPos.z + mw > rotatedYPos.z - zw) &&
		(rotatedMPos.z - mw < rotatedYPos.z + zw))
	{
		// �����������̏���
		ans = TRUE;
	}

	return ans;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���TRUE
//=============================================================================
BOOL CollisionBC(XMFLOAT3 pos1, XMFLOAT3 pos2, float r1, float r2)
{
	BOOL ans = FALSE;						// �O����Z�b�g���Ă���

	float len = (r1 + r2) * (r1 + r2);		// ���a��2�悵����
	XMVECTOR temp = XMLoadFloat3(&pos1) - XMLoadFloat3(&pos2);
	temp = XMVector3LengthSq(temp);			// 2�_�Ԃ̋����i2�悵�����j
	float lenSq = 0.0f;
	XMStoreFloat(&lenSq, temp);

	// ���a��2�悵������苗�����Z���H
	if (len > lenSq)
	{
		ans = TRUE;	// �������Ă���
	}

	return ans;
}

//=============================================================================
// ����(dot)
//=============================================================================
float dotProduct(XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	float ans = v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
#else
	// �_�C���N�g�w�ł́A�A�A
	XMVECTOR temp = XMVector3Dot(*v1, *v2);
	float ans = 0.0f;
	XMStoreFloat(&ans, temp);
#endif

	return(ans);
}

//=============================================================================
// �O��(cross)
//=============================================================================
void crossProduct(XMVECTOR* ret, XMVECTOR* v1, XMVECTOR* v2)
{
#if 0
	ret->x = v1->y * v2->z - v1->z * v2->y;
	ret->y = v1->z * v2->x - v1->x * v2->z;
	ret->z = v1->x * v2->y - v1->y * v2->x;
#else
	// �_�C���N�g�w�ł́A�A�A
	* ret = XMVector3Cross(*v1, *v2);
#endif

}

//=============================================================================
// ���C�L���X�g
// p0, p1, p2�@�|���S���̂R���_
// pos0 �n�_
// pos1 �I�_
// hit�@��_�̕ԋp�p
// normal �@���x�N�g���̕ԋp�p
// �������Ă���ꍇ�ATRUE��Ԃ�
//=============================================================================
BOOL RayCast(XMFLOAT3 xp0, XMFLOAT3 xp1, XMFLOAT3 xp2, XMFLOAT3 xpos0, XMFLOAT3 xpos1, XMFLOAT3* hit, XMFLOAT3* normal)
{
	XMVECTOR	p0 = XMLoadFloat3(&xp0);
	XMVECTOR	p1 = XMLoadFloat3(&xp1);
	XMVECTOR	p2 = XMLoadFloat3(&xp2);
	XMVECTOR	pos0 = XMLoadFloat3(&xpos0);
	XMVECTOR	pos1 = XMLoadFloat3(&xpos1);

	XMVECTOR	nor;	// �|���S���̖@��
	XMVECTOR	vec1;
	XMVECTOR	vec2;
	float		d1, d2;

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�(���̏����͌Œ蕨�Ȃ�\��Init()�ōs���Ă����Ɨǂ�)
		vec1 = p1 - p0;
		vec2 = p2 - p0;
		crossProduct(&nor, &vec2, &vec1);
		nor = XMVector3Normalize(nor);		// �v�Z���₷���悤�ɖ@�����m�[�}���C�Y���Ă���(���̃x�N�g���̒������P�ɂ��Ă���)
		XMStoreFloat3(normal, nor);			// ���߂��@�������Ă���
	}

	// �|���S�����ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�i�s�p�Ȃ�{�A�݊p�Ȃ�[�A���p�Ȃ�O�j
	vec1 = pos0 - p0;
	vec2 = pos1 - p0;
	{	// ���߂��|���S���̖@���ƂQ�̃x�N�g���i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		d1 = dotProduct(&vec1, &nor);
		d2 = dotProduct(&vec2, &nor);
		if (((d1 * d2) > 0.0f) || (d1 == 0 && d2 == 0))
		{
			// �������Ă���\���͖���
			return(FALSE);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		d1 = (float)fabs(d1);	// ��Βl�����߂Ă���
		d2 = (float)fabs(d2);	// ��Βl�����߂Ă���
		float a = d1 / (d1 + d2);							// ������

		XMVECTOR	vec3 = (1 - a) * vec1 + a * vec2;		// p0�����_�ւ̃x�N�g��
		XMVECTOR	p3 = p0 + vec3;							// ��_
		XMStoreFloat3(hit, p3);								// ���߂���_�����Ă���

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			XMVECTOR	v1 = p1 - p0;
			XMVECTOR	v2 = p2 - p1;
			XMVECTOR	v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			XMVECTOR	v4 = p3 - p1;
			XMVECTOR	v5 = p3 - p2;
			XMVECTOR	v6 = p3 - p0;

			// �O�ςŊe�ӂ̖@�������߂āA�|���S���̖@���Ƃ̓��ς��Ƃ��ĕ������`�F�b�N����
			XMVECTOR	n1, n2, n3;

			crossProduct(&n1, &v4, &v1);
			if (dotProduct(&n1, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n2, &v5, &v2);
			if (dotProduct(&n2, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�

			crossProduct(&n3, &v6, &v3);
			if (dotProduct(&n3, &nor) < 0.0f) return(FALSE);	// �������Ă��Ȃ�
		}
	}

	return(TRUE);	// �������Ă���I(hit�ɂ͓������Ă����_�������Ă���Bnormal�ɂ͖@���������Ă���)
}
//=============================================================================
//�{�b�N�X�ɂ�铖���蔻��
//=============================================================================
BOOL BoxCollider(XMFLOAT3 size, XMFLOAT3 pos, float rotationY, XMFLOAT3 rayStart, XMFLOAT3 rayEnd, XMFLOAT3* hit, XMFLOAT3* normal)
{
	// Y����]�̍s��𐶐�
	XMMATRIX rotationMatrix = XMMatrixRotationY(rotationY);

	// �{�b�N�X�̃��[�J�����W�n�ł̒��_��`
	XMFLOAT3 localVertices[8] = {
		{ -size.x / 2, -size.y / 2, -size.z / 2 }, // ������
		{  size.x / 2, -size.y / 2, -size.z / 2 }, // �E����
		{ -size.x / 2,  size.y / 2, -size.z / 2 }, // ���㉜
		{  size.x / 2,  size.y / 2, -size.z / 2 }, // �E�㉜
		{ -size.x / 2, -size.y / 2,  size.z / 2 }, // �����O
		{  size.x / 2, -size.y / 2,  size.z / 2 }, // �E���O
		{ -size.x / 2,  size.y / 2,  size.z / 2 }, // ����O
		{  size.x / 2,  size.y / 2,  size.z / 2 }  // �E��O
	};

	// ��]��K�p���ă��[���h���W�n�ł̒��_���v�Z
	XMFLOAT3 worldVertices[8];
	for (int i = 0; i < 8; ++i) {
		XMVECTOR localVertex = XMLoadFloat3(&localVertices[i]);
		XMVECTOR worldVertex = XMVector3TransformCoord(localVertex, rotationMatrix);
		worldVertex = XMVectorAdd(worldVertex, XMLoadFloat3(&pos));
		XMStoreFloat3(&worldVertices[i], worldVertex); // XMVECTOR����XMFLOAT3�֕ϊ�
	}

	// �{�b�N�X�̊e�ʂ��`����|���S���̒��_�C���f�b�N�X
	int indices[36] = {
		0, 1, 2, 2, 1, 3, // ����
		1, 5, 3, 3, 5, 7, // �E��
		5, 4, 7, 7, 4, 6, // �O��
		4, 0, 6, 6, 0, 2, // ����
		2, 3, 6, 6, 3, 7, // ���
		4, 5, 0, 0, 5, 1  // ����
	};

	// �e�ʂɑ΂��ă��C�L���X�g���s���A��_�����邩�ǂ������`�F�b�N
	for (int i = 0; i < 12; ++i) {
		XMFLOAT3 p0 = worldVertices[indices[i * 3]];
		XMFLOAT3 p1 = worldVertices[indices[i * 3 + 1]];
		XMFLOAT3 p2 = worldVertices[indices[i * 3 + 2]];

		if (RayCast(p0, p1, p2, rayStart, rayEnd, hit, normal)) 
		{
			return TRUE; // �����肠��
		}
	}

	return FALSE; // ������Ȃ�
}
