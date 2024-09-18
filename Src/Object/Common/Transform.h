#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>
class Transform
{

public:

	// �R���X�g���N�^
	Transform(void);
	Transform(const int model);

	// �f�X�g���N�^
	~Transform(void);

	// ���f���̃n���h��ID
	int modelId;

	// �傫��
	VECTOR scl;

	// ��]
	VECTOR rot;

	// �ʒu
	VECTOR pos;
	VECTOR localPos;

	// �s��
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// ��]
	Quaternion quaRot;

	// ���[�J����]
	Quaternion quaRotLocal;

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	void Release(void);

	void SetModel(const int model) { modelId = model; };

	// �O���������擾
	const VECTOR& GetForward(void) const { return GetDir(Utility::DIR_F); };

	// ����������擾
	const VECTOR& GetBack(void) const { return GetDir(Utility::DIR_B); };

	// �E�������擾
	const VECTOR& GetRight(void) const { return GetDir(Utility::DIR_R); };

	// ���������擾
	const VECTOR& GetLeft(void) const { return GetDir(Utility::DIR_L); };

	// ��������擾
	const VECTOR& GetUp(void) const { return GetDir(Utility::DIR_U); };

	// ���������擾
	const VECTOR& GetDown(void) const { return GetDir(Utility::DIR_D); };

	// �Ώە������擾
	const VECTOR& GetDir(const VECTOR& vec) const { return quaRot.PosAxis(vec); };

};