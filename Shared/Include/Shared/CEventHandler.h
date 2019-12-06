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

	static inline Listeners_t& Listeners() { return m_listeners; }

private:
	static Listeners_t m_listeners;

public:
	static void CreateEvent(TEventClass& Event) {
		for (auto fn : Listeners())
			if (fn)
				fn(&Event);
	}

	// - Adds a callback and returns a handle used to later remove it
	// - Listeners are called when a certain class's event is created
	static unsigned Listen(Callback_t Func)
	{
		unsigned index = 0;
		for (auto& listener : m_listeners)
		{
			if (!listener)
			{
				listener = Func;
				return index;
			}
			index++;
		}

		m_listeners.push_back(Func);
		return index;
	}

	static void EndListen(unsigned Handle)
	{
		if (Handle == m_listeners.size() - 1)
			return m_listeners.pop_back();
		
		unsigned index = 0;
		for (auto& listener : m_listeners)
		{
			if (index == Handle)
			{
				listener = nullptr;
				return;
			}
			index++;
		}
	}
};

#endif // CEventHandler_H