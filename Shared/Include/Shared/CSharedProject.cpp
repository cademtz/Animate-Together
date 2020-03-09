/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:06:49 PM
 */

#include "CSharedProject.h"

CSharedProject::Listeners_t CSharedProject::m_listeners;

CSharedProject::CSharedProject() : m_root(this) {
}