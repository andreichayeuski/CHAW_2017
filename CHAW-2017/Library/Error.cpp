#include "stdafx.h"
#include "Error.h"
namespace Error
{
	//	0   -  9   -  системные
	//	10  -  19  -  параметров
	//	20  -  29  -  открытия и чтения файлов
	//	30  -  49  -  ошибки лексического анализа
	//  50  -  79  -  ошибки синтаксического анализа
	//  80  -  99  -  ошибки семантического анализа
	ERROR errors[ERROR_MAX_ENTRY] = //таблица ошибок
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"), // код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),

		ERROR_ENTRY(10, "Ошибка входных параметров: Ошибка при создании файла протокола"), 
		ERROR_ENTRY(11, "Ошибка входных параметров: Параметр -in должен быть задан"),
		ERROR_ENTRY(12, "Ошибка входных параметров: Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(13), ERROR_ENTRY_NODEF(14), ERROR_ENTRY_NODEF(15),
		ERROR_ENTRY_NODEF(16), ERROR_ENTRY_NODEF(17), ERROR_ENTRY_NODEF(18), ERROR_ENTRY_NODEF(19),

		ERROR_ENTRY(20, "Ошибка проверки входного файла: Ошибка при открытии файла с исходным кодом"),
		ERROR_ENTRY(21, "Ошибка проверки входного файла: Превышена длина слова"),
		ERROR_ENTRY(22, "Ошибка проверки входного файла: Превышена длина идентификатора"),
		ERROR_ENTRY(23, "Ошибка проверки входного файла: Недопустимый символ в исходном файле"),
		ERROR_ENTRY(24, "Ошибка проверки входного файла: Превышен размер файла с исходным кодом"),
		ERROR_ENTRY_NODEF(25),
		ERROR_ENTRY_NODEF(26), ERROR_ENTRY_NODEF(27), ERROR_ENTRY_NODEF(28), ERROR_ENTRY_NODEF(29),
		
		ERROR_ENTRY(30, "Ошибка лексики: Цепочка символов не разобрана"),
		ERROR_ENTRY(31, "Ошибка лексики: Таблица лексем переполнена"),
		ERROR_ENTRY(32, "Ошибка лексики: Таблица идентификаторов переполнена"),
		ERROR_ENTRY(33, "Ошибка лексики: Два арифметических знака подряд"),
		ERROR_ENTRY_NODEF(34), ERROR_ENTRY_NODEF(35), ERROR_ENTRY_NODEF(36), ERROR_ENTRY_NODEF(37), 
		ERROR_ENTRY_NODEF(38), ERROR_ENTRY_NODEF(39),
		ERROR_ENTRY_NODEF10(40), 
		
		ERROR_ENTRY(50, "Ошибка синтаксиса: Неверная структура программы"),
		ERROR_ENTRY(51, "Ошибка синтаксиса: Ошибочный оператор"),
		ERROR_ENTRY(52, "Ошибка синтаксиса: Неверное выражение"),
		ERROR_ENTRY(53, "Ошибка синтаксиса: Ошибка в параметрах функции или операторе объявления"),
		ERROR_ENTRY(54, "Ошибка синтаксиса: Ошибка в параметре вызываемой функции out стандартной библиотеки"),
		ERROR_ENTRY(55, "Ошибка синтаксиса: Ошибка в параметрах вызываемой функции pow стандартной библиотеки"),
		ERROR_ENTRY(56, "Ошибка синтаксиса: Ошибка в параметре вызываемой функции strl стандартной библиотеки"),
		ERROR_ENTRY(57, "Ошибка синтаксиса: Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(58, "Ошибка синтаксиса: Ошибка арифметического оператора"),
		ERROR_ENTRY(59, "Ошибка синтаксиса: Ошибка в операторе ветвления"),
		ERROR_ENTRY(60, "Ошибка синтаксиса: Ошибка в возвращаемом выражении"),
		ERROR_ENTRY(61, "Ошибка синтаксиса: Не найден конец правила"),
		ERROR_ENTRY(62, "Ошибка синтаксиса: Цепочка разобрана не полностью (стек не пустой)"),
		ERROR_ENTRY_NODEF(63), ERROR_ENTRY_NODEF(64),ERROR_ENTRY_NODEF(65),
		ERROR_ENTRY_NODEF(66), ERROR_ENTRY_NODEF(67),ERROR_ENTRY_NODEF(68),ERROR_ENTRY_NODEF(69),
		ERROR_ENTRY_NODEF10(70),
		
		ERROR_ENTRY(80, "Ошибка семантики: Повторное объявление идентификатора"),
		ERROR_ENTRY(81, "Ошибка семантики: Ошибка в типе идентификатора"),
		ERROR_ENTRY(82, "Ошибка семантики: Ошибка в передаваемых значениях в функцию"),
		ERROR_ENTRY(83, "Ошибка семантики: Ошибка в параметре вызываемой функции strl стандартной библиотеки"),
		ERROR_ENTRY(84, "Ошибка семантики: Необъявленный идентификатор"),
		ERROR_ENTRY(85, "Ошибка семантики: Несоответствие типов в операторе присваивания"),
		ERROR_ENTRY(86, "Ошибка семантики: Слишком большое значение dig-литерала"), 
		ERROR_ENTRY_NODEF(87),ERROR_ENTRY_NODEF(88),ERROR_ENTRY_NODEF(89),
		ERROR_ENTRY_NODEF10(90)
	};
	ERROR geterror(int id)
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		return errors[id];
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		errors[id].intext.line = line;
		errors[id].intext.col = col;
		return errors[id];
	}
	void AddError(ARRAY_OF_ERRORS* arr, ERROR err)
	{
		if (arr->size < ARRAY_OF_ERRORS_MAX_SIZE)
		{
			arr->arr_of_err[arr->size++] = err;
		}
		else
		{
			throw *arr;
		}
	}
}