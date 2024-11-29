#include <DxLib.h>
#include "../Utility/Utility.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
	:
	modelId(-1),
	scl(Utility::VECTOR_ZERO),
	rot(Utility::VECTOR_ZERO),
	pos(Utility::VECTOR_ZERO),
	localPos(Utility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{
}

TransformComponent::TransformComponent(int model)
	:
	modelId(model),
	scl(Utility::VECTOR_ZERO),
	rot(Utility::VECTOR_ZERO),
	pos(Utility::VECTOR_ZERO),
	localPos(Utility::VECTOR_ZERO),
	matScl(MGetIdent()),
	matRot(MGetIdent()),
	matPos(MGetIdent()),
	quaRot(Quaternion()),
	quaRotLocal(Quaternion())
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Update()
{

	// �傫��
	matScl = MGetScale(scl);

	// ��]
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// �ʒu
	matPos = MGetTranslate(pos);

	// �s��̍���
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// �s������f���ɔ���
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

}

void TransformComponent::Release()
{
}