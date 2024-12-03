#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Utility/Utility.h"
#include "../Component/Component.h"

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
	Transform();
	Transform(const int model);

	// �f�X�g���N�^
	~Transform();

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
	void Update();

	void Release();

	void SetModel(const int model) { modelId = model; };

	// �O���������擾
	const VECTOR& GetForward() const { return GetDir(Utility::DIR_F); };

	// ����������擾
	const VECTOR& GetBack() const { return GetDir(Utility::DIR_B); };

	// �E�������擾
	const VECTOR& GetRight() const { return GetDir(Utility::DIR_R); };

	// ���������擾
	const VECTOR& GetLeft() const { return GetDir(Utility::DIR_L); };

	// ��������擾
	const VECTOR& GetUp() const { return GetDir(Utility::DIR_U); };

	// ���������擾
	const VECTOR& GetDown() const { return GetDir(Utility::DIR_D); };

	// �Ώە������擾
	const VECTOR& GetDir(const VECTOR& vec) const { return quaRot.PosAxis(vec); };

};