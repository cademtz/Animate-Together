/*	Description:
 *		Provides an abstract class that can handle events and listeners
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 3:13:30 PM
 */

#ifndef CEventHandler_H
#define CEventHandler_H
#ifdef _WIN32
#pragma once
#endif

#include <list>
#include <functional>

template <typename TEventClass, typename TCallback = void(TEventClass*)>
class CEventHandler
{
protected:
	typedef std::function<TCallback> Callback_t;
	typedef std::list<Callback_t> Listeners_t;

	static Listeners_t& Listeners()
	{
		static Listeners_t inst;
		return inst; // Too much hassle to initialize a private static member variable, so we make a singleton
	}

public:
	static void CreateEvent(TEventClass& Event) {
		for (auto fn : Listeners())
			fn(&Event);
	}

	// - Adds a callback to the listener list
	// - Listeners are called when a certain class's event is created
	static inline void Listen(Callback_t Func) { Listeners().push_back(Func); }
};

#endif // CEventHandler_H