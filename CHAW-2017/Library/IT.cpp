#include "stdafx.h"
#include "IT.h"

namespace IT
{
	IdTable Create(int size)
	{
		IdTable* p = new IdTable;
		p->maxsize = size;
		p->size = 0;
		p->table = new Entry[size];
		return (*p);
	}

	void Add(IdTable& idtable, Entry entry)
	{
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return (idtable.table[n]);
	}

	int IsId(IdTable& idtable, string id)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].id == id)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int IsDublId(IdTable& idtable, string id)
	{
		for (int i = 0; i < idtable.size - 1; i++)
		{
			if (idtable.table[i].id == id)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int CheckId(IdTable& idtable, string value)
	{
		bool rc = false;
		int i;
		for (i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].value.vstr.str == value)
			{
				rc = true;
				break;
			}
		}
		return rc ? i : TI_NULLIDX;
	}

	int CheckId(IdTable& idtable, int value)
	{
		bool rc = false;
		int i;
		for (i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].value.vint == value)
			{
				if (idtable.table[i].id_type == L && idtable.table[i].id_data_type == DIG)
				{
					rc = true;
					break;
				}
			}
		}
		return rc ? i : TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}
	void ShowTable(Log::LOG log, IdTable& idtable)
	{
		int i, numberOP = 0;
		*(log.stream) << setfill('-') << setw(87) << '-' << endl;
		*(log.stream) << "   №" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение    " << endl;
		*(log.stream) << setw(87) << '-' << endl;
		for (i = 0; i < idtable.size; i++)
		{
			*(log.stream) << setfill(' ') << setw(4) << std::right << i << " | ";
			*(log.stream) << setw(13) << left << idtable.table[i].id << " | ";
			switch (idtable.table[i].id_data_type)
			{
			case DIG: 
			{
				*(log.stream) << setw(10) << left;
				if (idtable.table[i].id_type == O)
				{
					*(log.stream) << "-" << " | ";
				}
				else
				{
					*(log.stream) << "dig" << " | ";
				}
				break; 
			}
			case CHR: 
			{
				*(log.stream) << setw(10) << left;
				if (idtable.table[i].id_type == O)
				{
					*(log.stream) << "-" << " | ";
				}
				else
				{
					*(log.stream) << "chr" << " | ";
				}
				break;
			}
			default: *(log.stream) << setw(10) << left << "unknown" << " | "; break;
			}
			switch (idtable.table[i].id_type)
			{
			case V: *(log.stream) << setw(18) << left << "переменная" << " | "; break;
			case F: *(log.stream) << setw(18) << left << "функция" << " | "; break;
			case P: *(log.stream) << setw(18) << left << "параметр" << " | "; break;
			case L: *(log.stream) << setw(18) << left << "литерал" << " | "; break;
			case O: *(log.stream) << setw(18) << left << "оператор" << " | ";
				numberOP++;
				break;
			default: *(log.stream) << setw(18) << left << "unknown" << " | "; break;
			}
			*(log.stream) << setw(11) << left << idtable.table[i].id_first_LE << " | ";
			if (idtable.table[i].id_data_type == DIG && (idtable.table[i].id_type == V || idtable.table[i].id_type == L))
			{
				*(log.stream) << setw(18) << left << idtable.table[i].value.vint;
			}
			else if (idtable.table[i].id_data_type == CHR && (idtable.table[i].id_type == V || idtable.table[i].id_type == L))
			{
				*(log.stream) << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			}
			else
			{
				*(log.stream) << "-";
			}
			*(log.stream) << endl;
		}
		*(log.stream) << setfill('-') << setw(87) << '-' << endl;
		*(log.stream) << "Количество идентификаторов: " << i - numberOP << endl;
		*(log.stream) << setw(87) << '-' << endl;
	}
}