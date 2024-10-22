#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{

public:

	// �A�j���[�V�����f�[�^
	struct AnimationData
	{

		// �A�j���[�V�����n���h��
		int animHandle = -1;

		// �A�j���[�V�����^�C�v
		std::string state = "";

		// �A�^�b�`No
		int attachNo = -1;

		// �D�悳���A�j���[�V����
		bool isPriority = false;

		// �A�j���[�V�����̑��Đ�����
		float animTotalTime = 0.0f;

		// �A�j���[�V�������x
		float speedAnim = 0.0f;

		// �Đ����̃A�j���[�V��������
		float stepAnim = 0.0f;

		// �A�j���[�V�������n�܂鎞��
		float startTime = 0.0f;

		// �A�j���[�V�������I��鎞��
		float endStep = 0.0f;

		// �u�����h���[�g
		float blendRate = 0.0f;

		// �u�����h�^�C��
		float blendTime = 0.0f;

		// ���[�v���邩���Ȃ���
		bool isLoop = false;

		// �X�g�b�v���邩���Ȃ���
		bool isStop = false;

		// �t�Đ�
		bool isReverse = false;

		// �A�j���[�V�����ԍ�
		int animIndex = 0;

	};

	// �R���X�g���N�^
	AnimationController(int modelId);

	// �f�X�g���N�^
	~AnimationController(void);

	// �A�j���[�V�����ǉ�
	void Add(const std::string state, const std::string& path, float startStep,
		 float speed, int animHandle, bool isLoop, int animIndex, bool isReverse);

	// �A�j���[�V�����̍X�V
	void Update(const float deltaTime);

	// �Đ����̃A�j���[�V����
	std::string GetPlayType(void) const;

	// �A�^�b�`����Ă��鐔���擾
	int GetAttachNum(void) const;

	// ��ԑJ��
	void ChangeAnimation(std::string state);

	// �A�j���[�V�����f�[�^�̎擾
	AnimationData GetAnimData(const std::string& state);

	// ���ׂẴA�j���[�V�����f�[�^���擾
	const std::map <std::string, AnimationData>& GetAnimDatas(void) const;

	// �A�j���[�V�������n�܂鎞�Ԃ�ݒ�
	void SetStartStepAnim(std::string state, int stepAnim);

	// �D�悳��Ă���A�j���[�V�����^�C�v���擾
	bool GetIsPriority(void);

	// �A�j���[�V�������Đ�����Ă��邩�ǂ���
	bool IsPlayNowAnimation(void);

	// ���̃A�j���[�V�����̍Đ����I�����Ă��邩����
	bool IsEndPlayAnimation(void);

	// ���̃A�j���[�V�����̑��Đ����Ԃ�-1.0�O���I�����Ă��邩����
	bool IsPreEndPlayAnimation(void);

	// �Đ����Ă���A�j���[�V�������u�����h����Ă��邩�ǂ���
	bool IsBlendPlay(std::string state);

	// �u�����h���Ă���A�j���[�V�������Đ��I�����Ă��邩����
	bool IsEndBlendingPlayAnimation(std::string state);

	// �X�g�b�v������
	void Stop(bool stop);

	// �X���[������
	void Slow(bool isSlow);

	// �A�j���[�V�����X�s�[�h��ς���
	void SpeedChange(float speed);

private:

	// ���f���̃n���h��ID
	int modelId_;

	// ���f����ǂݍ��񂾂��̃t���O
	bool LoadModel_;

	// ��ޕʂ̃A�j���[�V�����f�[�^
	std::map <std::string, AnimationData> animData_;

	// ���̏��
	std::string state_;

	// ��O�̏��
	std::string preState_;

	// �A�^�b�`���Ă��鐔
	int AttachNum_;

	// �q�b�g�X�g�b�v
	bool isStop_;

	// �X�s�[�h�R���g���[������p
	bool isSlow_;

	// �Q�[���S�̂̃X�s�[�h
	float gameSpeed_;

	// �A�^�b�`
	void Attatch(std::string state);

	// �f�^�b�`
	void Dettach(int attachNo);

};

