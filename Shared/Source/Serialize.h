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

class CSerialize
{
public:
	template<typename... VarArgs>
	CSerialize(const VarArgs&... Args) : CSerialize() {
		Add(Args...);
	}

	CSerialize(QByteArray Bytes = QByteArray(sizeof(int), 0)) : m_bytes(Bytes) { }

	inline const QByteArray Bytes()
	{
		qToBigEndian(m_bytes.size(), m_bytes.data()); // Set size header
		return m_bytes;
	}

	// - Sets instance's data to use existing serialized bytes
	inline void SetSerialized(QByteArray Bytes) {
		m_bytes = Bytes;
	}

	template <typename... VarArgs>
	inline void Deserialize(VarArgs&... Args) {
		Next(m_bytes.data() + sizeof(m_bytes.size()), Args...);
	}

	template <typename... VarArgs>
	static inline void Deserialize(const char* Data, VarArgs&... Args) {
		Next(Data, Args...);
	}

	template<typename T, typename... VarArgs>
	inline void Add(const T& First, const VarArgs... Args)
	{
		Add(First);
		Add(Args...);
	}

	// - Adds a QString as Utf8 text unless specified as 'Str.toUtf16'
	inline void Add(const QString& Str) { AddUtf(Str, false); }
	inline void Add(const char* Str) { AddUtf(QString::fromUtf8(Str), false); }
	inline void Add(const char16_t* Str) { AddUtf(QString::fromUtf16(Str), true); }

	inline void Add(const bool& Val) {
		m_bytes.append(Val);
	}

	template<typename T>
	inline void Add(const T& Val)
	{
		T big = qToBigEndian(Val);
		m_bytes.append((char*)&big, sizeof(T));
	}

	inline void Add() { }

private:
	inline void AddUtf(const QString& Str, bool Utf16)
	{
		Add(Utf16); // Utf header
		Add(Str.size()); // Size header
		if (Utf16)
			m_bytes.append((char*)Str.utf16(), Str.size() * sizeof(char16_t)); // Keep as utf16
		else
			m_bytes.append(Str.toUtf8(), Str.size());
	}

	template<typename T, typename... VarArgs>
	static inline void Next(const char* Pos, T& First, VarArgs&... Args)
	{
		Next(Pos, First);
		Next(Pos, Args...);
	}
	
	static inline void Next(const char* Pos, QString& Str)
	{
		bool utf16;
		int size;
		Next(Pos, utf16);
		Next(Pos, size);
		if (utf16)
			Str = QString::fromUtf16((const char16_t*)Pos, size);
		else
			Str = QString::fromUtf8(Pos, size);
		Pos += size * (utf16 ? sizeof(char16_t) : sizeof(char));
	}

	static inline void Next(const char* Pos, bool& Val) // qbswap likes char and uint8, but not bool apparently?
	{
		Val = *(bool*)Pos;
		Pos += sizeof(Val);
	}

	template<typename T>
	static inline void Next(const char* Pos, T& Val)
	{
		Val = qFromBigEndian<T>(Pos);
		Pos += sizeof(Val);
	}

	static inline void Next(const char* pos) { }

	QByteArray m_bytes;
};

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

template <class P>
inline void NextData(const bool& Src, P& DestPos)
{
	*(bool*)DestPos = Src;
	DestPos += sizeof(bool);
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
	Dest = QString::fromUtf16((const ushort*)SrcPos, Length);
	SrcPos += Length * sizeof(QChar);
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