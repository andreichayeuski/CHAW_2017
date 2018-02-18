#pragma once
#include "Log.h"
#define TI_MAXSIZE		4096		//максимальное кол-во строк в идентификаторе
#define TI_INT_DEFAULT	0x00000000	//значение по умолчанию дл€ типа integer
#define TI_STRDEFAULT	0x00		//значение по умолчанию дл€ типа string
#define TI_NULLIDX		0xffffffff	//нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255	

namespace IT
{
	enum IDDATATYPE { OFF = 0, DIG = 1, CHR = 2, OPR = 3 };
	enum IDTYPE { 
		N = 0,			// нет типа 
		F = 1,			// функци€
		V = 2,			// переменна€
		P = 3,			// параметр функции
		L = 4,			// литерал
		O = 5			// оператор
	};

	struct Entry
	{
		int			id_first_LE;		//индекс первого вхождени€ в таблице лексем
		string		id;					//идентификатор
		IDDATATYPE	id_data_type;		//тип данных
		IDTYPE		id_type;			//тип идентификатора
		struct
		{
			int vint;					//значение integer
			struct {
				int len;						//длина string
				string str;						//символы string
			} vstr;								//значение string
		} value;				//значение идентификатора
	};

	struct IdTable				//экземпл€р таблицы идентификаторов
	{
		int maxsize;						//емкость таблицы идентификаторов < TI_MAXSIZE
		int size;								//текущий размер таблицы идентификаторов < maxsize
		Entry* table;							//массив строк таблицы идентификаторов
	};

	IdTable Create(							//создать таблицу идентификаторов
		int size								//емкость таблицы идентификаторов
	);

	void Add(								//добавить строку в таблицу идентификаторов
		IdTable& idtable,						//экземпл€р таблицы идентификаторов
		Entry entry								//строка таблицы идентификаторов
	);

	Entry GetEntry(							//получить строку таблицы идентификаторов
		IdTable& idtable,						//экземпл€р таблицы идентификаторов
		int n									//номер получаемой строки
	);

	int IsId(								//возврат: номер строки(если есть), TI_NULLIDX(если нет)
		IdTable& idtable,						//экземпл€р таблицы идентификаторов
		string id								//идентафикатор
	);

	void Delete(IdTable& idtable);					//удалить таблицу id
	int CheckId(IdTable& idtable, string value);
	int IsDublId(IdTable& idtable, string id);		//проверка на дублирование ид
	int CheckId(IdTable& idtable, int value);
	void ShowTable(Log::LOG log, IdTable& idtable);
};