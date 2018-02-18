#pragma once
#define ARRAY_OF_ERRORS_MAX_SIZE 5
#define ERROR_THROW(id) Error::geterror(id);  //throw  ERROR_THROW(id)
#define ERROR_THROW_IN(id, line, col) Error::geterrorin(id, line, col); //throw ERROR_THROW(id, ������, �������)
#define ERROR_ENTRY(id, message) {id, message, {-1,-1}}	//������� ������� ������
#define ERROR_MAXSIZE_MESSAGE 200						//������������ ����� ��������� �������
#define ERROR_ENTRY_NODEF(id) ERROR_ENTRY(-id, "�������������� ������")  //1 �������������� ������� ������� ������

#define ERROR_ENTRY_NODEF10(id)   ERROR_ENTRY_NODEF(id+0),ERROR_ENTRY_NODEF(id+1),ERROR_ENTRY_NODEF(id+2),ERROR_ENTRY_NODEF(id+3), \
								  ERROR_ENTRY_NODEF(id+4),ERROR_ENTRY_NODEF(id+5),ERROR_ENTRY_NODEF(id+6),ERROR_ENTRY_NODEF(id+7), \
								  ERROR_ENTRY_NODEF(id+8),ERROR_ENTRY_NODEF(id+9)

#define ERROR_MAX_ENTRY 100

namespace Error
{
	struct ERROR	// ���� ���������� ��� throw ERROR_THROW | ERROR_THROW_IN � catch(ERROR)
	{
		int id;										// ��� ������
		char message[ERROR_MAXSIZE_MESSAGE];		// ��������� �� ������
		struct IN									// ���������� ��� ������ ��� ��������� ������� ������
		{
			short line;						        // ����� ������ (������ � ����)
			short col;								// ����� ������� (������ � ����)
		} intext;
	};
	struct ARRAY_OF_ERRORS							// ������ ������ ��� ����������� ���������� ������ �� ����� ������ ���������
	{
		ERROR arr_of_err[ARRAY_OF_ERRORS_MAX_SIZE];
		int size = 0;
	};

	ERROR geterror(int id);								// c����������� ERROR ��� ERROR_THROW 
	ERROR geterrorin(int id, int line, int col);		// ������������ ERROR ��� ERROR_THROW_IN
	void AddError(ARRAY_OF_ERRORS* arr, ERROR err);		// �������� ������ � ������ ������ (���� ������������ - ����������)
}