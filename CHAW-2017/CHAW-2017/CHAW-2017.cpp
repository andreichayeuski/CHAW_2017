#include "stdafx.h"
#include "Lexer.h"
#include "FST.h"
#include "MFST.h"
#include "Greibach.h"
#include "PolishNotation.h"
#include "CodeGeneration.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(0, "");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::GetLog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		//проверка файла с исходным кодом на допустимость символов
		In::IN in = In::GetIn(parm.in);
		Log::WriteIn(log, in);

		log = Log::GetLog(parm.lex);
		Log::WriteLine(log, "Лексический и семантический анализ\n", "");
		Lex::LEX lex = Lex::Lexer(in);
		Log::WriteLine(log, "Выполнены без ошибок", "");
		LT::ShowTable(log, lex.lextable);
		IT::ShowTable(log, lex.idtable);

		log = Log::GetLog(parm.sin);
		Log::WriteLine(log, "Синтаксический анализ\n", "");
		if (parm.need_trace)
		{
			MFST_TRACE_START(log)
		}
			MFST::Mfst mfst(lex, GRB::getGreibach());
		mfst.start(parm.need_trace, log);
		mfst.savededucation();
		mfst.printrules(log);
		Log::WriteLine(log, "\nПольская нотация", "");

		CallPolishNotation(&lex.lextable, &lex.idtable);
		LT::ShowTable(log, lex.lextable);
		IT::ShowTable(log, lex.idtable);
		Log::WriteLine(log, "Выполнена без ошибок", "");

		log = Log::GetLog(parm.out);
		CodeGeneration::CodeGeneration(lex, log);
		cout << "Трансляция выполнена успешно!\n";
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		wcout << L"Ошибка. Откройте файл " << log.logfile << endl;
		Log::WriteError(log, e);
	}
	catch (Error::ARRAY_OF_ERRORS e)
	{
		wcout << L"Ошибки. Откройте файл " << log.logfile << endl;
		for (int i = 0; i < e.size; i++)
		{
			Log::WriteError(log, e.arr_of_err[i]);
		}
	}
	catch (...)
	{
		cout << "Ошибка: Системный сбой" << endl;
	}
	system("pause");
	return 0;
};