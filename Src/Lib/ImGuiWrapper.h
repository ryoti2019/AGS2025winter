#pragma once
#include <Windows.h>

class ImGuiWrapper
{

public:

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static ImGuiWrapper& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

private:

	// �ÓI�C���X�^���X
	static ImGuiWrapper* instance_;

	// �E�C���h�E�v���[�W��
    static LRESULT CALLBACK WndProc(
        HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ImGuiWrapper(void);
	// �R�s�[�R���X�g���N�^�����l
	ImGuiWrapper(const ImGuiWrapper&);
	// �f�X�g���N�^�����l
	~ImGuiWrapper(void);

	void UpdateInputMouse(void);
	void UpdateNewFrame(void);

};

