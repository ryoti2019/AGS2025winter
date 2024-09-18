#include <math.h>
#include "DxLib.h"

class FpsControl {
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	static const int N = 60;//���ς����T���v����
	static const int FPS = 60;	//�ݒ肵��FPS

public:
	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static FpsControl& GetInstance(void);
	FpsControl();
	void Init(void);
	bool Update();
	void Draw();
	void Wait();

private:
	// �V���O���g���p�C���X�^���X
	static FpsControl* instance_;
	
};
