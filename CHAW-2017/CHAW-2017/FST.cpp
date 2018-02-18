#include "stdafx.h"
#include "FST.h"
namespace FST
{
	bool step(FST& fst, short* &rstates)	//один шаг автомата
	{
		bool rc = false;
		swap(rstates, fst.rstates);	//смена массивов
		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					if (fst.nodes[i].relations[j].symbol == fst.str[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short)*fst.nstates);
		short lstring = fst.str.size();
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;				//продвинули позицию
			rc = step(fst, rstates);	//один шаг автомата
		};
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}

	RELATION::RELATION(char c, short nn)
	{
		symbol = c;
		nnode = nn;
	};

	NODE::NODE()	//по умолчанию
	{
		n_relation = 0;
		RELATION *relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...)	//с параметрами
	{
		n_relation = n;
		RELATION *p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++)
		{
			relations[i] = p[i];
		}
	};

	FST::FST(char c, LEX_TYPE l_t, string s, short ns, NODE n, ...)
	{
		lexema = c;
		str = s;
		nstates = ns;
		lex_type = l_t;
		nodes = new NODE[ns];
		NODE *p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short)*nstates);
		rstates[0] = 0;
		position = -1;
	};
	FST::FST()
	{}

	FST* arr_of_fst_op()
	{
		FST fstAriph(LEX_ARIPH, SYMB_ARIPH, "", 2, NODE(3, RELATION('+', 1), RELATION('-', 1), RELATION('*', 1)), \
			NODE()
		);
		FST fstLeftHesis(LEX_LEFTHESIS, SYMB_NEED_PRIORITY, "", 2, NODE(1, RELATION('(', 1)), \
			NODE()
		);
		FST fstRightHesis(LEX_RIGHTHESIS, SYMB_NEED_PRIORITY, "", 2, NODE(1, RELATION(')', 1)), \
			NODE()
		);
		FST fstEqual(LEX_EQUAL, SYMB_LEXEMS, "", 2, NODE(1, RELATION('=', 1)), \
			NODE()
		);
		FST fstLeftBrace(LEX_LEFTBRACE, SYMB_LEXEMS, "", 2, NODE(1, RELATION('{', 1)), \
			NODE()
		);
		FST fstRightBrace(LEX_RIGHTBRACE, SYMB_LEXEMS, "", 2, NODE(1, RELATION('}', 1)), \
			NODE()
		);
		FST fstSemicolon(LEX_SEMICOLON, SYMB_LEXEMS, "", 2, NODE(1, RELATION(';', 1)), \
			NODE()
		);
		FST fstColon(LEX_COLON, SYMB_LEXEMS, "", 2, NODE(1, RELATION(':', 1)), \
			NODE()
		);
		FST fstComma(LEX_COMMA, SYMB_NEED_PRIORITY, "", 2, NODE(1, RELATION(',', 1)), \
			NODE()
		);
		FST fstLess(LEX_LESS, SYMB_LEXEMS, "", 2, NODE(1, RELATION('<', 1)), \
			NODE()
		);
		FST fstGreat(LEX_GREAT, SYMB_LEXEMS, "", 2, NODE(1, RELATION('>', 1)), \
			NODE()
		);
		FST fstDigLit(LEX_DIG, SYMB_LITERAL, "", 2, NODE(10,
			RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
			RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
			RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
			RELATION('9', 1)),
			NODE()
		);
		FST fstId(LEX_ID, SYMB_TABLE_ID, "", 2, NODE(26, RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
			RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
			RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
			RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
			RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
			RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
			RELATION('y', 1), RELATION('z', 1)),
			NODE()
		);
		FST* result = new FST[ARR_OF_FST_OP_SIZE];
		result[0] = fstAriph;				//
		result[1] = fstSemicolon;			//
		result[2] = fstColon;				//
		result[3] = fstComma;				//
		result[4] = fstLeftHesis;			//
		result[5] = fstRightHesis;			//
		result[6] = fstEqual;				//
		result[7] = fstLeftBrace;			//	
		result[8] = fstRightBrace;			//
		result[9] = fstLess;
		result[10] = fstGreat;
		result[11] = fstDigLit;
		result[12] = fstId;
		return result;
	}
	FST* arr_of_fst_word()
	{
		FST fstDig(LEX_DIG, SYMB_LEXEMS, "", 4, NODE(1, RELATION('d', 1)),
			NODE(1, RELATION('i', 2)),
			NODE(1, RELATION('g', 3)),
			NODE()
		);
		FST fstChr(LEX_CHR, SYMB_LEXEMS, "", 4, NODE(1, RELATION('c', 1)),
			NODE(1, RELATION('h', 2)),
			NODE(1, RELATION('r', 3)),
			NODE()
		);
		FST fstFunc(LEX_FUNC, SYMB_LEXEMS, "", 5, NODE(1, RELATION('f', 1)),
			NODE(1, RELATION('u', 2)),
			NODE(1, RELATION('n', 3)),
			NODE(1, RELATION('c', 4)),
			NODE()
		);
		FST fstVar(LEX_VAR, SYMB_LEXEMS, "", 4, NODE(1, RELATION('v', 1)),
			NODE(1, RELATION('a', 2)),
			NODE(1, RELATION('r', 3)),
			NODE()
		);
		FST fstBody(LEX_BODY, SYMB_LEXEMS, "", 5, NODE(1, RELATION('b', 1)),
			NODE(1, RELATION('o', 2)),
			NODE(1, RELATION('d', 3)),
			NODE(1, RELATION('y', 4)),
			NODE()
		);
		FST fstRet(LEX_RET, SYMB_LEXEMS, "", 4, NODE(1, RELATION('r', 1)),
			NODE(1, RELATION('e', 2)),
			NODE(1, RELATION('t', 3)),
			NODE()
		);
		FST fstDigLit(LEX_DIG, SYMB_LITERAL, "", 2, NODE(20, RELATION('0', 0), RELATION('1', 0), RELATION('2', 0),
			RELATION('3', 0), RELATION('4', 0), RELATION('5', 0),
			RELATION('6', 0), RELATION('7', 0), RELATION('8', 0),
			RELATION('9', 0),
			RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
			RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
			RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
			RELATION('9', 1)),
			NODE()
		);
		FST fstId(LEX_ID, SYMB_TABLE_ID, "", 2, NODE(52, RELATION('a', 0), RELATION('b', 0), RELATION('c', 0), RELATION('d', 0),
			RELATION('e', 0), RELATION('f', 0), RELATION('g', 0), RELATION('h', 0),
			RELATION('i', 0), RELATION('j', 0), RELATION('k', 0), RELATION('l', 0),
			RELATION('m', 0), RELATION('n', 0), RELATION('o', 0), RELATION('p', 0),
			RELATION('q', 0), RELATION('r', 0), RELATION('s', 0), RELATION('t', 0),
			RELATION('u', 0), RELATION('v', 0), RELATION('w', 0), RELATION('x', 0),
			RELATION('y', 0), RELATION('z', 0),
			RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
			RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
			RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
			RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
			RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
			RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
			RELATION('y', 1), RELATION('z', 1)),
			NODE()
		);
		FST fstStrLit(LEX_CHR, SYMB_LITERAL, "", 4, NODE(2, RELATION('"', 1), RELATION('"', 2)), \
			NODE(276, \
				RELATION('A', 1), RELATION('B', 1), RELATION('C', 1), RELATION('D', 1), RELATION('E', 1), RELATION('F', 1), \
				RELATION('G', 1), RELATION('H', 1), RELATION('I', 1), RELATION('J', 1), RELATION('K', 1), RELATION('L', 1), \
				RELATION('M', 1), RELATION('N', 1), RELATION('O', 1), RELATION('P', 1), RELATION('Q', 1), RELATION('R', 1), \
				RELATION('S', 1), RELATION('T', 1), RELATION('U', 1), RELATION('V', 1), RELATION('W', 1), RELATION('X', 1), \
				RELATION('Y', 1), RELATION('Z', 1), \
				RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION('e', 1), RELATION('f', 1), \
				RELATION('g', 1), RELATION('h', 1), RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1), \
				RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1), RELATION('q', 1), RELATION('r', 1), \
				RELATION('s', 1), RELATION('t', 1), RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1), \
				RELATION('y', 1), RELATION('z', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), \
				RELATION('5', 1), RELATION('6', 1), RELATION('7', 1), RELATION('8', 1), RELATION('9', 1), RELATION('0', 1), \
				\
				RELATION('А', 1), RELATION('Б', 1), RELATION('В', 1), RELATION('Г', 1), RELATION('Д', 1), RELATION('Е', 1), \
				RELATION('Ё', 1), RELATION('Ж', 1), RELATION('З', 1), RELATION('И', 1), RELATION('Й', 1), RELATION('К', 1), \
				RELATION('Л', 1), RELATION('М', 1), RELATION('Н', 1), RELATION('О', 1), RELATION('П', 1), RELATION('Р', 1), \
				RELATION('С', 1), RELATION('Т', 1), RELATION('У', 1), RELATION('Ф', 1), RELATION('Х', 1), RELATION('Ц', 1), \
				RELATION('Ч', 1), RELATION('Ш', 1), RELATION('Щ', 1), RELATION('Ъ', 1), RELATION('Ы', 1), RELATION('Ь', 1), \
				RELATION('Э', 1), RELATION('Ю', 1), RELATION('Я', 1), \
				RELATION('а', 1), RELATION('б', 1), RELATION('в', 1), RELATION('г', 1), RELATION('д', 1), RELATION('е', 1), \
				RELATION('ё', 1), RELATION('ж', 1), RELATION('з', 1), RELATION('и', 1), RELATION('й', 1), RELATION('к', 1), \
				RELATION('л', 1), RELATION('м', 1), RELATION('н', 1), RELATION('о', 1), RELATION('п', 1), RELATION('р', 1), \
				RELATION('с', 1), RELATION('т', 1), RELATION('у', 1), RELATION('ф', 1), RELATION('х', 1), RELATION('ц', 1), \
				RELATION('ч', 1), RELATION('ш', 1), RELATION('щ', 1), RELATION('ъ', 1), RELATION('ы', 1), RELATION('ь', 1), \
				RELATION('э', 1), RELATION('ю', 1), RELATION('я', 1), RELATION(' ', 1), RELATION('.', 1), RELATION(',', 1), \
				RELATION('?', 1), RELATION('!', 1), RELATION(';', 1), RELATION(':', 1), RELATION('-', 1), RELATION(')', 1), \
				RELATION('(', 1), \
				\
				RELATION('A', 2), RELATION('B', 2), RELATION('C', 2), RELATION('D', 2), RELATION('E', 2), RELATION('F', 2), \
				RELATION('G', 2), RELATION('H', 2), RELATION('I', 2), RELATION('J', 2), RELATION('K', 2), RELATION('L', 2), \
				RELATION('M', 2), RELATION('N', 2), RELATION('O', 2), RELATION('P', 2), RELATION('Q', 2), RELATION('R', 2), \
				RELATION('S', 2), RELATION('T', 2), RELATION('U', 2), RELATION('V', 2), RELATION('W', 2), RELATION('X', 2), \
				RELATION('Y', 2), RELATION('Z', 2), \
				RELATION('a', 2), RELATION('b', 2), RELATION('c', 2), RELATION('d', 2), RELATION('e', 2), RELATION('f', 2), \
				RELATION('g', 2), RELATION('h', 2), RELATION('i', 2), RELATION('j', 2), RELATION('k', 2), RELATION('l', 2), \
				RELATION('m', 2), RELATION('n', 2), RELATION('o', 2), RELATION('p', 2), RELATION('q', 2), RELATION('r', 2), \
				RELATION('s', 2), RELATION('t', 2), RELATION('u', 2), RELATION('v', 2), RELATION('w', 2), RELATION('x', 2), \
				RELATION('y', 2), RELATION('z', 2), RELATION('1', 2), RELATION('2', 2), RELATION('3', 2), RELATION('4', 2), \
				RELATION('5', 2), RELATION('6', 2), RELATION('7', 2), RELATION('8', 2), RELATION('9', 2), RELATION('0', 2), \
				\
				RELATION('А', 2), RELATION('Б', 2), RELATION('В', 2), RELATION('Г', 2), RELATION('Д', 2), RELATION('Е', 2), \
				RELATION('Ё', 2), RELATION('Ж', 2), RELATION('З', 2), RELATION('И', 2), RELATION('Й', 2), RELATION('К', 2), \
				RELATION('Л', 2), RELATION('М', 2), RELATION('Н', 2), RELATION('О', 2), RELATION('П', 2), RELATION('Р', 2), \
				RELATION('С', 2), RELATION('Т', 2), RELATION('У', 2), RELATION('Ф', 2), RELATION('Х', 2), RELATION('Ц', 2), \
				RELATION('Ч', 2), RELATION('Ш', 2), RELATION('Щ', 2), RELATION('Ъ', 2), RELATION('Ы', 2), RELATION('Ь', 2), \
				RELATION('Э', 2), RELATION('Ю', 2), RELATION('Я', 2), \
				RELATION('а', 2), RELATION('б', 2), RELATION('в', 2), RELATION('г', 2), RELATION('д', 2), RELATION('е', 2), \
				RELATION('ё', 2), RELATION('ж', 2), RELATION('з', 2), RELATION('и', 2), RELATION('й', 2), RELATION('к', 2), \
				RELATION('л', 2), RELATION('м', 2), RELATION('н', 2), RELATION('о', 2), RELATION('п', 2), RELATION('р', 2), \
				RELATION('с', 2), RELATION('т', 2), RELATION('у', 2), RELATION('ф', 2), RELATION('х', 2), RELATION('ц', 2), \
				RELATION('ч', 2), RELATION('ш', 2), RELATION('щ', 2), RELATION('ъ', 2), RELATION('ы', 2), RELATION('ь', 2), \
				RELATION('э', 2), RELATION('ю', 2), RELATION('я', 2), RELATION(' ', 2), RELATION('.', 2), RELATION(',', 2), \
				RELATION('?', 2), RELATION('!', 2), RELATION(';', 2), RELATION(':', 2), RELATION('-', 2), RELATION(')', 2), \
				RELATION('(', 2)), \
			\
			NODE(1, RELATION('"', 3)), \
			NODE()
		);
		FST fstIf(LEX_IF, SYMB_LEXEMS, "", 3, NODE(1, RELATION('i', 1)),
			NODE(1, RELATION('f', 2)),
			NODE()
		);
		FST fstThen(LEX_THEN, SYMB_LEXEMS, "", 5, NODE(1, RELATION('t', 1)), \
			NODE(1, RELATION('h', 2)), \
			NODE(1, RELATION('e', 3)), \
			NODE(1, RELATION('n', 4)), \
			NODE()
		);
		FST fstElse(LEX_ELSE, SYMB_LEXEMS, "", 5, NODE(1, RELATION('e', 1)), \
			NODE(1, RELATION('l', 2)), \
			NODE(1, RELATION('s', 3)), \
			NODE(1, RELATION('e', 4)), \
			NODE()
		);
		FST fstOut(LEX_OUT, SYMB_STATIC_LIB, "", 4, NODE(1, RELATION('o', 1)), \
			NODE(1, RELATION('u', 2)), \
			NODE(1, RELATION('t', 3)), \
			NODE()
		);
		FST fstPower(LEX_POW, SYMB_STATIC_LIB, "", 6, NODE(1, RELATION('p', 1)), \
			NODE(1, RELATION('o', 2)), \
			NODE(1, RELATION('w', 3)), \
			NODE(1, RELATION('e', 4)), \
			NODE(1, RELATION('r', 5)), \
			NODE()
		);
		FST fstStrl(LEX_STRL, SYMB_STATIC_LIB, "", 5, NODE(1, RELATION('s', 1)), \
			NODE(1, RELATION('t', 2)), \
			NODE(1, RELATION('r', 3)), \
			NODE(1, RELATION('l', 4)), \
			NODE()
		);
		FST* result = new FST[ARR_OF_FST_WORD_SIZE];
		result[0] = fstDig;
		result[1] = fstChr;
		result[2] = fstVar;
		result[3] = fstFunc;
		result[4] = fstBody;
		result[5] = fstRet;
		result[6] = fstDigLit;
		result[7] = fstStrLit;
		result[8] = fstIf;
		result[9] = fstThen;
		result[10] = fstElse;
		result[11] = fstOut;
		result[12] = fstPower;
		result[13] = fstStrl;
		result[14] = fstId;
		return result;
	}
}