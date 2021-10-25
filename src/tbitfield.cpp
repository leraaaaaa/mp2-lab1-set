// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw "negative len";
	BitLen = len;
	MemLen = BitLen / (sizeof(TELEM) * 8); 
	if ((BitLen % (sizeof(TELEM) * 8)) > 0) MemLen++;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0) throw "negative index";
	if (n > (BitLen - 1)) throw "too large index";
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0) throw "negative index";
	if (n > (BitLen - 1)) throw "too large index";
	return  (1 << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return (BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) throw "negative index";
	if (n > (BitLen - 1)) throw "too large index";
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) throw "negative index";
	if (n > (BitLen - 1)) throw "too large index";
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) throw "negative index";
	if (n > (BitLen - 1)) throw "too large index";
	int value = 0;
	if ((pMem[GetMemIndex(n)] & GetMemMask(n))) value = 1;
	return value;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	delete[] pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((BitLen == bf.BitLen))
	{
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
				return 0;
		return 1;
	}
	else
		return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return (!(*this == bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int Length;
	if (bf.BitLen > BitLen)
		Length = bf.BitLen;
	else
		Length = BitLen;
	TBitField a(Length);
	for (int i = 0; i < MemLen; i++)
		a.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		a.pMem[i] = a.pMem[i] | bf.pMem[i];
	return a;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (bf.BitLen > BitLen)
	{
		TBitField a(bf.BitLen);
		for (int i = 0; i < MemLen; i++)
			a.pMem[i] = pMem[i] & bf.pMem[i];
		return a;
	}
	else
	{
		TBitField a(BitLen);
		for (int i = 0; i < bf.MemLen; i++)
			a.pMem[i] = pMem[i] & bf.pMem[i];
		return a;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField a = (*this);
	for (int i = 0; i < BitLen; i++)
	{
		if (a.GetBit(i))
			a.ClrBit(i);
		else
			a.SetBit(i);
	}
	return a;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int value;
	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> value;
		if (value)
			bf.SetBit(i);
		else
			bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
