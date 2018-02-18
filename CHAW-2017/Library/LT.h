#pragma once
#include "Log.h"
#define LT_NOPRIORITY 0xffffffff		// ��� ����������
#define LEXEMA_FIXZISE 1				// ������������� ������ �������
#define LT_MAXSIZE 4096					// ������������ ���-�� ����� � ������� ������
#define TL_TI_NULLIDX 0xffffffff		// ��� �������� ������� ���������������
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
#define LEX_ID			'i'				//	�������������
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
{										// ������� ������
	struct Entry
	{
		char lexema;						// �������
		int sn;								// ����� ������ � �������� ����
		int idxTI;							// ������ � ������� ��������������� 
		short priority;						// ��������� ��� ��������
	};

	struct LexTable
	{
		int maxsize;						// ������� ������� ������ 
		int size;							// ������� ������ ������� ������
		Entry* table;						// ������ ����� ������� ������	
	};

	LexTable Create(						// ������� ������
		int size							// �������
	);

	Entry GetEntry(LexTable& lextable, int n);

	Entry Add(								// �������� ������ ������� ������
		LexTable& lextable,					// ��������� �������
		Entry entry							// ������ ������ ������
	);

	void Delete(LexTable& lextable);		// ������� ������� ������
	void ShowTable(Log::LOG log, LexTable& lexTable);
}