#pragma once
#include <DxLib.h>
#include <string>
#include <map>
#include "../Common/Transform.h"
class ResourceManager;

class EffekseerController
{

public:

	struct EFFECT_DATA
	{

		// �G�t�F�N�g�̖��O
		std::string name;

		// �G�t�F�N�g�n���h��
		int resId;

		// �Đ��n���h��
		int playId;

		// ���W
		VECTOR pos;

		// �傫��
		float scale;
	};

	// �R���X�g���N�^
	EffekseerController();

	// �f�X�g���N�^
	~EffekseerController();

	/// <summary>
	/// �G�t�F�N�g�f�[�^�ǉ�
	/// </summary>
	/// <param name="name">�G�t�F�N�g�̖��O</param>
	/// <param name="handleId">�G�t�F�N�g�n���h��</param>
	/// <param name="scale">�G�t�F�N�g�̃X�P�[��</param>
	virtual void Add(const std::string& name, const int handleId, const float scale);

	// �`��
	virtual void Draw(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle);

	// �`����~�߂�
	virtual void DrawStop(const std::string& handle);

	// �G�t�F�N�g�Ǐ]
	virtual void FollowPos(const VECTOR& pos, const Quaternion& quaRot, const VECTOR& relativePos, const std::string& handle);

protected:

	// �G�t�F�N�g�f�[�^
	std::map <std::string, EFFECT_DATA> effectData_;

};