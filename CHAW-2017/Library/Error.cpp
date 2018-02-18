#include "stdafx.h"
#include "Error.h"
namespace Error
{
	//	0   -  9   -  ���������
	//	10  -  19  -  ����������
	//	20  -  29  -  �������� � ������ ������
	//	30  -  49  -  ������ ������������ �������
	//  50  -  79  -  ������ ��������������� �������
	//  80  -  99  -  ������ �������������� �������
	ERROR errors[ERROR_MAX_ENTRY] = //������� ������
	{
		ERROR_ENTRY(0, "������������ ��� ������"), // ��� ������ ��� ��������� 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "��������� ����"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),

		ERROR_ENTRY(10, "������ ������� ����������: ������ ��� �������� ����� ���������"), 
		ERROR_ENTRY(11, "������ ������� ����������: �������� -in ������ ���� �����"),
		ERROR_ENTRY(12, "������ ������� ����������: ��������� ����� �������� ���������"),
		ERROR_ENTRY_NODEF(13), ERROR_ENTRY_NODEF(14), ERROR_ENTRY_NODEF(15),
		ERROR_ENTRY_NODEF(16), ERROR_ENTRY_NODEF(17), ERROR_ENTRY_NODEF(18), ERROR_ENTRY_NODEF(19),

		ERROR_ENTRY(20, "������ �������� �������� �����: ������ ��� �������� ����� � �������� �����"),
		ERROR_ENTRY(21, "������ �������� �������� �����: ��������� ����� �����"),
		ERROR_ENTRY(22, "������ �������� �������� �����: ��������� ����� ��������������"),
		ERROR_ENTRY(23, "������ �������� �������� �����: ������������ ������ � �������� �����"),
		ERROR_ENTRY(24, "������ �������� �������� �����: �������� ������ ����� � �������� �����"),
		ERROR_ENTRY_NODEF(25),
		ERROR_ENTRY_NODEF(26), ERROR_ENTRY_NODEF(27), ERROR_ENTRY_NODEF(28), ERROR_ENTRY_NODEF(29),
		
		ERROR_ENTRY(30, "������ �������: ������� �������� �� ���������"),
		ERROR_ENTRY(31, "������ �������: ������� ������ �����������"),
		ERROR_ENTRY(32, "������ �������: ������� ��������������� �����������"),
		ERROR_ENTRY(33, "������ �������: ��� �������������� ����� ������"),
		ERROR_ENTRY_NODEF(34), ERROR_ENTRY_NODEF(35), ERROR_ENTRY_NODEF(36), ERROR_ENTRY_NODEF(37), 
		ERROR_ENTRY_NODEF(38), ERROR_ENTRY_NODEF(39),
		ERROR_ENTRY_NODEF10(40), 
		
		ERROR_ENTRY(50, "������ ����������: �������� ��������� ���������"),
		ERROR_ENTRY(51, "������ ����������: ��������� ��������"),
		ERROR_ENTRY(52, "������ ����������: �������� ���������"),
		ERROR_ENTRY(53, "������ ����������: ������ � ���������� ������� ��� ��������� ����������"),
		ERROR_ENTRY(54, "������ ����������: ������ � ��������� ���������� ������� out ����������� ����������"),
		ERROR_ENTRY(55, "������ ����������: ������ � ���������� ���������� ������� pow ����������� ����������"),
		ERROR_ENTRY(56, "������ ����������: ������ � ��������� ���������� ������� strl ����������� ����������"),
		ERROR_ENTRY(57, "������ ����������: ������ � ���������� ���������� �������"),
		ERROR_ENTRY(58, "������ ����������: ������ ��������������� ���������"),
		ERROR_ENTRY(59, "������ ����������: ������ � ��������� ���������"),
		ERROR_ENTRY(60, "������ ����������: ������ � ������������ ���������"),
		ERROR_ENTRY(61, "������ ����������: �� ������ ����� �������"),
		ERROR_ENTRY(62, "������ ����������: ������� ��������� �� ��������� (���� �� ������)"),
		ERROR_ENTRY_NODEF(63), ERROR_ENTRY_NODEF(64),ERROR_ENTRY_NODEF(65),
		ERROR_ENTRY_NODEF(66), ERROR_ENTRY_NODEF(67),ERROR_ENTRY_NODEF(68),ERROR_ENTRY_NODEF(69),
		ERROR_ENTRY_NODEF10(70),
		
		ERROR_ENTRY(80, "������ ���������: ��������� ���������� ��������������"),
		ERROR_ENTRY(81, "������ ���������: ������ � ���� ��������������"),
		ERROR_ENTRY(82, "������ ���������: ������ � ������������ ��������� � �������"),
		ERROR_ENTRY(83, "������ ���������: ������ � ��������� ���������� ������� strl ����������� ����������"),
		ERROR_ENTRY(84, "������ ���������: ������������� �������������"),
		ERROR_ENTRY(85, "������ ���������: �������������� ����� � ��������� ������������"),
		ERROR_ENTRY(86, "������ ���������: ������� ������� �������� dig-��������"), 
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