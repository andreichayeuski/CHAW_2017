#pragma once
#include "stdafx.h"

void CallPolishNotation(LT::LexTable* lextable, IT::IdTable* idtable);

bool PolishNotation( // построение польской записи в таблице лексем
	int				lextable_pos,	// позиция выражения в lextable
	LT::LexTable*	lextable,		// таблица лексем
	IT::IdTable*	idtable			// таблица идентификаторов
);