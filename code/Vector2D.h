#pragma once

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class Vector2D
{
public:

	static const Vector2D s_Up;
	static const Vector2D s_Down;
	static const Vector2D s_Left;
	static const Vector2D s_Right;
	static const Vector2D s_Zero;

	explicit Vector2D( const float x = 0.f, const float y = 0.f );
	Vector2D( const Vector2D& other );
	Vector2D& operator=( const Vector2D& other );

	Vector2D operator+( const Vector2D& other ) const;
	Vector2D operator+( const float scalar ) const;
	void operator+=( const Vector2D& other );
	void operator+=( const float scalar );
	Vector2D operator-( const Vector2D& other ) const;
	Vector2D operator-( const float scalar ) const;
	Vector2D operator-() const;
	void operator-=( const Vector2D& other );
	void operator-=( const float scalar );
	Vector2D operator*( const float scalar ) const; // Scalar-Vector Product.
	void operator*=( const float scalar ); // Scalar-Vector Product.
	float operator*( const Vector2D& other ) const; // Dot Product.
	Vector2D operator/( const float scalar ) const;
	void operator/=( const float scalar );
	bool operator==( const Vector2D& other ) const;
	bool operator!=( const Vector2D& other ) const;

	void SetX( const float newX ) { x = newX; }
	float GetX() const { return x; }
	void SetY( const float newY ) { y = newY; }
	float GetY() const { return y; }

	Vector2D& Rotate( const float rad, const Vector2D origin = Vector2D::s_Zero);
	Vector2D Abs() const;
	void Clamp( const float xMin, const float xMax, const float yMin, const float yMax );
	float Magnitude() const;
	Vector2D Normalized() const;

private:

	float x;
	float y;
	float accumulatedRotation;
};
