#include "stdafx.h"
#include "Lexer.h"
#include "FST.h"

namespace Lex
{
	struct Func
	{
		int indexTI;
		int size = 0;
		string name;
		IT::IDDATATYPE array_of_data_type[FUNC_MAX_SIZE];
	};
	struct Func_Handler
	{
		Func arr_of_func[FUNC_MAX_SIZE];
		int size = -1;
	}Func_Han;

	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);

	FST::STATE_MACHINE SearchFST(string src)
	{
		int size;
		FST::STATE_MACHINE result;
		FST::FST* arr_of_fst = new FST::FST;
		if (src.size() == 1)
		{
			arr_of_fst = FST::arr_of_fst_op();
			size = ARR_OF_FST_OP_SIZE;
		}
		else
		{
			arr_of_fst = FST::arr_of_fst_word();
			size = ARR_OF_FST_WORD_SIZE;
		}
		for (int i = 0; i < size; i++)
		{
			arr_of_fst[i].str = src;
			if (execute(arr_of_fst[i]))
			{
				result.lexema = arr_of_fst[i].lexema;
				result.lex_type = arr_of_fst[i].lex_type;
				return result;
			}
		}
		return { FST::SYMB_NOT_FOUND, 0 };
	}

	LT::Entry CreateLex(char lex, int line, int id_in_TI, int prior)
	{
		LT::Entry result;
		result.priority = prior;
		result.lexema = lex;
		result.sn = line;
		result.idxTI = id_in_TI;
		return result;
	}
	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type)
	{
		IT::Entry result;
		result.id = id;
		result.id_first_LE = line;
		result.id_data_type = id_data_type;
		result.id_type = id_type;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, int vint)
	{
		IT::Entry result;
		result.id = id;
		result.id_first_LE = line;
		result.id_data_type = id_data_type;
		result.id_type = id_type;
		result.value.vint = vint;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, string vstr)
	{
		IT::Entry result;
		result.id = id;
		result.id_first_LE = line;
		result.id_data_type = id_data_type;
		result.id_type = id_type;
		result.value.vstr.str = vstr;
		result.value.vstr.len = vstr.size();
		return result;
	}

	string MakeString(char symb, int num_of_lit)
	{
		string result = "1";
		result[0] = symb;
		return result + to_string(num_of_lit);
	}

	bool CheckFunction(IT::IDDATATYPE *arr_of_it_entry, string name)
	{
		for (int i = 0; i < Func_Han.size + 1; i++)
		{
			if (Func_Han.arr_of_func[i].name == name)
			{
				for (int j = 0; j < Func_Han.arr_of_func[i].size; j++)
				{
					if (Func_Han.arr_of_func[i].array_of_data_type[j] != arr_of_it_entry[j])
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	LEX Lexer(In::IN in)
	{
		bool flag_in_func = false,			// скобки для параметров функции?
			flag_hesis_func = false,		// для определения параметров функции
			flag_id_func = false,			// для имени функции
			flag_into_func = false,			// внутри функции?
			flag_body = false,				// в body?
			func_var = false,				// можно ли объявлять переменные?
			func_type = false;				// есть ли тип возвращаемого значения

		bool flag_ret = false,				// оператор return?
			flag_ariph = false,				// предыдущий символ - арифметический знак?
			flag_pow = false,				// вызов стандартной функции pow?
			flag_strl = false,				// вызов стандартной функции strl?
			flag_func = false,				// идентификатор функции?
			flag_equal = false,				// оператор '='?
			flag_if = false;				// оператор if?

		string func_name = "",				// имя текущей функции (для области видимости)
			func_hesis_name;				// имя функции (для семантического анализа её параметров)
		IT::IDDATATYPE arr_of_it_entry[FUNC_MAX_SIZE];
		Error::ERROR err_to_arr;			// ошибка в массив		
		Error::ARRAY_OF_ERRORS errors;		// массив ошибок на данном этапе
		In::Sep lex_from_queue;				// слово-лексема, подлежащая разбору на данной итерации
		LT::Entry lex_constr;				// лексема, которая формируется в результате данной итерации
		FST::STATE_MACHINE fst_st_mach;		// конечный автомат для конкретного слова-лексемы
		IT::IDDATATYPE id_data_type;		// текущий тип идентификатора
		int size = in.Lexema.size(),		// количество лексем 
			size_arr_it_entry = 0,			// текущая позиция в массиве типов данных параметров локальной функции
			num_of_lit = 0,					// количество литералов
			num_of_id = 0;					// количество идентификаторов без типа данных
		for (int i = 0; i < size; i++)
		{
			lex_from_queue = in.Lexema.front();
			in.Lexema.pop();
			fst_st_mach = SearchFST(lex_from_queue.word);
			lex_constr = CreateLex(fst_st_mach.lexema, lex_from_queue.line, TL_TI_NULLIDX, LT_NOPRIORITY);
			switch (fst_st_mach.lex_type)
			{
			case FST::SYMB_NOT_FOUND:
			{
				err_to_arr = ERROR_THROW_IN(30, lex_constr.sn, -1);
				Error::AddError(&errors, err_to_arr);
				break;
			}
			case FST::SYMB_TABLE_ID:
			{
				int index = 0;
				if (flag_id_func)
				{
					flag_hesis_func = true;
					flag_id_func = false;
					func_name = lex_from_queue.word;
					if (IT::IsDublId(idTable, lex_from_queue.word) != TI_NULLIDX)		// семантика
					{
						err_to_arr = ERROR_THROW_IN(80, lex_constr.sn, -1);
						Error::AddError(&errors, err_to_arr);
					}
					idTable.table[idTable.size] = CreateId(lexTable.size, lex_from_queue.word, IT::OFF, IT::F);
					lex_constr.idxTI = idTable.size++;
					Func_Han.size++;
					func_type = false;
					break;
				}
				if (flag_hesis_func)
				{
					if ((index = IT::IsId(idTable, func_name + lex_from_queue.word)) == TI_NULLIDX && !flag_in_func)
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, IT::OFF, IT::P);
						lex_constr.idxTI = idTable.size++;
						num_of_id += 1;
					}
					else
					{
						if (flag_body)
						{
							arr_of_it_entry[size_arr_it_entry++] = idTable.table[index].id_data_type;
						}
						lex_constr.idxTI = index;
					}
				}
				else if (flag_into_func || flag_body)
				{
					if ((index = IT::IsId(idTable, func_name + lex_from_queue.word)) == TI_NULLIDX)
					{
						if ((index = IT::IsId(idTable, lex_from_queue.word)) != TI_NULLIDX) // если локальная функция
						{
							lex_constr.idxTI = index;
							if (idTable.table[index].id_type == IT::F)
							{
								func_hesis_name = idTable.table[index].id;
								flag_hesis_func = true;
							}
						}
						if (func_var)
						{
							idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, IT::OFF, IT::V);
							lex_constr.idxTI = idTable.size++;
							num_of_id += 1;
						}
						else if (index == TI_NULLIDX)		//семантика
						{
							err_to_arr = ERROR_THROW_IN(84, lex_from_queue.line, -1);
							Error::AddError(&errors, err_to_arr);
						}
					}
					else
					{
						lex_constr.idxTI = index;
						if (flag_equal && !flag_hesis_func)			// семантика
						{
							if (idTable.table[lex_constr.idxTI].id_data_type != id_data_type)
							{
								err_to_arr = ERROR_THROW_IN(85, lex_from_queue.line, -1);
								Error::AddError(&errors, err_to_arr);
							}
						}
						if (func_var)
						{
							err_to_arr = ERROR_THROW_IN(80, lex_constr.sn, -1);
							Error::AddError(&errors, err_to_arr);
						}
					}
				}

				if (flag_ret || flag_if || flag_pow)			// семантика
				{
					if (idTable.table[lex_constr.idxTI].id_data_type != IT::DIG)
					{
						err_to_arr = ERROR_THROW_IN(81, lex_constr.sn, -1);
						Error::AddError(&errors, err_to_arr);
					}
					flag_ret = flag_pow = false;
					if (flag_pow)
					{
						if (idTable.table[lex_constr.idxTI].value.vint < 0)	// семантика
						{
							err_to_arr = ERROR_THROW_IN(81, lex_constr.sn, -1);
							Error::AddError(&errors, err_to_arr);
						}
					}
				}
				if (flag_strl)									// семантика
				{
					if (idTable.table[lex_constr.idxTI].id_data_type != IT::CHR)
					{
						err_to_arr = ERROR_THROW_IN(83, lex_constr.sn, -1);
						Error::AddError(&errors, err_to_arr);
					}
					flag_strl = false;
				}
				flag_ariph = false;
				break;
			}
			case FST::SYMB_NEED_PRIORITY:
			{
				switch (lex_from_queue.word[0])
				{
				case LEX_COMMA:
				{
					lex_constr.priority = 1;
					break;
				}
				case LEX_LEFTHESIS:
				{
					if (flag_hesis_func)
					{
						lex_constr.priority = 4;
						flag_in_func = true;
					}
					else
					{
						lex_constr.priority = 0;
					}
					break;
				}
				case LEX_RIGHTHESIS:
				{
					if (flag_hesis_func)
					{
						if (flag_in_func)
						{
							if (flag_body)
							{
								if (CheckFunction(arr_of_it_entry, func_hesis_name))
								{
									err_to_arr = ERROR_THROW_IN(82, lex_constr.sn, -1);
									Error::AddError(&errors, err_to_arr);
								}
								func_hesis_name = "";
								size_arr_it_entry = 0;
							}
							lex_constr.priority = 4;
							flag_in_func = flag_hesis_func = flag_pow = false;
						}
						else
						{
							err_to_arr = ERROR_THROW_IN(53, lex_constr.sn, -1);
							Error::AddError(&errors, err_to_arr);
						}
					}
					else
					{
						lex_constr.priority = 0;
					}
					break;
				}
				}
				break;
			}
			case FST::SYMB_LEXEMS:
			{
				switch (fst_st_mach.lexema)
				{
				case LEX_BODY:
				{
					flag_body = true;
					func_name = lex_from_queue.word;
					break;
				}
				case LEX_FUNC:
				{
					flag_id_func = true;
					break;
				}
				case LEX_CHR:
				{
					if (num_of_id)
					{
						for (int i = 0; i < num_of_id; i++)
						{
							idTable.table[idTable.size - 1 - i].id_data_type = IT::CHR;
							idTable.table[idTable.size - 1 - i].value.vstr.str = "";
							idTable.table[idTable.size - 1 - i].value.vstr.len = 0;
							if (flag_hesis_func && !flag_body)
							{
								Func_Han.arr_of_func[Func_Han.size].array_of_data_type[Func_Han.arr_of_func[Func_Han.size].size++] = idTable.table[idTable.size - 1 - i].id_data_type;
								Func_Han.arr_of_func[Func_Han.size].name = func_name;
							}
						}
						num_of_id = 0;
					}
					if (!func_type && !flag_in_func)
					{
						int i = idTable.size;
						func_type = true;
						while (idTable.table[i--].id_type != IT::F)
						{
							if (idTable.table[i].id_type == IT::F)
							{
								idTable.table[i].id_data_type = IT::CHR;
							}
						}
					}
					func_var = false;
					lex_constr.lexema = LEX_T;
					break;
				}
				case LEX_DIG:
				{
					if (num_of_id)
					{
						for (int i = 0; i < num_of_id; i++)
						{
							idTable.table[idTable.size - 1 - i].id_data_type = IT::DIG;
							idTable.table[idTable.size - 1 - i].value.vint = 0;
							if (flag_hesis_func && !flag_body)
							{
								Func_Han.arr_of_func[Func_Han.size].array_of_data_type[Func_Han.arr_of_func[Func_Han.size].size++] = idTable.table[idTable.size - 1 - i].id_data_type;
								Func_Han.arr_of_func[Func_Han.size].name = func_name;
							}
						}
						num_of_id = 0;
					}
					if (!func_type && !flag_in_func)
					{
						int i = idTable.size;
						func_type = true;
						while (idTable.table[i--].id_type != IT::F)
						{
							if (idTable.table[i].id_type == IT::F)
							{
								idTable.table[i].id_data_type = IT::DIG;
							}
						}
					}
					func_var = false;
					lex_constr.lexema = LEX_T;
					break;
				}
				case LEX_RET:
				{
					flag_ret = true;
					break;
				}
				case LEX_EQUAL:
				{
					flag_equal = true;
					id_data_type = idTable.table[lexTable.table[lexTable.size - 1].idxTI].id_data_type;
					break;
				}
				case LEX_SEMICOLON:
				{
					if (flag_hesis_func)
					{
						err_to_arr = ERROR_THROW_IN(52, lex_constr.sn, -1);
						Error::AddError(&errors, err_to_arr);
					}
					func_var = flag_equal = flag_ret = false;
					break;
				}
				case LEX_LEFTBRACE:
				{
					if (flag_in_func || flag_hesis_func)
					{
						err_to_arr = ERROR_THROW_IN(52, lex_constr.sn, -1);
						Error::AddError(&errors, err_to_arr);
					}
					flag_into_func = true;
					break;
				}
				case LEX_RIGHTBRACE:
				{
					flag_into_func = false;
					break;
				}
				case LEX_VAR:
				{
					func_var = true;
					break;
				}
				case LEX_IF:
				{
					flag_if = true;
					break;
				}
				case LEX_THEN:
				{
					flag_if = false;
					break;
				}
				}
				break;
			}
			case FST::SYMB_ARIPH:
			{
				switch (lex_from_queue.word[0])
				{
				case LEX_PLUS:
				{
					lex_constr.priority = 2;
					break;
				}
				case LEX_MINUS:
				{
					lex_constr.priority = 2;
					break;
				}
				case LEX_STAR:
				{
					lex_constr.priority = 3;
					break;
				}
				}
				if (flag_ariph)
				{
					err_to_arr = ERROR_THROW_IN(33, lex_constr.sn, -1);
					Error::AddError(&errors, err_to_arr);
				}
				flag_ariph = true;
				lex_constr.idxTI = idTable.size;
				idTable.table[idTable.size++] = CreateId(lexTable.size, lex_from_queue.word, IT::OPR, IT::O);
				break;
			}
			case FST::SYMB_STATIC_LIB:
			{
				int index;
				if ((index = IT::IsId(idTable, lex_from_queue.word)) == TI_NULLIDX)///////////////////
				{
					idTable.table[idTable.size] = CreateId(lexTable.size, lex_from_queue.word, IT::DIG, IT::F);
					lex_constr.idxTI = idTable.size++;
				}
				else
				{
					lex_constr.idxTI = index;
				}
				if (lex_constr.lexema == LEX_POW)		// семантика
				{
					flag_pow = true;
				}
				if (lex_constr.lexema == LEX_STRL)		// семантика
				{
					flag_strl = true;
				}
				flag_hesis_func = true;
				break;
			}
			case FST::SYMB_LITERAL:
			{
				int position;
				switch (fst_st_mach.lexema)
				{
				case LEX_DIG:
				{
					int index = IT::CheckId(idTable, atoi(lex_from_queue.word.c_str()));
					if (index == TI_NULLIDX)
					{
						idTable.table[position = idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, num_of_lit++), IT::DIG, IT::L, atoi(lex_from_queue.word.c_str()));
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						position = lex_constr.idxTI = index;
					}
					if (idTable.table[lex_constr.idxTI].value.vint > INT32_MAX) // добавлено из лекций
					{
						err_to_arr = ERROR_THROW_IN(86, lex_constr.sn, -1);
						Error::AddError(&errors, err_to_arr);
					}
					if (flag_pow)
					{
						if (idTable.table[lex_constr.idxTI].value.vint < 0)	// семантика
						{
							err_to_arr = ERROR_THROW_IN(81, lex_constr.sn, -1);
							Error::AddError(&errors, err_to_arr);
						}
					}
					flag_ariph = false;
					break;
				}
				case LEX_CHR:
				{
					int index = IT::CheckId(idTable, lex_from_queue.word);//спросить про строковые литералы кавычки
					if (index == TI_NULLIDX)
					{
						idTable.table[position = idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, num_of_lit++), IT::CHR, IT::L, lex_from_queue.word);
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						position = lex_constr.idxTI = index;
					}
					if (flag_strl)									// семантика
					{
						flag_strl = false;
					}
					break;
				}
				}
				if (flag_body && flag_hesis_func)
				{
					arr_of_it_entry[size_arr_it_entry++] = idTable.table[lex_constr.idxTI].id_data_type;
				}
				if (flag_equal && !flag_hesis_func && !flag_strl)			// семантика
				{
					if (idTable.table[lex_constr.idxTI].id_data_type != id_data_type)
					{
						err_to_arr = ERROR_THROW_IN(85, lex_from_queue.line, -1);
						Error::AddError(&errors, err_to_arr);
					}
				}
				break;
			}
			}
			lexTable.table[lexTable.size++] = lex_constr;
			if (lexTable.size == LT_MAXSIZE)
			{
				throw ERROR_THROW(31);
			}
			if (idTable.size == TI_MAXSIZE)
			{
				throw ERROR_THROW(32);
			}
		}

		if (errors.size)
		{
			throw errors;
		}
		return { lexTable,idTable };
	}
}