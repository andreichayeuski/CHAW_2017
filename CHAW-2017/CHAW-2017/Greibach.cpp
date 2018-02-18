#include "stdafx.h"
#include "Greibach.h"

#define GRB_ERROR_SERIES 50

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
	Greibach greibach(
		NS('A'), TS('$'),                     // ��������� ������, ��� �����
		11,									  // ���������� ������
		Rule(
			NS('A'), GRB_ERROR_SERIES + 0,		// �������� ��������� ���������
			2,									// A->	fi(F):t{N}A  |  b{N}
			Rule::Chain(11, TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(':'), TS('t'), TS('{'), NS('N'), TS('}'), NS('A')),
			Rule::Chain(4, TS('b'), TS('{'), NS('N'), TS('}'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,		// ��������� ��������
			9,									// N->	vF;N  |  vF;  |  rR;  |	 i = E;N  |  i = E;	 |	o(O);  |  o(O);N  |  qQ1{N}2{N}N  |  qQ1{N}N
			Rule::Chain(4, TS('v'), NS('F'), TS(';'), NS('N')),
			Rule::Chain(3, TS('v'), NS('F'), TS(';')),
			Rule::Chain(3, TS('r'), NS('R'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('o'), TS('('), NS('O'), TS(')'), TS(';')),
			Rule::Chain(6, TS('o'), TS('('), NS('O'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(11, TS('q'), NS('Q'), TS('1'), TS('{'), NS('N'), TS('}'), TS('2'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(7, TS('q'), NS('Q'), TS('1'), TS('{'), NS('N'), TS('}'), NS('N'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,		// �������� ���������
			13,									// E->	i  |  c  |  d  |  iM  |  dM  |  p(P)  |  p(P)M  |  s(S)  |  s(S)M  |  i(I)  |  i(I)M  |  (E)  |  (E)M
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(1, TS('d')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('d'), NS('M')),
			Rule::Chain(4, TS('p'), TS('('), NS('P'), TS(')')),
			Rule::Chain(5, TS('p'), TS('('), NS('P'), TS(')'), NS('M')),
			Rule::Chain(4, TS('s'), TS('('), NS('S'), TS(')')),
			Rule::Chain(5, TS('s'), TS('('), NS('S'), TS(')'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), NS('I'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('I'), TS(')'), NS('M')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 3,		// ��������� ������� � �������� ����������
			3,									// F->	i,F  |  i:t  |  i:t,F
			Rule::Chain(3, TS('i'), TS(','), NS('F')),
			Rule::Chain(3, TS('i'), TS(':'), TS('t')),
			Rule::Chain(5, TS('i'), TS(':'), TS('t'), TS(','), NS('F'))
		),
		Rule(
			NS('O'), GRB_ERROR_SERIES + 4,		// ��������� ���������� ������� out ����������� ����������
			3,									// O->  i  |  c  |  d
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(1, TS('d'))
		),
		Rule(
			NS('P'), GRB_ERROR_SERIES + 5,		// ��������� ���������� ������� pow ����������� ����������
			2,									// P->  i,d  |  d,d
			Rule::Chain(3, TS('i'), TS(','), TS('d')),
			Rule::Chain(3, TS('d'), TS(','), TS('d'))
		),
		Rule(
			NS('S'), GRB_ERROR_SERIES + 6,		// ��������� ���������� ������� strl ����������� ����������
			2,									// S->  i  |  c
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c'))
		),
		Rule(
			NS('I'), GRB_ERROR_SERIES + 7,		// ��������� ���������� ������� 
			6,									// I->  i  |  i,I  |  d  |  d,I  |  c  |  c,I
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('i'), TS(','), NS('I')),
			Rule::Chain(1, TS('d')),
			Rule::Chain(3, TS('d'), TS(','), NS('I')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(3, TS('c'), TS(','), NS('I'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 8,		// ��������
			2,									// M->  aE  |  aEM
			Rule::Chain(2, TS('a'), NS('E')),
			Rule::Chain(3, TS('a'), NS('E'), NS('M'))
		),
		Rule(
			NS('Q'), GRB_ERROR_SERIES + 9,		// ���������
			2,									// Q->  (R<R)  |  (R>R)
			Rule::Chain(5, TS('('), NS('R'), TS('<'), NS('R'), TS(')')),
			Rule::Chain(5, TS('('), NS('R'), TS('>'), NS('R'), TS(')'))
		),
		Rule(
			NS('R'), GRB_ERROR_SERIES + 10,		// ��������� return
			2,									// R->  i  |  d
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('d'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //����������� ������� - ������ ����� �������(���������� �������� � �������, �������� ��� ����������...)
	{
		nt = new GRBALPHABET[size = psize];    //������� ����������
		int*p = (int*)&s;                      //����������� ������ ��������� �
		for (short i = 0; i < psize; ++i)
		{
			nt[i] = (GRBALPHABET)p[i];
		}			//��������� ������� ����������
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) //����������� �������
	{															//(����������, ������������� ���������������� ���������, ���������� �������(������ ������ �������), ��������� ������� (������ ������ �������)
		nn = pnn;							//����������
		iderror = piderror;					//�������������
		chains = new Chain[size = psize];	//����� ��� �������
		Chain*p = &c;
		for (int i = 0; i < size; i++)
		{
			chains[i] = p[i];			//��������� ��������� �������
		}
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)//����������� ���������� �������(��������� ������, ��� �����, ���������� ������, �������...)
	{
		startN = pstartN;					//��������� ������
		stbottomT = pstbottom;				//��� �����
		rules = new Rule[size = psize];		//�������� ������
		Rule*p = &r;
		for (int i = 0; i < size; i++) 
		{
			rules[i] = p[i];			//��������� �������
		}
	};
	Greibach getGreibach() { return greibach; }; //�������� ����������
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //�������� ������� (����� ������ �������, ������������ ������� ����������)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
		{
			k++;								 //���� � ������ ���������� ������ � ���� ����� ������ ������� �� ����� �������� �������
		}
		if (k < size)
		{
			prule = rules[rc = k];    //������������ ������� ��������� ����� ������� � �������� �
		}
		return rc;						 //������������ ����� ������� ��� -1
	};
	Rule Greibach::getRule(short n) //�������� ������� �� ������
	{
		Rule rc;				//������ ������� 
		if (n < size) 
		{
			rc = rules[n];		//����������� ������� �������
		}
		return rc;			//���������� ��������� �������
	};
	char* Rule::getCRule(char* b, short nchain) //�������� ������� � ���� N->������� (�����, ����� �������(������ �����) � �������)
	{
		char bchain[200];															//������
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00;	//�������� -> 
		chains[nchain].getCChain(bchain);											//�������� ������ ������� �������
		strcat_s(b, sizeof(bchain) + 5, bchain);									//��������� ������ (����, � ������ ��������, ������)
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //�������� ��������� �� j ���������� �������, ������� � ����� ��� -1 
	{                                                                //(������ ������ �������, ������������ �������, ����� �������)
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
		{
			++j;
		}
		rc = (j < size ? j : -1);
		if (rc >= 0) 
		{
			pchain = chains[rc];
		}
		return rc;
	};
	char*Rule::Chain::getCChain(char*b)				//�������� ������ ������� �������
	{
		for (int i = 0; i < size; i++)
		{
			b[i] = Chain::alphabet_to_char(nt[i]);
		}
		b[size] = 0x00;
		return b;
	};
};