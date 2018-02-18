#pragma once
#include "stdafx.h"
#define ARR_OF_FST_WORD_SIZE 15
#define ARR_OF_FST_OP_SIZE 13
namespace FST
{
	enum LEX_TYPE { // ��� �������
		SYMB_NOT_FOUND = -1,	// -1 - ������
		SYMB_TABLE_ID = 0,		// 0 - ��� ������� ���������������
		SYMB_NEED_PRIORITY,		// 1 - ��� ���������� ����������
		SYMB_LEXEMS,			// 2 - "������� �������"
		SYMB_ARIPH,				// 3 - �������������� �����
		SYMB_STATIC_LIB,		// 4 - ������� ����������� ����������
		SYMB_LITERAL			// 5 - ��������
	};

	struct RELATION		 // �����: ������ -> ������� ����� �������� ��
	{
		char symbol;			// ������ ��������
		short nnode;			// ����� ������� �������
		RELATION(
			char c = 0x00,			// ������ ��������
			short ns = NULL			// ����� ���������
		);
	};

	struct NODE			// ������� ����� ���������
	{
		short n_relation;		// ���������� ����������� ����
		RELATION *relations;	// ����������� ����
		NODE();
		NODE(
			short n,				// ���������� ����������� ����
			RELATION rel, ...		// ������ ����
		);
	};

	struct STATE_MACHINE
	{
		LEX_TYPE lex_type;
		char lexema;
	};

	struct FST			// ������������������� �������� �������
	{
		char lexema;			// �������
		LEX_TYPE lex_type;		// ��� �������
		string str;				// ������� (������, ����������� 0�00)
		short position;			// ������� ������� � �������
		short nstates;			// ���������� ��������� ��������
		NODE* nodes;			// ���� ���������: [0] - ��������� ���������, [nstates - 1] - ��������
		short* rstates;			// ��������� ��������� �������� �� ������ �������
		FST();
		FST(
			char c,					// �������
			LEX_TYPE l_t,			// ��� �������
			string s,				// �������(������, ����������� 0�00)
			short ns,				// ���������� ��������� ��������
			NODE n, ...				// ������ ��������� (���� ���������)
		);
	};

	bool execute(		// ��������� ������������� �������
		FST& fst			// ������������������� �������� �������
	);
	FST* arr_of_fst_op();		// ������ ��������� ������-��������� ��������
	FST* arr_of_fst_word();		// ������ ��������� ������-����
}