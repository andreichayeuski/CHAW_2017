#include "stdafx.h"
#include "Greibach.h"
#include "MFST.h"
#include "Lexer.h"

int FST_TRACE_n = -1;
Error::ARRAY_OF_ERRORS errors;
char rbuf[205], sbuf[205], lbuf[1024];
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
#define MFST_TRACE1(log)		*(log.stream)<<setw(4)<<left<<++FST_TRACE_n << ": "\
						<<setw(20)<<left<<rule.getCRule(rbuf, nrulechain)\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getCSt(sbuf)<<endl;
#define MFST_TRACE2(log)		*(log.stream)<<setw(4)<<left<< FST_TRACE_n << ": "\
						<<setw(20)<<left<<" "\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getCSt(sbuf)<<endl;
#define MFST_TRACE3(log)		*(log.stream)<<setw(4)<<left<< ++FST_TRACE_n << ": "\
						<<setw(20)<<left<<" "\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getCSt(sbuf)<<endl;
#define MFST_TRACE4(c,log)  *(log.stream)<<setw(4)<<left<< ++FST_TRACE_n<<": "<<setw(20)<<left<<c<<endl;
#define MFST_TRACE5(c,log)  *(log.stream)<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<c<<endl;
#define MFST_TRACE6(c,k,log)  *(log.stream)<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<c<<k<<endl;
#define MFST_TRACE7(log)  *(log.stream)<<setw(4)<<left<< state.lenta_position<<": " \
								<<setw(20)<<left<<rule.getCRule(rbuf,state.nrulechain)<<endl;

namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrulechain = -1;
		nrule = -1;
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	}
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	}
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	}
	Mfst::Mfst()
	{
		lenta = 0;
		lenta_position = 0;
		lenta_size = 0;
	}
	Mfst::Mfst(Lex::LEX plex, GRB::Greibach pgreibach)
	{
		greibach = pgreibach;
		lex = plex;
		lenta = new short[lenta_size = lex.lextable.size];
		for (int k = 0; k < lenta_size; k++)					//перекодирование ленты
		{
			lenta[k] = TS(lex.lextable.table[k].lexema);
		}
		lenta_position = 0;
		st.push(greibach.stbottomT);
		st.push(greibach.startN);
		nrulechain = -1;
	};
	Mfst::RC_STEP Mfst::step(Log::LOG log, bool need_trace)
	{
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top()))
			{
				GRB::Rule rule;
				if ((nrule = greibach.getRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
					{
						if (need_trace)
						{
							MFST_TRACE1(log)
						}
						savestate(need_trace,log);
						st.pop();
						push_chain(chain);
						rc = NS_OK;
						if (need_trace)
						{
							MFST_TRACE2(log)
						}
					}
					else
					{
						if (need_trace)
						{
							MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", log)
						}
						savediagnosis(NS_NORULECHAIN);
						rc = reststate(need_trace, log) ? NS_NORULECHAIN : NS_NORULE;
					};
				}
				else
				{
					rc = NS_ERROR;
				};
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++;
				st.pop();
				nrulechain = -1;
				rc = TS_OK;
				if (need_trace)
				{
					MFST_TRACE3(log)
				}
			}
			else
			{
				if (need_trace)
				{
					MFST_TRACE4("TS_NOK/NS_NORULECHAIN", log)
				}
					rc = reststate(need_trace,log) ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else
		{
			if (st.top() == greibach.stbottomT)
			{
				rc = LENTA_END;
				if (need_trace)
				{
					MFST_TRACE4("LENTA_END", log)
				}
			}
			else
			{
				throw ERROR_THROW(61);
			}
		};
		return rc;
	};
	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--)
		{
			st.push(chain.nt[k]);
		}
		return true;
	}
	bool Mfst::savestate(bool need_trace, Log::LOG log)
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		if (need_trace)
		{
			MFST_TRACE6("SAVESTATE: ", storestate.size(),log)
		}
		return true;
	}
	bool Mfst::reststate(bool need_trace, Log::LOG log)
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			if (need_trace)
			{
				MFST_TRACE5("RESTATE", log)
					MFST_TRACE2(log)
			}
		}
		return rc;
	}
	bool Mfst::savediagnosis(RC_STEP pprc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		{
			k++;
		}
		if (rc = (k < MFST_DIAGN_NUMBER)) {
			diagnosis[k] = MfstDiagnosis(lenta_position, pprc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
			{
				diagnosis[j].lenta_position = -1;
			}
		}
		return rc;
	}
	bool Mfst::start(bool need_trace, Log::LOG log)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(log, need_trace);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		{
			rc_step = step(log, need_trace);
		}

		switch (rc_step)
		{
		case LENTA_END:
		{
			if (need_trace)
			{
				MFST_TRACE4("--------->LENTA_END",log)
					*(log.stream) << "---------------------------------------------------------------------------------------\n";
			}
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			*(log.stream) << setw(4) << left << "Всего строк (число лексем):  " << lenta_size << ", синтаксический анализ выполнен без ошибок\n";
			rc = true;
			break;
		}

		case NS_NORULE:
		{	*(log.stream) << "NS_NORULE";
		*(log.stream) << getDiagnosis(0, buf) << endl;
		*(log.stream) << getDiagnosis(1, buf) << endl;
		*(log.stream) << getDiagnosis(2, buf) << endl;
		break;
		}
		if (need_trace)
		{
		case NS_NORULECHAIN:	MFST_TRACE4("---------->NS_NORULECHAIN", log) break;
		case NS_ERROR:			MFST_TRACE4("---------->NS_ERROR", log) break;
		case SURPRISE:			MFST_TRACE4("---------->SURPRISE", log) break;
		}
		}
		if (errors.size)
		{
			throw errors;
		}
		return rc;
	}
	char* Mfst::getCSt(char* buf)
	{
		for (int k = (signed)st.size() - 1; k >= 0; k--)
		{
			short p = st._Get_container()[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		};
		buf[st.size()] = 0x00;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
		{
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		}
		buf[i - pos] = 0x00;
		return buf;
	}
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char *rc = "";
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = greibach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = ERROR_THROW_IN(errid, lex.lextable.table[lpos].sn, -1);
			Error::AddError(&errors, err);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s", err.id, lex.lextable.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}
	void Mfst::printrules(Log::LOG &log)
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = greibach.getRule(state.nrule);
			MFST_TRACE7(log)
		};
	}
	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		}
		return true;
	}
}