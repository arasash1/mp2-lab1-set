// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
	if (len < 0)
		throw out_of_range("len of BitField < 0");
	BitLen = len;
	MemLen = (BitLen + BITS_IN_ONE_MEM - 1) / BITS_IN_ONE_MEM; // вычисляем количество элементов массива
	pMem = new TELEM[MemLen]; // выделяем память под массив
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{

	return n / BITS_IN_ONE_MEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{

	return 1 << (n % BITS_IN_ONE_MEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen))
		throw out_of_range("bit pos is out of range");
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen))
		throw out_of_range("bit pos is out of range");
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))
		throw out_of_range("bit pos is out of range");
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField & bf) // присваивание
{
	if (this != &bf)
	{
		if (MemLen != bf.MemLen)
		{
			TELEM* newMem = new TELEM[bf.MemLen];
			delete[] pMem;
			pMem = newMem;
			MemLen = bf.MemLen;
		}

		BitLen = bf.BitLen;

		
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}

	return *this;
}

int TBitField::operator==(const TBitField & bf) const // сравнение
{
	if (BitLen != bf.BitLen)
	{
		return 0;
	}

	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	}

	return 1;
}

int TBitField::operator!=(const TBitField & bf) const // сравнение
{
	if (BitLen != bf.BitLen)
	{
		return 1;
	}

	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 1;
		}
	}

	return 0;
}

TBitField TBitField::operator|(const TBitField & bf) // операция "или"
{
	int len = 0;
	if (BitLen > bf.BitLen)
	{
		len = BitLen;
	}
	else
	{
		len = bf.BitLen;
	}
	TBitField bfres(len);

	int MinMemLen = 0;
	if (MemLen < bf.MemLen)
	{
		MinMemLen = MemLen;
	}
	else
	{
		MinMemLen = bf.MemLen;
	}

	for (int i = 0; i < MinMemLen; i++)
	{
		bfres.pMem[i] = pMem[i] | bf.pMem[i];
	}

	for (int i = MinMemLen; i < MemLen; i++)
	{
		bfres.pMem[i] = pMem[i]; // Вставляем оставшиеся биты из текущего объекта
	}

	for (int i = MinMemLen; i < bf.MemLen; i++)
	{
		bfres.pMem[i] = bf.pMem[i]; // Вставляем оставшиеся биты из bf
	}

	return bfres;
}

TBitField TBitField::operator&(const TBitField & bf) // операция "и"
{
	int len = 0;
	if (BitLen > bf.BitLen)
	{
		len = BitLen;
	}
	else
	{
		len = bf.BitLen;
	}
	TBitField bfres(len);

	int MinMemLen = 0;
	if (MemLen < bf.MemLen)
	{
		MinMemLen = MemLen;
	}
	else
	{
		MinMemLen = bf.MemLen;
	}
	
	for (int i = 0; i < MinMemLen; i++)
	{
		bfres.pMem[i] = pMem[i] & bf.pMem[i];
	}

	return bfres;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (this->GetBit(i))
		{
			res.ClrBit(i);
		}
		else
		{
			res.SetBit(i);
		}
	}

	return res;
}

// ввод/вывод

istream& operator>>(istream & istr, TBitField & bf) // ввод
{
	char data;
	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> data;
		if (data == '1')
		{
			bf.SetBit(i);
		}
		else if (data == '0')
		{
			bf.ClrBit(i);
		}
	}
	return istr;
}

ostream& operator<<(ostream & ostr, const TBitField & bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
