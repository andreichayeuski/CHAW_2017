#pragma once
#include "stdafx.h"

void CallPolishNotation(LT::LexTable* lextable, IT::IdTable* idtable);

bool PolishNotation( // ���������� �������� ������ � ������� ������
	int				lextable_pos,	// ������� ��������� � lextable
	LT::LexTable*	lextable,		// ������� ������
	IT::IdTable*	idtable			// ������� ���������������
);