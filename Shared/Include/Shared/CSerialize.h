/*	Description:
 *		Provides tools for serializing data
 *
 *	Author: Hold on!
 *	Created: 10/23/2019 7:36:03 PM
 */

#ifndef CSerialize_H
#define CSerialize_H
#ifdef _WIN32
#pragma once
#endif

#include <qstring.h>
#include <qendian.h>
#include "NetObjects/CNetObject.h"

struct SerialStream
{
	const char* data;
	int pos = 0;

	template<typename T>
	inline SerialStream& operator>>(T& Var);
};

class CSerialize
{
public:
	template<typename... VarArgs>
	CSerialize(const VarArgs&... Args) : CSerialize() {
		Add(Args...);
	}

	CSerialize(QByteArray Bytes = QByteArray(sizeof(int), 0)) : m_bytes(Bytes) { }

	inline const QByteArray Bytes() const { return m_bytes; }
	static SerialStream Stream(const char* Data) { return { Data }; }

	// - Sets instance's data to use existing serialized bytes
	inline void SetSerialized(QByteArray Bytes) {
		m_bytes = Bytes;
	}

	template<typename... VarArgs>
	inline void Deserialize(VarArgs&... Args) {
		Next(m_bytes.data() + sizeof(m_bytes.size()), Args...);
	}

	template<typename... VarArgs>
	static inline void Deserialize(const char* Data, VarArgs&... Args) {
		Next(Data, Args...);
	}

	template <typename T>
	static inline void FromStream(SerialStream& Stream, T& Data) {
		Next(Stream.pos(), Data);
	}

	template<typename T, typename... VarArgs>
	inline void Add(const T& First, const VarArgs... Args)
	{
		Add(First);
		Add(Args...);
	}

	// - Adds a QString as Utf8 text unless specified with 'Str.utf16'
	inline void Add(const QString& Str) { AddUtf(Str, false); }
	inline void Add(const char* Str) { AddUtf(QString::fromUtf8(Str), false); }
	inline void Add(const ushort* Str) { AddUtf(QString::fromUtf16(Str), true); }

	inline void Add(const bool& Val)
	{
		m_bytes.append(Val);
		UpdateLen();
	}

	template<typename T>
	inline void Add(const T& Val)
	{
		T big = qToBigEndian(Val);
		m_bytes.append((char*)&big, sizeof(T));
		UpdateLen();
	}

	inline void Add(const CSerialize& Data)
	{
		m_bytes.append(Data.Bytes());
		UpdateLen();
	}

	template<typename T>
	inline void Add(const QList<T>& List)
	{
		Add(List.size());
		for (T item : List)
			Add(item);
	}

	template<typename T>
	inline void Add(const CNetObject& Obj) { Add(Obj.Handle()); }

private:

	inline void Add() { }

	inline void AddUtf(const QString& Str, bool Utf16)
	{
		Add(Utf16); // Utf header
		Add(Str.size()); // Size header
		if (Utf16)
			m_bytes.append((char*)Str.utf16(), Str.size() * sizeof(char16_t)); // Keep as utf16
		else
			m_bytes.append(Str.toUtf8(), Str.size());
		UpdateLen();
	}

	template<typename T, typename... VarArgs>
	static inline void Next(const char*& Pos, T& First, VarArgs&... Args)
	{
		Next(Pos, First);
		Next(Pos, Args...);
	}
	
	static inline void Next(const char*& Pos, QString& Str)
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

	static inline void Next(const char*& Pos, CSerialize& Data)
	{
		int size;
		Next(Pos, size);
		Data = CSerialize(Pos, size);
		Pos += size;
	}

	static inline void Next(const char*& Pos, bool& Val) // qbswap likes char and uint8, but not bool apparently?
	{
		Val = *(bool*)Pos;
		Pos += sizeof(Val);
	}

	template<typename T>
	static inline void Next(const char*& Pos, QList<T> List)
	{
		int size;
		Next(Pos, size);
		for (int i = 0; i < size; i++)
		{
			T item;
			Next(Pos, item);
			List.append(item);
		}
	}

	static inline void Next(const char*& Pos, CNetObject& Obj)
	{
		unsigned handle;
		Next(Pos, handle);
		Obj = CNetObject(handle);
	}

	template<typename T>
	static inline void Next(const char*& Pos, T& Val)
	{
		Val = qFromBigEndian<T>(Pos);
		Pos += sizeof(Val);
	}

	static inline void Next(const char*& pos) { }

	inline void UpdateLen() {
		qToBigEndian(m_bytes.size(), m_bytes.data()); // Set size header
	}

	QByteArray m_bytes;
};

template<typename T>
inline SerialStream & SerialStream::operator>>(T & Var)
{
	CSerialize::FromStream(*this, Var);
	return *this;
}

#endif // CSerialize_H