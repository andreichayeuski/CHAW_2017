#include "stdafx.h"
#include "PolishNotation.h"

void CallPolishNotation(LT::LexTable* lextable, IT::IdTable* idtable)
{
	for (int i = 0; i < lextable->size; i++)
	{
		if (lextable->table[i].lexema == LEX_EQUAL)
		{
			PolishNotation(++i, lextable, idtable);
		}
	}
}

void AddToResult(IT::IdTable* idtable, LT::Entry* result, LT::Entry paste, int *pos, int *flag, int lenout, int lextable_pos)
{
	if (idtable->table[paste.idxTI].id_first_LE >= lextable_pos && idtable->table[paste.idxTI].id_first_LE < (lextable_pos + lenout))
	{
		idtable->table[paste.idxTI].id_first_LE = lextable_pos + *flag;
	}
	*flag += 1;
	result[*pos] = paste;
	*pos += 1;
}

void AddToResult(stack<LT::Entry>* stk, IT::IdTable* idtable, LT::Entry* result, int *pos, int *flag, int lenout, int lextable_pos)
{
	if (idtable->table[stk->top().idxTI].id_first_LE >= lextable_pos && idtable->table[stk->top().idxTI].id_first_LE < (lextable_pos + lenout))
	{
		idtable->table[stk->top().idxTI].id_first_LE = lextable_pos + *flag;
	}
	*flag += 1;
	result[*pos] = stk->top();
	*pos += 1;
	stk->pop();
}

bool PolishNotation(int lextable_pos, LT::LexTable* lextable, IT::IdTable* idtable)
{
	stack<LT::Entry> stk;
	LT::Entry* result = new LT::Entry[lextable->size];
	int zap = 0,
		func_pos_TI = -1,					//позиция id функции
		flag = 0,
		more = 0,
		len = 0,							//общая длина
		lenout = 0,							//длина выходной строки
		semicolonid;						//ид для элемента таблицы с точкой с запятой
	LT::Entry tmp, buf;
	for (int i = lextable_pos; lextable->table[i].lexema != LEX_SEMICOLON; i++)
	{
		lenout = i + 1;
		semicolonid = i + 1;
	}
	for (int i = lextable_pos; i < lenout; i++)
	{
		tmp = lextable->table[i];
		if (tmp.lexema == LEX_ID || tmp.lexema == LEX_DIG || tmp.lexema == LEX_CHR || tmp.lexema == LEX_POW || tmp.lexema == LEX_STRL)
		{
			if (idtable->table[tmp.idxTI].id_type == IT::F)
			{
				func_pos_TI = tmp.idxTI;
				continue;
			}
			AddToResult(idtable, result, tmp, &len, &flag, lenout, lextable_pos);
		}
		else
		{
			if (tmp.lexema == LEX_ARIPH)
			{
				while (!stk.empty() && stk.top().priority >= tmp.priority)
				{
					if (stk.top().lexema != LEX_LEFTHESIS)
					{
						AddToResult(&stk, idtable, result, &len, &flag, lenout, lextable_pos);
					}
					else break;
				}
				stk.push(tmp);
			}
			if (tmp.lexema == LEX_COMMA)
			{
				zap++;
				while (stk.top().lexema == LEX_ARIPH)
				{
					AddToResult(&stk, idtable, result, &len, &flag, lenout, lextable_pos);
				}
			}
			else if (tmp.lexema != LEX_RIGHTHESIS)
			{
				if (stk.empty() || stk.top().lexema == LEX_LEFTHESIS || tmp.lexema == LEX_LEFTHESIS)
				{
					stk.push(tmp);
				}
			}
			if (tmp.lexema == LEX_RIGHTHESIS && tmp.priority != 4)
			{
				more += 2;
				while (stk.top().lexema != LEX_LEFTHESIS)
				{
					AddToResult(&stk, idtable, result, &len, &flag, lenout, lextable_pos);
				}
				stk.pop();
			}
			if (tmp.lexema == LEX_RIGHTHESIS && tmp.priority == 4)
			{
				buf.lexema = LEX_SUBST;
				buf.idxTI = func_pos_TI;
				buf.sn = result[len - 1].sn;
				buf.priority = zap + 1;
				result[len++] = buf;
				if (zap != 0)
				{
					more += zap;
					zap = 0;
				}
				while (stk.top().lexema != LEX_LEFTHESIS)
				{
					AddToResult(&stk, idtable, result, &len, &flag, lenout, lextable_pos);
				}
				stk.pop();
				more += 2;
			}
		}
	}
	while (!stk.empty())
	{
		AddToResult(&stk, idtable, result, &len, &flag, lenout, lextable_pos);
	}
	for (int i = lextable_pos, k = 0; i < lextable_pos + len; i++, k++)
	{
		lextable->table[i] = result[k];												//запись в таблицу польской записи
	}
	lextable->table[lextable_pos + len] = lextable->table[semicolonid];			//вставка элемента с точкой с запятой
	for (int i = 0; i < more; i++)
	{
		lextable->size--;
		for (int j = lextable_pos + len + 1; j < lextable->size; j++)				//сдвигаем на лишнее место
		{
			lextable->table[j] = lextable->table[j + 1];
			if (lextable->table[j].idxTI != TL_TI_NULLIDX && idtable->table[lextable->table[j].idxTI].id_first_LE == (j + 1))
			{
				idtable->table[lextable->table[j].idxTI].id_first_LE -= 1;
			}
		}
	}
	return true;
}