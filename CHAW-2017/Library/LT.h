#pragma once
#include "Log.h"
#define LT_NOPRIORITY 0xffffffff		// нет приоритета
#define LEXEMA_FIXZISE 1				// фиксированный размер лексемы
#define LT_MAXSIZE 4096					// максимальное кол-во строк в таблице лексем
#define TL_TI_NULLIDX 0xffffffff		// нет элемента таблицы идентификаторов
#define LEX_T			't'				//	
#define LEX_DIG			'd'				//	dig
#define LEX_CHR			'c'				//	chr
#define LEX_VAR			'v'				//	var
#define LEX_FUNC		'f'				//	func
#define LEX_BODY		'b'				//	body
#define LEX_RET			'r'				//	ret

#define LEX_IF			'q'				//	if
#define LEX_THEN		'1'				//	then
#define LEX_ELSE		'2'				//	else
#define LEX_OUT			'o'				//	out
#define LEX_POW			'p'				//	pow
#define LEX_STRL		's'				//	strl
#define LEX_ID			'i'				//	идентификатор
#define LEX_ARIPH		'a'
#define LEX_SEMICOLON	';'				//	;
#define LEX_COLON		':'				//	:
#define LEX_COMMA		','				//	,
#define LEX_LEFTHESIS	'('				//	(
#define LEX_RIGHTHESIS	')'				//	)
#define LEX_EQUAL		'='				//  =
#define LEX_LEFTBRACE	'{'				//	{
#define LEX_RIGHTBRACE	'}'				//	}
#define LEX_LESS		'<'				//	<
#define LEX_GREAT		'>'				//	>
#define LEX_PLUS		'+'				//	+
#define LEX_MINUS		'-'				//	-
#define LEX_STAR		'*'				//	*

#define LEX_SUBST		'@'
#define LEX_L			'L'				//  L
namespace LT
{										// таблица лексем
	struct Entry
	{
		char lexema;						// лексема
		int sn;								// номер строки в исходном коде
		int idxTI;							// индекс в таблице идентификаторов 
		short priority;						// приоритет дл€ операций
	};

	struct LexTable
	{
		int maxsize;						// емкость таблицы лексем 
		int size;							// текущий размер таблицы лексем
		Entry* table;						// массив строк таблицы лексем	
	};

	LexTable Create(						// таблица лексем
		int size							// емкость
	);

	Entry GetEntry(LexTable& lextable, int n);

	Entry Add(								// получить строку таблицы лексем
		LexTable& lextable,					// экземпл€р таблицы
		Entry entry							// строка таблцы лексем
	);

	void Delete(LexTable& lextable);		// удалить таблицу лексем
	void ShowTable(Log::LOG log, LexTable& lexTable);
}