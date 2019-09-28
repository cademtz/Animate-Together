/*	Description:
 *		Apple
 *
 *	Author: Hold on!
 *	Created: 9/27/2019 7:16:58 PM
 */

#ifndef COnionSkin_H
#define COnionSkin_H
#ifdef _WIN32
#pragma once
#endif

#define APPLE APPLE

class COnionSkin
{
	static bool m_enabled;
	static int m_back, m_fwd;

	static void CreateEvent();

public:
	static inline bool Enabled() { return m_enabled; }
	static void Enable(bool Yes);

	// - Distance backwards to display frames
	static inline int Back() { return m_back; }
	static void SetBack(int Back);

	// - Distance forwards to display frames
	static inline int Forward() { return m_fwd; }
	static void SetForward(int Fwd);
};

#endif // COnionSkin_H