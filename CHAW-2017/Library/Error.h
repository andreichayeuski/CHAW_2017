#pragma once
#define ARRAY_OF_ERRORS_MAX_SIZE 5
#define ERROR_THROW(id) Error::geterror(id);  //throw  ERROR_THROW(id)
#define ERROR_THROW_IN(id, line, col) Error::geterrorin(id, line, col); //throw ERROR_THROW(id, строка, столбец)
#define ERROR_ENTRY(id, message) {id, message, {-1,-1}}	//элемент таблицы ошибок
#define ERROR_MAXSIZE_MESSAGE 200						//максимальная длина сообщения обшибке
#define ERROR_ENTRY_NODEF(id) ERROR_ENTRY(-id, "Неопределенная ошибка")  //1 неопределенный элемент таблицы ошибок

#define ERROR_ENTRY_NODEF10(id)   ERROR_ENTRY_NODEF(id+0),ERROR_ENTRY_NODEF(id+1),ERROR_ENTRY_NODEF(id+2),ERROR_ENTRY_NODEF(id+3), \
								  ERROR_ENTRY_NODEF(id+4),ERROR_ENTRY_NODEF(id+5),ERROR_ENTRY_NODEF(id+6),ERROR_ENTRY_NODEF(id+7), \
								  ERROR_ENTRY_NODEF(id+8),ERROR_ENTRY_NODEF(id+9)

#define ERROR_MAX_ENTRY 100

namespace Error
{
	struct ERROR	// типы исключений для throw ERROR_THROW | ERROR_THROW_IN и catch(ERROR)
	{
		int id;										// код ошибки
		char message[ERROR_MAXSIZE_MESSAGE];		// сообщение об ошибке
		struct IN									// расширение для ошибок при обработке входных данных
		{
			short line;						        // номер строки (отсчёт с нуля)
			short col;								// номер столбца (отсчёт с нуля)
		} intext;
	};
	struct ARRAY_OF_ERRORS							// массив ошибок для определения нескольких ошибок на одном уровне обработки
	{
		ERROR arr_of_err[ARRAY_OF_ERRORS_MAX_SIZE];
		int size = 0;
	};

	ERROR geterror(int id);								// cформировать ERROR для ERROR_THROW 
	ERROR geterrorin(int id, int line, int col);		// сформировать ERROR для ERROR_THROW_IN
	void AddError(ARRAY_OF_ERRORS* arr, ERROR err);		// добавить ошибку в массив ошибок (если переполнение - исключение)
}