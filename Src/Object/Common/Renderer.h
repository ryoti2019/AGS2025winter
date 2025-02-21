#pragma once
#include<DxLib.h>
#include<memory>
#include "../Common/Vector2.h"
class Material;
class ModelMaterial;

class Renderer
{

public:

	// �R���X�g���N�^
	Renderer(int modelId, std::shared_ptr<ModelMaterial> modelMaterial);

	// �f�X�g���N�^
	~Renderer();

	// �`���`�̐���
	void MakeSquereVertex(void);

	void SetConstBufsVS(FLOAT4 constBufs, int idx);

	void SetConstBufsPS(FLOAT4 constBufs, int idx);

	// �`��
	void Draw(void);

private:

	// ���_�V�F�[�_�p�I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	// �s�N�Z���V�F�[�_�p�̃I���W�i���萔�o�b�t�@�̎g�p�J�n�X���b�g
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// ���_��
	static constexpr int NUM_VERTEX = 4;

	// ���_�C���f�b�N�X
	static constexpr int NUM_VERTEX_IDX = 6;

	//�|���S����
	static constexpr int NUM_POLYGON = 2;

	// �e�N�X�`�����W�̒��S
	static constexpr float HALF_TEX_COORD = 0.5f;

	// �e�N�X�`�����W�̍ő�l
	static constexpr float FULL_TEX_COORD = 1.0f;

	// �C���f�b�N�X�o�b�t�@�̗v�f��
	static constexpr int NUM_INDICES = 6;

	// 2�̎O�p�`�ŕ`�悷�邽�߂̃C���f�b�N�X���
	static constexpr int INDEX_SEQUENCE[NUM_INDICES] = { 0, 1, 2, 1, 3, 2 };

	// �F�̍ő�l
	static constexpr int COLOR_MAX = 255;

	// ���f���̃n���h��ID
	int modelId_;

	// ���_
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// ���_�C���f�b�N�X
	WORD indexes_[NUM_VERTEX_IDX];

	// ���f���}�e���A��
	std::shared_ptr<ModelMaterial> modelMaterial_;

	int screen_;

	void SetReservePS(void);

	void SetReserveVS(void);

};

