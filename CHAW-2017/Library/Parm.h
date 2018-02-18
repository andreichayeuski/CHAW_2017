#pragma once
#define PARM_IN  L"-in:"					// ключ для файла исходного кода
#define PARM_OUT L"-out:"					// ключ для файла объектного кода	
#define PARM_LOG L"-log:"					// ключ для файла журнала 
#define PARM_LEX L"-lex:"					// ключ для файла-протокола лексического и семантического анализа
#define PARM_SIN L"-sin:"					// ключ для файла-протокола синтаксического анализа
#define PARM_TRACE L"-ntr"					// ключ для разрешения вывода трассировки синтаксического анализа
#define PARM_MAX_SIZE 300					// максимальная длина строки параметра
#define PARM_OUT_DEFAULT_EXT L".asm"		// расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log"		// расширение файла протокола по умолчанию
#define PARM_LEX_DEFAULT_EXT L".lx" 		// расширение файла-протокола лексического анализа
#define PARM_SIN_DEFAULT_EXT L".sin" 		// расширение файла-протокола синтаксического анализа

namespace Parm
{
	struct PARM
	{
		wchar_t  in[PARM_MAX_SIZE];			// -in:     имя файла исходного кода
		wchar_t out[PARM_MAX_SIZE];			// -out:    имя файла объектного кода
		wchar_t log[PARM_MAX_SIZE];			// -log:    имя файла протокола
		wchar_t lex[PARM_MAX_SIZE];			// -lex:	имя файла протокола лексического анализа
		wchar_t sin[PARM_MAX_SIZE];			// -sin:	имя файла протокола синтаксического анализа
		bool need_trace = false;
	};
	PARM getparm(int argc, _TCHAR* argv[]);		// сформировать struct PARM на основе входных параметров функции main
}