/*	Description:
 *		Holds the CBaseBrush class as well as all the global brushes
 *
 *	Author: Hold on!
 *	Created: 3/17/2019 3:36:43 PM
 */

#ifndef CBaseBrush_H
#define CBaseBrush_H
#ifdef _WIN32
#pragma once
#endif

#include <qbrush.h>
#include <list>
#include <functional>

namespace BrushesPrivate
{
	class CBaseBrush : public QBrush
	{

	public:
		const std::string& GetName() const { return name; }
		static void InitBrushes();

	protected:
		CBaseBrush();
		std::string name;
		static std::list<CBaseBrush*>* init_list;

		virtual void Init() = 0;
	};

	class CCheckerBrush : public CBaseBrush
	{
	protected:
		void Init();
	};
}

#endif // CBaseBrush_H