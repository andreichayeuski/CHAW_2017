#include "stdafx.h"
#include "Error.h"
#include "Parm.h"

namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		bool fl = false, fl_out = false, fl_log = false, fl_lex = false, fl_sin = false;
		PARM p;
		wchar_t *buf;
		for (int i = 1; i < argc; i++)
		{
			if (wcslen(argv[i]) >= PARM_MAX_SIZE)
			{
				throw ERROR_THROW(12);
			}
			if (buf = wcsstr(argv[i], PARM_IN))
			{
				wcscpy_s(p.in, buf + wcslen(PARM_IN));
				fl = true;
			}
			if (buf = wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(p.out, buf + wcslen(PARM_OUT));
				fl_out = true;
			}
			if (buf = wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(p.log, buf + wcslen(PARM_LOG));
				fl_log = true;
			}
			if (buf = wcsstr(argv[i], PARM_LEX))
			{
				wcscpy_s(p.log, buf + wcslen(PARM_LEX));
				fl_lex = true;
			}
			if (buf = wcsstr(argv[i], PARM_SIN))
			{
				wcscpy_s(p.in, buf + wcslen(PARM_SIN));
				fl_sin = true;
			}
			if (wcscmp(argv[i], PARM_TRACE) == 0)
			{
				p.need_trace = true;
			}
		}
		if (!fl)
		{
			throw ERROR_THROW(11);
		}
		if (!fl_out)
		{
			wcscpy_s(p.out, p.in);
			wcsncat_s(p.out, PARM_OUT_DEFAULT_EXT, wcslen(PARM_OUT_DEFAULT_EXT));
		}
		if (!fl_log)
		{
			wcscpy_s(p.log, p.in);
			wcsncat_s(p.log, PARM_LOG_DEFAULT_EXT, wcslen(PARM_LOG_DEFAULT_EXT));
		}
		if (!fl_lex)
		{
			wcscpy_s(p.lex, p.in);
			wcsncat_s(p.lex, PARM_LEX_DEFAULT_EXT, wcslen(PARM_LEX_DEFAULT_EXT));
		}
		if (!fl_sin)
		{
			wcscpy_s(p.sin, p.in);
			wcsncat_s(p.sin, PARM_SIN_DEFAULT_EXT, wcslen(PARM_SIN_DEFAULT_EXT));
		}
		return p;
	}
};