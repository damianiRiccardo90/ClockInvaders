///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Projectile.h"

// Local.
#include "C_Application.h"
#include "graphics.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

static const unsigned int k_PROJECTILE_DEFAULT_COLOR = Color::k_WHITE;
static const float k_PROJECTILE_DEFAULT_VELOCITY = 150.f;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Projectile::Projectile( C_Application* owner, const Vector2D& position, const Vector2D& facing )
	// The BB is a rectangle that has the projectile (which is just a line) as its diagonal, so to find the vector
	// that goes from the center to the lower right vertex of the BB, I need to take the line direction, retrieve 
	// its absolute value (so that it points in the right direction no matter the rotation applied) and then scale 
	// it using half the total length of the line (which represents a projectile). I'm assuming the facing vector
	// is already normalized.
	: Entity( owner, k_PROJECTILE_DEFAULT_COLOR, Vector2D( facing.Abs() * k_PROJECTILE_DEFAULT_HALFLENGTH ), 
		position, facing, facing * k_PROJECTILE_DEFAULT_VELOCITY )
{}

void Projectile::Render()
{
	const Vector2D lineStart = m_Position - m_Facing * k_PROJECTILE_DEFAULT_HALFLENGTH;
	const Vector2D lineEnd = m_Position + m_Facing * k_PROJECTILE_DEFAULT_HALFLENGTH;

	const int xLineStart = static_cast< int >( lineStart.GetX() );
	const int yLineStart = static_cast< int >( lineStart.GetY() );
	const int xLineEnd = static_cast< int >( lineEnd.GetX() );
	const int yLineEnd = static_cast< int >( lineEnd.GetY() );
	DrawLine( xLineStart, yLineStart, xLineEnd, yLineEnd, m_Color );
}

void Projectile::HandleScreenBordersCollision()
{
	Destroy();
}
