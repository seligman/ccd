#if 0

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#include "SQLite.h"
#include "ExecEngine.h"

sqlite3 * ExecEngine::s_db = NULL;

bool ExecEngine::DB_OpenT(TCHAR * filename, bool readonly)
{
#ifdef UNICODE
	return DB_OpenW(filename, readonly);
#else
	return DB_OpenA(filename, readonly);
#endif
}

bool ExecEngine::DB_OpenW(wchar_t * filename, bool readonly)
{
	bool ret = true;

	if (!s_db)
	{
		size_t len = wcslen(filename);
		int needed = WideCharToMultiByte(CP_UTF8, 0, filename, len, NULL, 0, NULL, NULL);
		needed += 2;
		char * temp = new char[needed];
		memset(temp, 0, needed);
		WideCharToMultiByte(CP_UTF8, 0, filename, len, temp, needed, NULL, NULL);

		ret = DB_OpenA(temp, readonly);

		delete[] temp;
  	}

	return ret;
}

bool ExecEngine::DB_OpenA(char * filename, bool readonly)
{
	bool ret = true;

	if (!s_db)
	{
		if (readonly)
		{
			if (sqlite3_open_v2(filename, &s_db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK)
			{
				ret = false;
			}
		}
		else
		{
			if (sqlite3_open(filename, &s_db) != SQLITE_OK)
			{
				ret = false;
			}
		}
	}

	return ret;
}

bool ExecEngine::DB_Close()
{
	bool ret = true;

	if (s_db)
	{
		if (sqlite3_close(s_db) != SQLITE_OK)
		{
			ret = false;
		}

		s_db = NULL;
	}

	return ret;
}

ExecEngine::ExecEngine(sqlite3 * db)
{
	m_stmt = NULL;
	m_db = db;
	m_bind = 1;
	m_buffer = NULL;
}

ExecEngine::ExecEngine()
{
	m_stmt = NULL;
	m_db = s_db;
	m_bind = 1;
	m_buffer = NULL;
}

bool ExecEngine::BindTextA(char * value)
{
	int rc = sqlite3_bind_text(m_stmt, m_bind, value, -1, SQLITE_TRANSIENT);
	m_bind ++;
	return rc == SQLITE_OK;
}

bool ExecEngine::BindTextW(wchar_t * value)
{
	int rc = sqlite3_bind_text16(m_stmt, m_bind, value, -1, SQLITE_TRANSIENT);
	m_bind ++;
	return rc == SQLITE_OK;
}

bool ExecEngine::BindTextT(TCHAR * value)
{
#ifdef UNICODE
	int rc = sqlite3_bind_text16(m_stmt, m_bind, value, -1, SQLITE_TRANSIENT);
#else
	int rc = sqlite3_bind_text(m_stmt, m_bind, value, -1, SQLITE_TRANSIENT);
#endif
	m_bind ++;
	return rc == SQLITE_OK;
}

bool ExecEngine::BindInt64(int64_t value)
{
	int rc = sqlite3_bind_int64(m_stmt, m_bind, value);
	m_bind ++;
	return rc == SQLITE_OK;
}

bool ExecEngine::BindInt(int value)
{
	int rc = sqlite3_bind_int64(m_stmt, m_bind, value);
	m_bind ++;
	return rc == SQLITE_OK;
}

bool ExecEngine::DB_Run(char * sql)
{
	ExecEngine temp;
	return temp.Run(sql);
}

bool ExecEngine::Run(char * sql)
{
	bool ret = true;

	if (ret)
	{
		ret = Prepare(sql);
	}

	if (ret)
	{
		ret = Execute();
	}

	if (ret)
	{
		ret = Close();
	}

	return ret;
}

bool ExecEngine::Execute()
{
	while (NextRow())
	{
		// Nothing to do;
	}

	return true;
}

bool ExecEngine::NextRow()
{
	while (m_stmt)
	{
		int rc = sqlite3_step(m_stmt);

		if (rc == SQLITE_ROW)
		{
			m_bind = 0;
			return true;
		}

		if (rc == SQLITE_DONE)
		{
			return false;
		}
	}

	return false;
}

ExecEngine::~ExecEngine()
{
	Close();
}

bool ExecEngine::Close()
{
	if (m_stmt)
	{
		sqlite3_finalize(m_stmt);
		m_stmt = NULL;
	}
	return true;
}

char* ExecEngine::GetColTextA()
{
	char* ret = (char*)sqlite3_column_text(m_stmt, m_bind);
	m_bind++;
	return ret;
}

wchar_t* ExecEngine::GetColTextW()
{
	wchar_t* ret = (wchar_t*)sqlite3_column_text16(m_stmt, m_bind);
	m_bind++;
	return ret;
}

TCHAR* ExecEngine::GetColTextT()
{
#ifdef UNICODE
	wchar_t* ret = (wchar_t*)sqlite3_column_text16(m_stmt, m_bind);
#else
	char* ret = (char*)sqlite3_column_text(m_stmt, m_bind);
#endif
	m_bind++;
	return ret;
}

int64_t ExecEngine::GetColInt64()
{
	int64_t ret = sqlite3_column_int64(m_stmt, m_bind);
	m_bind++;
	return ret;
}

int ExecEngine::GetColInt()
{
	int ret = sqlite3_column_int(m_stmt, m_bind);
	m_bind++;
	return ret;
}

void ExecEngine::PrepareAdd(char * sql)
{
	if (m_buffer)
	{
		size_t len = strlen(sql) + strlen(m_buffer) + 1;
		char * temp = new char[len];
		strcpy_s(temp, len, m_buffer);
		strcat_s(temp, len, sql);
		delete[] m_buffer;
		m_buffer = temp;
	}
	else
	{
		size_t len = strlen(sql) + 1;
		char * temp = new char[len];
		strcpy_s(temp, len, sql);
		m_buffer = temp;
	}
}

bool ExecEngine::PrepareDone()
{
#ifdef _DEBUG
	FILE* f = NULL;
	fopen_s(&f, "queries.txt", "a");
	fwrite(m_buffer, 1, strlen(m_buffer), f);
	fwrite("\n", 1, strlen("\n"), f);
	fclose(f);
#endif
	bool ret = Prepare(m_buffer);
	delete[] m_buffer;
	m_buffer = NULL;
	return ret;
}

bool ExecEngine::Prepare(char * sql)
{
	Close();

	int rc = sqlite3_prepare(m_db, sql, -1, &m_stmt, NULL);
	m_bind = 1;
	return rc == SQLITE_OK;
}

bool ExecEngine::Reset()
{
	int rc = SQLITE_OK;
	
	if (rc == SQLITE_OK)
	{
		rc = sqlite3_reset(m_stmt);
	}

	if (rc == SQLITE_OK)
	{
		rc = sqlite3_clear_bindings(m_stmt);
	}

	m_bind = 1;

	return rc == SQLITE_OK;
}


#endif
