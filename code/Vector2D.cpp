///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Vector2D.h"

// System.
#include <cmath>

//////////////  S T A T I C  M E M B E R  V A R I A B L E S  /////////////

const Vector2D Vector2D::s_Up( 0.f, -1.f );
const Vector2D Vector2D::s_Down( 0.f, 1.f );
const Vector2D Vector2D::s_Left( -1.f, 0.f );
const Vector2D Vector2D::s_Right( 1.f, 0.f );
const Vector2D Vector2D::s_Zero( 0.f, 0.f );

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Vector2D::Vector2D( const float x /*= 0.f*/, const float y /*= 0.f*/ )
	: x( x )
	, y( y )
	, accumulatedRotation( 0.f )
{}

Vector2D::Vector2D( const Vector2D& other )
	: x( other.x )
	, y( other.y )
	, accumulatedRotation( other.accumulatedRotation )
{}

Vector2D& Vector2D::operator=( const Vector2D& other )
{
	if ( this != &other )
	{
		x = other.x;
		y = other.y;
		accumulatedRotation = other.accumulatedRotation;
	}
	return *this;
}

Vector2D Vector2D::operator+( const Vector2D& other ) const
{
	return Vector2D( x + other.x, y + other.y );
}

Vector2D Vector2D::operator+( const float scalar ) const
{
	return Vector2D( x + scalar, y + scalar );
}

void Vector2D::operator+=( const Vector2D& other )
{
	x += other.x;
	y += other.y;
}

void Vector2D::operator+=( const float scalar )
{
	x += scalar;
	y += scalar;
}

Vector2D Vector2D::operator-( const Vector2D& other ) const
{
	return Vector2D( x - other.x, y - other.y );
}

Vector2D Vector2D::operator-( const float scalar ) const
{
	return Vector2D( x - scalar, y - scalar );
}

void Vector2D::operator-=( const Vector2D& other )
{
	x -= other.x;
	y -= other.y;
}

void Vector2D::operator-=( const float scalar )
{
	x -= scalar;
	y -= scalar;
}

Vector2D Vector2D::operator*( const float scalar ) const
{
	return Vector2D( x * scalar, y * scalar );
}

void Vector2D::operator*=( const float scalar )
{
	x *= scalar;
	y *= scalar;
}

float Vector2D::operator*( const Vector2D& other ) const
{
	return x * other.x + y * other.y;
}

Vector2D Vector2D::operator/( const float scalar ) const
{
	if ( scalar == 0.f ) return Vector2D();

	return Vector2D( x / scalar, y / scalar );
}

void Vector2D::operator/=( const float scalar )
{
	if ( scalar == 0.f ) return;

	x /= scalar;
	y /= scalar;
}

bool Vector2D::operator==( const Vector2D& other ) const
{
	return x == other.x && y == other.y;
}

bool Vector2D::operator!=( const Vector2D& other ) const
{
	return x != other.x || y != other.y;
}

Vector2D& Vector2D::Rotate( const float rad, const Vector2D origin /*= s_Zero*/)
{
	const float sin = std::sin( rad );
	const float cos = std::cos( rad );

	*this -= origin;

	const float oldX = x;
	const float oldY = y;
	x = oldX * cos - oldY * sin;
	y = oldX * sin + oldY * cos;

	*this += origin;

	return *this;
}

Vector2D Vector2D::Abs() const
{
	return Vector2D( std::abs( x ), std::abs( y ) );
}

void Vector2D::Clamp( const float xMin, const float xMax, const float yMin, const float yMax )
{
	x = ( x < xMin ) ? xMin : ( ( x > xMax ) ? xMax : x );
	y = ( y < yMin ) ? yMin : ( ( y > yMax ) ? yMax : y );
}

float Vector2D::Magnitude() const
{
	return std::sqrt( x * x + y * y );
}

Vector2D& Vector2D::Normalize()
{
	const float magnitude = Magnitude();
	if ( magnitude != 0.f )
	{
		x /= magnitude;
		y /= magnitude;
	}

	return *this;
}
