#if 0

#pragma once

class ExecEngine
{
public:
	static bool		DB_OpenA(char * file, bool readonly);
	static bool		DB_OpenW(wchar_t * file, bool readonly);
	static bool		DB_OpenT(TCHAR * file, bool readonly);
	static bool		DB_Close();
	static bool		DB_Run(char * sql);

			ExecEngine();
			ExecEngine(sqlite3 * db);
			~ExecEngine();

	void	PrepareAdd(char * sql);
	bool	PrepareDone();
	bool	Prepare(char * sql);
	bool	Run(char * sql);
	bool	Close();

	bool	BindTextA(char * value);
	bool	BindTextW(wchar_t * value);
	bool	BindTextT(TCHAR * value);
	bool	BindInt(int value);
	bool	BindInt64(int64_t value);

	int			GetColInt();
	int64_t		GetColInt64();
	char*		GetColTextA();
	wchar_t*	GetColTextW();
	TCHAR*		GetColTextT();

	bool	NextRow();
	bool	Execute();
	bool	Reset();

private:
	static sqlite3 *	s_db;
	sqlite3 *		m_db;
    sqlite3_stmt *	m_stmt;
	int				m_bind;
	char *			m_buffer;
};

#endif
