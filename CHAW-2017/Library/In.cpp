#include "stdafx.h"
#include "In.h"
#include "Error.h"
namespace In
{
	IN GetIn(wchar_t infile[])
	{
		ifstream source_file(infile);
		if (source_file.fail())
		{
			throw ERROR_THROW(20);
		}
		IN in;
		Error::ERROR err_to_arr;
		Error::ARRAY_OF_ERRORS errors;			// массив для ошибок
		in.lines = 1;
		in.size = in.ignor = 0;		// инициализация структуры In::IN
		unsigned char symb;		// символ
		int col = 0;			// номер текущей позиции в строке	
		char temp[IN_WORD_MAX_LEN];	// буферная строка для формирования слов-лексем
		short wordLen = 0;			// текущая длина temp

		while ((!source_file.eof()) && (symb = source_file.get()))
		{
			if (symb == '\"')
			{
				temp[wordLen++] = symb;
				do
				{
					if ((symb = source_file.get()) && wordLen != IN_WORD_MAX_LEN)
					{
						temp[wordLen++] = symb;
						in.size++;
					}
					else
					{
						throw ERROR_THROW_IN(21, in.lines, col);
					}
				} while (symb != '\"');
				temp[wordLen] = '\0';
				in.Lexema.push({ temp, in.lines });
				wordLen = 0;
				in.size += 1;
				continue;
			}

			if (symb == IN_CODE_SPACE || in.code[(int)symb] == IN::S || symb == IN_CODE_ENDL || symb == IN_CODE_TAB)
			{
				temp[wordLen] = '\0';
				if (wordLen)
				{
					wordLen = 0;
					in.Lexema.push({ temp, in.lines });
				}
				if (symb == IN_CODE_SPACE || symb == IN_CODE_TAB)				// проверка на пробелы
				{
					in.ignor += 1;
				}
				if (in.code[(int)symb] == IN::S)		// проверка на символ-сепаратор
				{
					char buf[2];
					buf[0] = symb;
					buf[1] = '\0';
					in.Lexema.push({ buf, in.lines });
					wordLen = 0;
				}
				if (symb == IN_CODE_ENDL)				// проверка на символ перехода на новую строку
				{
					in.lines++;
					col = 0;
				}
			}
			else if (in.code[(int)symb] == IN::T)
			{
				col++;
				temp[wordLen++] = symb;
				in.size++;
			}
			else
			{
				err_to_arr = ERROR_THROW_IN(23, in.lines, col);
				Error::AddError(&errors, err_to_arr);
			}
			if (wordLen == IN_ID_MAX_SIZE)
			{
				err_to_arr = ERROR_THROW_IN(22, in.lines, col);
				Error::AddError(&errors, err_to_arr);
			}
			if (in.size == IN_MAX_LEN_TEXT)
			{
				throw ERROR_THROW(24);
			}
		}
		if (errors.size)
		{
			throw errors;
		}
		source_file.close();
		return in;
	}
}