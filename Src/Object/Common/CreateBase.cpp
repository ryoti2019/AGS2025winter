#include <random>
#include "CreateBase.h"

CreateBase::CreateBase()
{
}

void CreateBase::Update()
{
}

void CreateBase::Draw()
{
}

float CreateBase::GenerateRandNumber()
{

	// 非決定的な乱数生成器
	std::random_device rd;

	// メルセンヌ・ツイスタ法による乱数生成器
	std::mt19937 gen(rd());

	// 指定の範囲でランダムな数を取得
	std::uniform_int_distribution<> dist_int(-1000, 1000);
	float number = dist_int(gen);

    return number;

}
