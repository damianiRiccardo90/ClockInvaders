#pragma once

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class Vector2D
{
public:

	// Construct on first use idiom, to ensure proper initialization of static members across different classes.
	static const Vector2D& GetUp();
	static const Vector2D& GetDown();
	static const Vector2D& GetLeft();
	static const Vector2D& GetRight();
	static const Vector2D& GetZero();
	static float GetPI();

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

	Vector2D& Rotate( const float rad, const Vector2D& origin = GetZero() );
	Vector2D& ClampRotate( const float rad, const float min, const float max, const Vector2D& origin = GetZero() );
	// Returns the rotation in the range [0, 2PI], rotating counter-clockwise and starting from the s_Right vector.
	float GetRotation() const;
	Vector2D Abs() const;
	void Clamp( const float xMin, const float xMax, const float yMin, const float yMax );
	float Magnitude() const;
	Vector2D Normalized() const;

	static const float s_ROTATION_TOLERANCE;

private:

	float x;
	float y;
};
