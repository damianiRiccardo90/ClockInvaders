///////////////////////////  I N C L U D E S  ////////////////////////////

// Header.
#include "Clock.h"

// System.
#include <random>

// Local.
#include "C_Application.h"
#include "graphics.h"
#include "time.h"
#include "Vector2D.h"

///////////////////////////  C O N S T A N T S  //////////////////////////

static const Vector2D k_CLOCK_DEFAULT_BBHALFDIAG = Vector2D( 50.f, 50.f );
static const Vector2D k_CLOCK_DEFAULT_MINSIZE = Vector2D( 2.5f, 2.5f );
static const unsigned int k_CLOCK_DEFAULT_COLOR = Color::k_RED;
static const unsigned int k_CLOCK_DEFAULT_HOURSHANDCOLOR = Color::k_WHITE;
static const unsigned int k_CLOCK_DEFAULT_MINUTESHANDCOLOR = Color::k_BLUE;
static const float k_CLOCK_DEFAULT_MINVELOCITY = 50.f;
static const float k_CLOCK_DEFAULT_MAXVELOCITY = 200.f;
static const float k_CLOCK_DEFAULT_HOURSANGLESPAN = k_PI / 12;
static const float k_CLOCK_DEFAULT_MINUTESANGLESPAN = k_PI / 60;

////////////////  F U N C T I O N  D E F I N I T I O N S  ////////////////

Clock::Clock( C_Application* owner, const Vector2D& position /*= Vector2D::s_Zero*/, 
	const Vector2D& velocity /*= Vector2D::s_Zero*/ )
	: Entity( owner, k_CLOCK_DEFAULT_COLOR, k_CLOCK_DEFAULT_BBHALFDIAG, position, Vector2D::s_UP, velocity )
{
	if ( m_Position == Vector2D::s_ZERO && m_Velocity == Vector2D::s_ZERO )
	{
		Randomize();
	}
}

void Clock::Render()
{
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
	hoursHandDir.Rotate( -( currHours * s_Default_HoursAngleSpan ) );
	const Vector2D hoursHandPos = m_Position + hoursHandDir * s_Default_BBHalfDiag.GetX() / 2;

	Vector2D minutesHandDir = Vector2D::s_UP;
	minutesHandDir.Rotate( -( currMinutes * s_Default_MinutesAngleSpan ) );
	const Vector2D minutesHandPos = m_Position + minutesHandDir * s_Default_BBHalfDiag.GetX();

	const int xPos = static_cast< int >( m_Position.GetX() );
	const int yPos = static_cast< int >( m_Position.GetY() );
	const int xHoursHand = static_cast< int >( hoursHandPos.GetX() );
	const int yHoursHand = static_cast< int >( hoursHandPos.GetY() );
	const int xMinutesHand = static_cast< int >( minutesHandPos.GetX() );
	const int yMinutesHand = static_cast< int >( minutesHandPos.GetY() );
	DrawLine( xPos, yPos, xHoursHand, yHoursHand, s_Default_HoursHandColor );
	DrawLine( xPos, yPos, xMinutesHand, yMinutesHand, s_Default_MinutesHandColor );
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
	// Set a random position within the screen range boundaries.
	const float xMin = s_Default_BBHalfDiag.GetX();
	const float xMax = m_Owner->GetScreenWidth() - s_Default_BBHalfDiag.GetX();
	const float yMin = s_Default_BBHalfDiag.GetY();
	const float yMax = m_Owner->GetScreenHeight() - s_Default_BBHalfDiag.GetY();
	const Vector2D randomPos = Vector2D( GetRandomNumber( xMin, xMax ), GetRandomNumber( yMin, yMax ) );
	SetPosition( randomPos );

	// Set a velocity vector that has a random rotation and a random magnitude chosen within the default range.
	const float randomVelocityMagnitude = GetRandomNumber( s_Default_MinVelocity, s_Default_MaxVelocity );
	const float randomRotation = GetRandomNumber( 0.f, 2 * Vector2D::s_PI );
	Vector2D randomVelocity = Vector2D::s_UP;
	randomVelocity.Rotate( randomRotation ) *= randomVelocityMagnitude;
	SetVelocity( randomVelocity );
}

float Clock::GetRandomNumber( const float min, const float max ) const
{
	// Generate a random number
	std::random_device rd;
	std::mt19937 gen( rd() );
	std::uniform_real_distribution< float > distribution( min, max );
	return distribution( gen );
}

void Clock::Split()
{

}
