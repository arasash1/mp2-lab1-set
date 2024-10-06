// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{

}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
	return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	if (Elem < 0 || Elem >= MaxPower)
	{
		throw "Out of range";
	}

	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	if (Elem < 0 || Elem >= MaxPower)
	{
		throw "Out of range";
	}

	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	if (Elem < 0 || Elem >= MaxPower)
	{
		throw "Out of range";
	}

	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
	if (this != &s)
	{
		BitField = s.BitField;
		MaxPower = s.MaxPower;
	}

	return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
	return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
	return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
	int len = 0;
	if (MaxPower > s.MaxPower)
	{
		len = MaxPower;
	}
	else
	{
		len = s.MaxPower;
	}
	TSet res(len);

	res.BitField = BitField | s.BitField;
	return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	if (Elem < 0 || Elem >= MaxPower)
	{
		throw "Out of range";
	}

	TSet res(*this);
	res.InsElem(Elem);
	return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	if (Elem < 0 || Elem >= MaxPower)
	{
		throw "Out of range";
	}

	TSet res(*this);
	res.DelElem(Elem);
	return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
	int len = 0;
	if (MaxPower < s.MaxPower)
	{
		len = MaxPower;
	}
	else
	{
		len = s.MaxPower;
	}
	TSet res(len);

	res.BitField = BitField & s.BitField;
	return res;
}

TSet TSet::operator~(void) // дополнение
{
	TSet res(MaxPower);

	res.BitField = ~BitField;
	return res;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
	char data;
	for (int i = 0; i < s.BitField.GetLength(); i++)
	{
		istr >> data;
		if (data == '1')
		{
			s.BitField.SetBit(i);
		}
		else if (data == '0')
		{
			s.BitField.ClrBit(i);
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
	for (int i = 0; i < s.BitField.GetLength(); i++)
	{
		ostr << s.BitField.GetBit(i);
	}
	return ostr;
}
