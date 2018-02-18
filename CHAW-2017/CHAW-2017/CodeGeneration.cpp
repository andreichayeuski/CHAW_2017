#include "stdafx.h"
#include "CodeGeneration.h"

namespace CodeGeneration
{
	void CodeGeneration(Lex::LEX lex, Log::LOG log)
	{
		*(log.stream) << ".586\n\t.model flat, stdcall\n\tincludelib libucrt.lib\n\tincludelib kernel32.lib\n\t	includelib ../Debug/Standard_Library.lib\n\tExitProcess PROTO :DWORD\n\n";
		*(log.stream) << "\toutc PROTO: DWORD\n\toutd PROTO: DWORD\n\tstrl PROTO: DWORD\n\tpower PROTO: DWORD, :DWORD\n";
		*(log.stream) << "\n.stack 4096\n";

		*(log.stream) << ".const\n";
		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].id_type == IT::L)
			{
				*(log.stream) << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].id_data_type == IT::CHR)
				{
					*(log.stream) << " BYTE " << lex.idtable.table[i].value.vstr.str << ", 0\n";
				}
				if (lex.idtable.table[i].id_data_type == IT::DIG)
				{
					*(log.stream) << " DWORD " << lex.idtable.table[i].value.vint << endl;
				}
			}
		}

		*(log.stream) << ".data\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema == LEX_VAR)
			{
				while (lex.idtable.table[lex.lextable.table[i + 1].idxTI].id_type == IT::V)
				{
					*(log.stream) << "\t" << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id;
					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].id_data_type == IT::CHR)
					{
						*(log.stream) << " DWORD ?\n";
					}
					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].id_data_type == IT::DIG)
					{
						*(log.stream) << " SDWORD ?\n";
					}
					i += 2;
				}
			}
		}

		stack<string> stk;
		int num_of_points = 0,
			num_of_ret = 0,
			num_of_ends = 0;
		string strret = "",
			func_name = "";					// имя локальной функции
		bool flag_func = false,					// внутри локальной функции?
			flag_ret = false,					// был ret?
			flag_body = false,					// внутри главной функции?
			flag_if = false,					// внутри if?
			flag_then = false,					// внутри then?
			flag_else = false;					// внутри then/else?
		*(log.stream) << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_FUNC:
			{
				*(log.stream) << (func_name = lex.idtable.table[lex.lextable.table[++i].idxTI].id) << " PROC ";
				while (lex.lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lex.idtable.table[lex.lextable.table[i].idxTI].id_type == IT::P)
					{
						*(log.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].id << " : ";
						if (lex.idtable.table[lex.lextable.table[i++].idxTI].id_data_type == IT::DIG)
						{
							*(log.stream) << "SDWORD";
						}
						else
						{
							*(log.stream) << "DWORD";
						}
					}
					if (lex.lextable.table[i].lexema == LEX_COMMA)
					{
						*(log.stream) << ", ";
					}
					i++;
				}
				flag_func = true;
				*(log.stream) << endl;
				break;
			}
			case LEX_BODY:
			{
				flag_body = true;
				*(log.stream) << "main PROC\n";
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;
				while (lex.lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lex.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_DIG:
					{
						*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i].idxTI].id << endl;
						stk.push(lex.idtable.table[lex.lextable.table[i].idxTI].id);
						break;
					}
					case LEX_CHR:
					{
						*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i].idxTI].id << endl;
						stk.push("offset " + lex.idtable.table[lex.lextable.table[i].idxTI].id);
						break;
					}
					case LEX_SUBST:
					{
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)
						{
							*(log.stream) << "\tpop edx\n";
						}
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)
						{
							*(log.stream) << "\tpush " << stk.top() << endl;
							stk.pop();
						}
						*(log.stream) << "\t\tcall " << lex.idtable.table[lex.lextable.table[i].idxTI].id << "\n\tpush eax\n";
						break;
					}
					case LEX_ARIPH:
					{
						switch (lex.idtable.table[lex.lextable.table[i].idxTI].id[0])
						{
						case LEX_STAR:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tmul ebx\n\tpush eax\n";
							break;
						}
						case LEX_PLUS:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tadd eax, ebx\n\tpush eax\n";
							break;
						}
						case LEX_MINUS:
						{
							*(log.stream) << "\tpop ebx\n\tpop eax\n";
							*(log.stream) << "\tsub eax, ebx\n\tpush eax\n";
							break;
						}
						}
						break;
					}
					}
					i++;
				}
				*(log.stream) << "\tpop " << lex.idtable.table[lex.lextable.table[result_position].idxTI].id << "\n";
				break;
			}
			case LEX_RET:
			{
				*(log.stream) << "\tpush ";
				i++;
				if (lex.idtable.table[lex.lextable.table[i].idxTI].id_type == IT::L)
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].idxTI].value.vint << endl;
				}
				else
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].idxTI].id << endl;
				}
				if (flag_func)
				{
					*(log.stream) << "\t\tjmp local" << num_of_ret << endl;
					flag_ret = true;
				}
				if (flag_body)
				{
					*(log.stream) << "\t\tjmp theend\n";
					flag_ret = true;
				}
				break;
			}
			case LEX_RIGHTBRACE:
			{
				if (flag_body && !flag_then && !flag_else && !flag_func)
				{
					if (flag_ret)
					{
						*(log.stream) << "theend:\n";
						flag_ret = false;
					}
					*(log.stream) << "\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (flag_func)
				{
					if (flag_ret)
					{
						*(log.stream) << "local" << num_of_ret++ << ":\n";
						*(log.stream) << "\tpop eax\n\tret\n";
						flag_ret = false;
					}
					*(log.stream) << func_name << " ENDP\n\n";
					flag_func = false;
				}
				if (flag_then)
				{
					flag_then = false;
					if (flag_else)
					{
						*(log.stream) << "\tjmp e" << num_of_ends << endl;
						flag_else = false;
					}
					*(log.stream) << "m" << num_of_points++ << ":\n";
				}
				if (flag_else)
				{
					flag_else = false;
					*(log.stream) << "e" << num_of_ends++ << ":\n";
				}
				break;
			}
			case LEX_IF:
			{
				flag_if = true;
				break;
			}
			case LEX_LEFTHESIS:
			{
				if (flag_if)
				{
					*(log.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << endl;
					*(log.stream) << "\tcmp eax, " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << endl;
					if (lex.lextable.table[i + 2].lexema == LEX_GREAT)
					{
						*(log.stream) << "\t\tjg m" << num_of_points << endl;
						*(log.stream) << "\t\tjl m" << num_of_points + 1 << endl;
					}
					else
					{
						*(log.stream) << "\t\tjl m" << num_of_points << endl;
						*(log.stream) << "\t\tjg m" << num_of_points + 1 << endl;
					}
					*(log.stream) << "\t\tje m" << num_of_points + 1 << endl;
					int j = i;
					while (lex.lextable.table[j++].lexema != LEX_RIGHTBRACE)
					{
						if (lex.lextable.table[j + 1].lexema == LEX_ELSE)
						{
							flag_else = true;
							break;
						}
					}
					flag_if = false;
				}
				break;
			}
			case LEX_THEN:
			{
				flag_then = true;
				*(log.stream) << "m" << num_of_points++ << ":\n";
				break;
			}
			case LEX_ELSE:
			{
				flag_else = true;
				break;
			}
			case LEX_OUT:
			{
				if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].id_data_type == IT::DIG)
				{
					*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n\t\tcall outd\n";
				}
				else
				{
					*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n\t\tcall outc\n";
				}
				break;
			}
			}
		}
	}
}