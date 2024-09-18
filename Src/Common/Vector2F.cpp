#include "Vector2.h"
#include "Vector2F.h"

Vector2F::Vector2F()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2F::Vector2F(float vX, float vY)
{
	x = vX;
	y = vY;
}

Vector2F::Vector2F(Vector2 value)
{
	x = static_cast<float>(value.x);
	y = static_cast<float>(value.y);
}

Vector2F Vector2F::operator+(const Vector2F& value)
{
	return {x + value.x, y + value.y};
}

Vector2F Vector2F::operator-(const Vector2F& value)
{
	return { x - value.x, y - value.y };
}
