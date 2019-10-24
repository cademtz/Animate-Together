/*	Description:
 *		Provides tools for serializing data
 *
 *	Author: Hold on!
 *	Created: 10/23/2019 7:36:03 PM
 */

#ifndef Serialize_H
#define Serialize_H
#ifdef _WIN32
#pragma once
#endif

#include <cstring>
#include <qstring.h>
#include <qendian.h>

template <class P>
inline void NextData(const char* Src, P& DestPos)
{
	size_t len = strlen(Src) + 1;
	memcpy(DestPos, Src, len);
	DestPos += len;
}

template <class P>
inline void NextData(const QString& Src, P& DestPos)
{
	size_t size = Src.size() * sizeof(QChar);
	memcpy(DestPos, Src.data(), size);
	DestPos += size;
}

template <class D, class P>
inline void NextData(const D& Src, P& DestPos)
{
	qToBigEndian(Src, DestPos);
	DestPos += sizeof(D);
}

template <class P>
inline void GetData(char* Dest, size_t Size, const P& SrcPos)
{
	strcpy_s(Dest, Size, (const char*)SrcPos);
	SrcPos += strlen(Dest) + 1;
}

template <size_t N, class P>
inline void GetData(char(&Dest)[N], P& SrcPos)
{
	strcpy_s(Dest, (const char*)SrcPos);
	SrcPos += N;
}

template <class P>
inline void GetData(QString& Dest, size_t Length, P& SrcPos)
{
	size_t size = Length * sizeof(QChar);
	Dest = QString::fromUtf16((const ushort*)SrcPos, size);
	SrcPos += size;
}

template <class D, class P>
inline void GetData(D& Dest, P& SrcPos)
{
	Dest = qFromBigEndian<D>(SrcPos);
	SrcPos += sizeof(D);
}

struct CalcSize
{
	unsigned len;

	inline operator unsigned() const { return len; }

	inline CalcSize& operator+(CalcSize other)
	{
		len += other;
		return *this;
	}

	inline CalcSize& operator+(const char* Str)
	{
		len += strlen(Str) + 1;
		return *this;
	}

	inline CalcSize& operator+(const QString& other)
	{
		len += other.size() * sizeof(QChar);
		return *this;
	}

	template <class T>
	inline CalcSize& operator+(const T&)
	{
		len += sizeof(T);
		return *this;
	}

	CalcSize() : len(0) { }
};

#endif // Serialize_H