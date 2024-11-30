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

	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

}

void TransformComponent::Release()
{
}