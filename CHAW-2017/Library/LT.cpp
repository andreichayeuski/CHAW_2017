#include "stdafx.h"
#include "LT.h"
namespace LT
{
	LexTable Create(int size)
	{
		LexTable *newTabl = new LexTable;					//�������� ��������� ������� ������
		newTabl->maxsize = size;								//������� ������������ �������
		newTabl->size = 0;									//��������� ������ ����� ����
		newTabl->table = new Entry[size];					//�������� ������� ����� ������� ������
		memset(newTabl->table, 255, 255);					//���������� ����������� ������� ����� �� 255
		return *newTabl;
	}

	Entry Add(LexTable& lextable, Entry entry)
	{
		return(lextable.table[lextable.size++] = entry);	//���������� �� ������� �������� ������� ������� ������ �������� ������� ����� ������
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return (lextable.table[n]); 
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}
	void ShowTable(Log::LOG log, LexTable& lexTable)
	{
		int k = 1;
		*(log.stream) << "\n\n1: ";
		for (int i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].sn != k)
			{
				*(log.stream) << "\n" << lexTable.table[i].sn << ": ";
				k = lexTable.table[i].sn;
				if (lexTable.table[i].idxTI != (int)TL_TI_NULLIDX)
				{
					*(log.stream) << lexTable.table[i].lexema << "[" << lexTable.table[i].idxTI << "]";
				}
				else
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
			}
			else
			{
				if (lexTable.table[i].idxTI != (int)TL_TI_NULLIDX)
				{
					*(log.stream) << lexTable.table[i].lexema << "[" << lexTable.table[i].idxTI << "]";
				}
				else
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
			}
		}
		*(log.stream) << "\n���������� ������ - " << lexTable.size << endl;
	}
}