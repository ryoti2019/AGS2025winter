#include <math.h>
#include "DxLib.h"

class FpsControl {

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

	//���ς����T���v����
	static constexpr int FPS_AVERAGE_FRAME_COUNT = 60;

	//�ݒ肵��FPS
	static constexpr int FPS = 60;

	// 1�b������̃~���b
	static constexpr int MS_PER_SECOND = 1000;

	// �F�̍ő�l
	static constexpr int COLOR_MAX = 255;

	// �V���O���g���p�C���X�^���X
	static FpsControl* instance_;
	
	//����J�n����
	int mStartTime;

	//�J�E���^
	int mCount;

	//fps
	float mFps;

};
