#pragma once

///////////////////////////  I N C L U D E S  ////////////////////////////

// System.
#include <memory>
#include <vector>

// Local.
#include "Entity.h"

////////////////  F O R W A R D  D E C L A R A T I O N S  ////////////////

class Cannon;

//////////////////  C L A S S  D E C L A R A T I O N S  //////////////////

class C_Application
{
public:
	
	typedef unsigned int T_PressedKey;

	static const T_PressedKey s_KeyLeft;
	static const T_PressedKey s_KeyUp;
	static const T_PressedKey s_KeyRight;
	static const T_PressedKey s_KeyDown;
	static const T_PressedKey s_KeySpace;

	// Colors
	static const unsigned int s_Color_Red;
	static const unsigned int s_Color_Green;
	static const unsigned int s_Color_Blue;
	static const unsigned int s_Color_Black;
	static const unsigned int s_Color_White;

	C_Application( const float screenWidth, const float screenHeight );
	~C_Application() {}

	// Tick is called on fixed framerate (50fps).
	void Tick( const T_PressedKey pressedKeys, const float deltaTime );

	float GetScreenWidth() const { return m_ScreenWidth; }
	float GetScreenHeight() const { return m_ScreenHeight; }

	// TODO: Implement these..
	// SpawnEntity()
	// DestroyEntity()

private:

	void ProcessInput( const T_PressedKey pressedKeys );
	void ClearScreen();
	// Pretty trivial collision check, as I don't have time to do something more efficient. Interesting ideas
	// could have been quad trees to partition the space and optimize the computation for larger entity pools.
	void CheckCollisions();

	const float m_ScreenWidth;
	const float m_ScreenHeight;
	const int m_ScreenBackgroundColor;

	float m_DeltaTime;

	Cannon* m_Cannon;
	std::vector< std::unique_ptr< Entity > > m_Entities;
};
