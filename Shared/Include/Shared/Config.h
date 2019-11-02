/*	Description:
 *		Defaults and 
 *
 *	Author: Hold on!
 *	Created: 10/18/2019 5:41:27 PM
 */

#ifndef Config_H
#define Config_H
#ifdef _WIN32
#pragma once
#endif

// TO DO: Figure out CMake. I'm a complete Windows boomer, yeah, but it's a learning experience!
// Until then, this must be updated manually

#define AT_VERSION "1.4.1a"

#define AT_PROTO_PREFIX "AT Server"
#define AT_PROTO_MAJOR 0
#define AT_PROTO_MINOR 0

#define AT_DEFPORT 65000

#define AT_CFG "Config"
#define AT_CFG_SERVER AT_CFG "/Server"
#define AT_CFG_CLIENT AT_CFG "/Client"

#endif // Config_H