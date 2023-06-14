///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Vector2D.h"

// System.
#include <algorithm>
#include <cmath>

///////////////////////////  C O N S T A N T S  //////////////////////////

static const float k_ROTATION_TOLERANCE = 0.001f;

//////////////  S T A T I C  M E M B E R  V A R I A B L E S  /////////////

const Vector2D Vector2D::s_UP( 0.f, -1.f );
const Vector2D Vector2D::s_DOWN( 0.f, 1.f );
const Vector2D Vector2D::s_LEFT( -1.f, 0.f );
const Vector2D Vector2D::s_RIGHT( 1.f, 0.f );
const Vector2D Vector2D::s_ZERO( 0.f, 0.f );

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Vector2D::Vector2D( const float x /*= 0.f*/, const float y /*= 0.f*/ )
	: x( x )
	, y( y )
{}

Vector2D::Vector2D( const Vector2D& other )
	: x( other.x )
	, y( other.y )
{}

Vector2D& Vector2D::operator=( const Vector2D& other )
{
	if ( this != &other )
	{
		x = other.x;
		y = other.y;
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

Vector2D Vector2D::operator-() const
{
	return Vector2D( -x, -y );
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
	// Negating the input radians will result in counter-clockwise rotation when rad is positive.
	const float sin = std::sin( -rad );
	const float cos = std::cos( -rad ); 

	*this -= origin;

	const float oldX = x;
	const float oldY = y;
	x = oldX * cos - oldY * sin;
	y = oldX * sin + oldY * cos;

	*this += origin;

	return *this;
}

Vector2D& Vector2D::ClampRotate( const float rad, const float min, const float max, 
	const Vector2D origin /*= s_Zero*/ )
{
	const float oldRotation = GetRotation();
	const float newRotation = oldRotation + rad;
	float clampedRotation = std::clamp( newRotation, min, max );
	const float rotationDelta = clampedRotation - oldRotation;

	return Rotate( rotationDelta, origin );
}

float Vector2D::GetRotation() const
{
	// The std::atan2 function returns the angle in radians within the range[ -PI, PI ], with positive angles in 
	// the counterclockwise direction.
	float rotation = std::atan2( -y, x );
	
	// Wrap the angle around to the range of [ 0, 2PI ].
	if ( rotation < 0.f )
	{
		rotation += 2 * k_PI;
	}
	else if ( rotation >= 2 * k_PI )
	{
		rotation -= 2 * k_PI;
	}

	return rotation;
}

Vector2D Vector2D::Abs() const
{
	return Vector2D( std::abs( x ), std::abs( y ) );
}

void Vector2D::Clamp( const float xMin, const float xMax, const float yMin, const float yMax )
{
	x = std::clamp( x, xMin, xMax );
	y = std::clamp( y, yMin, yMax );
}

float Vector2D::Magnitude() const
{
	return std::sqrt( x * x + y * y );
}

Vector2D Vector2D::Normalized() const
{
	Vector2D output( *this );
	output /= Magnitude();

	return output;
}
