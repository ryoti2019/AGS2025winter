#pragma once
class Vector2;

class Vector2F {

public:

	// デフォルトコンストラクタ
	Vector2F();

	// コンストラクタ
	Vector2F(float vX, float vY);

	// コンストラクタ
	Vector2F(Vector2 value);

	float x, y;

	Vector2F operator+(const Vector2F& value);
	Vector2F operator-(const Vector2F& value);

private:

};