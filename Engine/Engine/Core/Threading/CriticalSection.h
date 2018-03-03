#pragma once
//---------------------------------------------------------------------------//
//        Copyright 2016  Immersive Pixels. All Rights Reserved.			 //

#include <Windows.h>

//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Win32 implementation of a critical section. This is used in order to control
// concurrent access to globally used resources!
//-----------------------------------------------------------------------------
class CCriticalSection
{
public:
	//-----------------------------------------------------------------------------
	// Creates a new instance of the windows 32 critical section implementation
	// and initializes it.
	//-----------------------------------------------------------------------------
	CCriticalSection::CCriticalSection( void )
	{ 
		InitializeCriticalSection( &mCS );
		EnterCriticalSection(&mCS);
	}

	//-----------------------------------------------------------------------------
	// Deletes the critical section again.
	//-----------------------------------------------------------------------------
	virtual CCriticalSection::~CCriticalSection( void )
	{ 
		LeaveCriticalSection(&mCS);
		DeleteCriticalSection ( &mCS );
	}

	//-----------------------------------------------------------------------------
	// Will lock the current section to the thread calling this method!
	//-----------------------------------------------------------------------------
	void CCriticalSection::Lock ( void )
	{
		EnterCriticalSection(&mCS);
	}

	//-----------------------------------------------------------------------------
	// Will unlock the current section to the thread calling this method!
	//-----------------------------------------------------------------------------
	void CCriticalSection::Unlock ( void )
	{
		LeaveCriticalSection ( &mCS );
	}
private:
	//-------------------------------------------------------------------------
    // Private Members
    //-------------------------------------------------------------------------
	mutable CRITICAL_SECTION mCS;
};