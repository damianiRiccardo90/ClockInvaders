///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Clock.h"

// Local.
#include "C_Application.h"
#include "graphics.h"
#include "Projectile.h"
#include "RandomGenerator.h"
#include "time.h"
#include "Vector2D.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

static const Vector2D k_CLOCK_DEFAULT_MINHALFDIAG = Vector2D( 2.5f, 2.5f );
static const unsigned int k_CLOCK_DEFAULT_COLOR = Color::k_RED;
static const unsigned int k_CLOCK_DEFAULT_HOURSHANDCOLOR = Color::k_WHITE;
static const unsigned int k_CLOCK_DEFAULT_MINUTESHANDCOLOR = Color::k_BLUE;
static const float k_CLOCK_DEFAULT_MINVELOCITY = 50.f;
static const float k_CLOCK_DEFAULT_MAXVELOCITY = 200.f;
static const float k_CLOCK_DEFAULT_HOURSANGLESPAN = k_PI / 12;
static const float k_CLOCK_DEFAULT_MINUTESANGLESPAN = k_PI / 60;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Clock::Clock( C_Application* owner, const Vector2D& position /*= Vector2D::s_ZERO*/, 
	const Vector2D& velocity /*= Vector2D::s_ZERO*/, const Vector2D& halfDiag /*= k_CLOCK_DEFAULT_BBHALFDIAG*/ )
	: Entity( owner, k_CLOCK_DEFAULT_COLOR, halfDiag, position, Vector2D::s_UP, velocity )
{
	if ( m_Position == Vector2D::s_ZERO && m_Velocity == Vector2D::s_ZERO && 
		halfDiag == k_CLOCK_DEFAULT_BBHALFDIAG )
	{
		Randomize();
	}
}

void Clock::Render()
{
	Entity::Render();

	// Draw the external box containing the clock.

	const Vector2D upLtVt = m_Position + Vector2D( -m_BBHalfDiagonal.GetX(), -m_BBHalfDiagonal.GetY() );
	const Vector2D lowLtVt = m_Position + Vector2D( -m_BBHalfDiagonal.GetX(), m_BBHalfDiagonal.GetY() );
	const Vector2D lowRtVt = m_Position + Vector2D( m_BBHalfDiagonal.GetX(), m_BBHalfDiagonal.GetY() );
	const Vector2D upRtVt = m_Position + Vector2D( m_BBHalfDiagonal.GetX(), -m_BBHalfDiagonal.GetY() );

	const int xUpLtVt = static_cast< int >( upLtVt.GetX() );
	const int yUpLtVt = static_cast< int >( upLtVt.GetY() );
	const int xLowLtVt = static_cast< int >( lowLtVt.GetX() );
	const int yLowLtVt = static_cast< int >( lowLtVt.GetY() );
	const int xLowRtVt = static_cast< int >( lowRtVt.GetX() );
	const int yLowRtVt = static_cast< int >( lowRtVt.GetY() );
	const int xUpRtVt = static_cast< int >( upRtVt.GetX() );
	const int yUpRtVt = static_cast< int >( upRtVt.GetY() );
	DrawLine( xUpLtVt, yUpLtVt, xLowLtVt, yLowLtVt, m_Color );
	DrawLine( xLowLtVt, yLowLtVt, xLowRtVt, yLowRtVt, m_Color );
	DrawLine( xLowRtVt, yLowRtVt, xUpRtVt, yUpRtVt, m_Color );
	DrawLine( xUpRtVt, yUpRtVt, xUpLtVt, yUpLtVt, m_Color );

	// Draw the internal hands of the clock.

	// Get current time.
	int currHours;
	int currMinutes;
	int currSeconds;
	GetTime( currHours, currMinutes, currSeconds );

	Vector2D hoursHandDir = Vector2D::s_UP;
	hoursHandDir.Rotate( -( currHours * k_CLOCK_DEFAULT_HOURSANGLESPAN ) );
	const Vector2D hoursHandPos = m_Position + hoursHandDir * m_BBHalfDiagonal.GetX() / 2;

	Vector2D minutesHandDir = Vector2D::s_UP;
	minutesHandDir.Rotate( -( currMinutes * k_CLOCK_DEFAULT_MINUTESANGLESPAN ) );
	const Vector2D minutesHandPos = m_Position + minutesHandDir * m_BBHalfDiagonal.GetX();

	const int xPos = static_cast< int >( m_Position.GetX() );
	const int yPos = static_cast< int >( m_Position.GetY() );
	const int xHoursHand = static_cast< int >( hoursHandPos.GetX() );
	const int yHoursHand = static_cast< int >( hoursHandPos.GetY() );
	const int xMinutesHand = static_cast< int >( minutesHandPos.GetX() );
	const int yMinutesHand = static_cast< int >( minutesHandPos.GetY() );
	DrawLine( xPos, yPos, xHoursHand, yHoursHand, k_CLOCK_DEFAULT_HOURSHANDCOLOR );
	DrawLine( xPos, yPos, xMinutesHand, yMinutesHand, k_CLOCK_DEFAULT_MINUTESHANDCOLOR );
}

void Clock::HandleCollision( Entity* other )
{
	Entity::HandleCollision( other );

	// Split the clock if we're being hit by a projectile, and the current size is higher than the allowed minimum.
	if ( Projectile* projectilePtr = dynamic_cast< Projectile* >( other ) )
	{
		if ( m_BBHalfDiagonal.GetX() > k_CLOCK_DEFAULT_MINHALFDIAG.GetX() || 
			m_BBHalfDiagonal.GetY() > k_CLOCK_DEFAULT_MINHALFDIAG.GetY() )
		{
			Split();
		}
		else
		{
			Destroy();
		}
	}
	// If hit by another clock, just invert the velocity vector.
	else if ( Clock* clockPtr = dynamic_cast< Clock* >( other ) )
	{
		m_Velocity = -m_Velocity;
	}
}

void Clock::HandleScreenBordersCollision()
{
	// When the screen borders are hit, invert the corresponding coordinate of the velocity vector.

	if ( IsCollidingWithScreenHorizontally() )
	{
		m_Velocity.SetX( -m_Velocity.GetX() );
	}

	if ( IsCollidingWithScreenVertically() )
	{
		m_Velocity.SetY( -m_Velocity.GetY() );
	}
}

void Clock::Randomize()
{
	if ( !m_Owner ) return;

	SetPosition( GetRandomPosition() );
	SetVelocity( GetRandomVelocity() );
}

Vector2D Clock::GetRandomPosition() const
{
	if ( !m_Owner ) return Vector2D::s_ZERO;

	const float xMin = m_BBHalfDiagonal.GetX();
	const float xMax = m_Owner->GetScreenWidth() - m_BBHalfDiagonal.GetX();
	const float yMin = m_BBHalfDiagonal.GetY();
	const float yMax = m_Owner->GetScreenHeight() - m_BBHalfDiagonal.GetY();

	RandomGenerator& random = RandomGenerator::GetInstance();
	return Vector2D( random.GenerateFloat( xMin, xMax ), random.GenerateFloat( yMin, yMax ) );
}

Vector2D Clock::GetRandomVelocity() const
{
	RandomGenerator& random = RandomGenerator::GetInstance();
	const float randomVelocityMagnitude =
		random.GenerateFloat( k_CLOCK_DEFAULT_MINVELOCITY, k_CLOCK_DEFAULT_MAXVELOCITY );
	const float randomRotation = random.GenerateFloat( 0.f, 2 * k_PI );
	Vector2D randomVelocity = Vector2D::s_UP;
	randomVelocity.Rotate( randomRotation ) *= randomVelocityMagnitude;
	return randomVelocity;
}

void Clock::Split()
{
	if ( !m_Owner ) return;

	const Vector2D halfCurrBB = m_BBHalfDiagonal / 2;
	RandomGenerator& random = RandomGenerator::GetInstance();
	const bool switchSides = random.GenerateBool();
	const Vector2D firstClockOffsetPos = 
		switchSides ? Vector2D( halfCurrBB.GetX(), -halfCurrBB.GetY() ) : halfCurrBB;
	m_Owner->RequestSpawnClock( m_Position + firstClockOffsetPos, GetRandomVelocity(), halfCurrBB );
	m_Owner->RequestSpawnClock( m_Position - firstClockOffsetPos, GetRandomVelocity(), halfCurrBB );

	Destroy();
}
