#pragma once
#include "stdafx.h"
#define ARR_OF_FST_WORD_SIZE 15
#define ARR_OF_FST_OP_SIZE 13
namespace FST
{
	enum LEX_TYPE { // тип лексемы
		SYMB_NOT_FOUND = -1,	// -1 - ошибка
		SYMB_TABLE_ID = 0,		// 0 - дл€ таблицы идентификаторов
		SYMB_NEED_PRIORITY,		// 1 - дл€ присвоени€ приоритета
		SYMB_LEXEMS,			// 2 - "обычные лексемы"
		SYMB_ARIPH,				// 3 - арифметические знаки
		SYMB_STATIC_LIB,		// 4 - функции статической библиотеки
		SYMB_LITERAL			// 5 - литералы
	};

	struct RELATION		 // ребро: символ -> вершина графа перехода  ј
	{
		char symbol;			// символ перехода
		short nnode;			// номер смежной вершины
		RELATION(
			char c = 0x00,			// символ перехода
			short ns = NULL			// новое состо€ние
		);
	};

	struct NODE			// вершина графа переходов
	{
		short n_relation;		// количество инцидентных рЄбер
		RELATION *relations;	// инцидентные рЄбра
		NODE();
		NODE(
			short n,				// количество инцидентных рЄбер
			RELATION rel, ...		// список рЄбер
		);
	};

	struct STATE_MACHINE
	{
		LEX_TYPE lex_type;
		char lexema;
	};

	struct FST			// недетерминированный конечный автомат
	{
		char lexema;			// лексема
		LEX_TYPE lex_type;		// тип лексемы
		string str;				// цепочка (строка, завершаетс€ 0х00)
		short position;			// текуща€ позици€ в цепочке
		short nstates;			// количество состо€ний автомата
		NODE* nodes;			// граф переходов: [0] - начальное состо€ние, [nstates - 1] - конечное
		short* rstates;			// возможные состо€ни€ автомата на данной позиции
		FST();
		FST(
			char c,					// лексема
			LEX_TYPE l_t,			// тип лексемы
			string s,				// цепочка(строка, завершаетс€ 0х00)
			short ns,				// количество состо€ний автомата
			NODE n, ...				// список состо€ний (граф переходов)
		);
	};

	bool execute(		// выполнить распознавание цепочки
		FST& fst			// недетерминированный конечный автомат
	);
	FST* arr_of_fst_op();		// массив автоматов лексем-одиночных символов
	FST* arr_of_fst_word();		// массив автоматов лексем-слов
}