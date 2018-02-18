#include "stdafx.h"
#include "Log.h"
namespace Log
{
	LOG GetLog(wchar_t logfile[])
	{
		LOG a = { NULL, NULL };
		wcscpy_s(a.logfile, logfile);
		ofstream * q = new ofstream(logfile);
		if (!q->fail())
		{
			a.stream = q;
		}
		else
		{
			throw ERROR_THROW(10);
		}
		return a;
	}
	void WriteLine(LOG log, char* c, ...)
	{
		char **p = &c;
		while (strcmp(*p, ""))
		{
			*(log.stream) << *p;
			p++;
		}
	}

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t **p = &c;
		char buf[PARM_MAX_SIZE];
		while (wcscmp(*p, L""))
		{
			wcstombs(buf, *p, PARM_MAX_SIZE);
			*(log.stream) << buf;
			p++;
		}
	}
	void WriteLog(LOG log)
	{
		struct tm newtime;
		__time64_t ltime;
		errno_t err;
		char s[100];
		_time64(&ltime);
		err = _localtime64_s(&newtime, &ltime);
		strftime(s, 100, "%c ", &newtime);
		*log.stream << "---- Протокол ------ Дата: " << s << " -------------" << endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		char buf[PARM_MAX_SIZE];
		*(log.stream) << "---- Параметры --------" << endl;
		wcstombs(buf, parm.in, PARM_MAX_SIZE);
		*(log.stream) << "-in:  " << buf << endl;
		wcstombs(buf, parm.out, PARM_MAX_SIZE);
		*(log.stream) << "-out: " << buf << endl;
		wcstombs(buf, parm.log, PARM_MAX_SIZE);
		*(log.stream) << "-log: " << buf << endl;
		wcstombs(buf, parm.lex, PARM_MAX_SIZE);
		*(log.stream) << "-lex: " << buf << endl;
		wcstombs(buf, parm.sin, PARM_MAX_SIZE);
		*(log.stream) << "-sin: " << buf << endl;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*(log.stream) << "---- Исходные данные ------" << endl;
		*(log.stream) << "Количество символов:" << in.size << endl;
		*(log.stream) << "Количество строк   :" << in.lines << endl;
		*(log.stream) << "Количество проигнорированных символов: " << in.ignor << endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream)
		{
			*log.stream << "Ошибка " << error.id << ": " << error.message
				<< ", строка " << error.intext.line
				<< ", позиция " << error.intext.col << endl << endl;
		}
		else
		{
			cout << "Ошибка " << error.id << ": " << error.message
				<< ", строка " << error.intext.line
				<< ", позиция " << error.intext.col << endl << endl;
		}
	}
	void Close(LOG log)
	{
		log.stream->close();
	}
}