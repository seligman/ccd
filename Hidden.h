WCHAR* GetHiddenW(int index);
char* GetHiddenA(int index);
#define STR_ADJUSTTOKENPRIVILEGES 1
#define HIDEA_ADJUSTTOKENPRIVILEGES GetHiddenA(STR_ADJUSTTOKENPRIVILEGES)
#define STR_ADVAPI32 2
#define HIDEW_ADVAPI32 GetHiddenW(STR_ADVAPI32)
#define STR_ALLDIRS 3
#define HIDEW_ALLDIRS GetHiddenW(STR_ALLDIRS)
#define STR_ATCCDDOTDOT 4
#define HIDEA_ATCCDDOTDOT GetHiddenA(STR_ATCCDDOTDOT)
#define STR_CANTWRITE64 5
#define HIDEW_CANTWRITE64 GetHiddenW(STR_CANTWRITE64)
#define STR_CAPPERCS 6
#define HIDEW_CAPPERCS GetHiddenW(STR_CAPPERCS)
#define STR_CCDDOT64DOTEXE 7
#define HIDEW_CCDDOT64DOTEXE GetHiddenW(STR_CCDDOT64DOTEXE)
#define STR_CHANGETO 8
#define HIDEW_CHANGETO GetHiddenW(STR_CHANGETO)
#define STR_CHANGETOPARAM 9
#define HIDEW_CHANGETOPARAM GetHiddenW(STR_CHANGETOPARAM)
#define STR_CLOSEHANDLE 10
#define HIDEA_CLOSEHANDLE GetHiddenA(STR_CLOSEHANDLE)
#define STR_CMDEXE 11
#define HIDEW_CMDEXE GetHiddenW(STR_CMDEXE)
#define STR_CREATEREMOTETHREAD 12
#define HIDEA_CREATEREMOTETHREAD GetHiddenA(STR_CREATEREMOTETHREAD)
#define STR_CRUSTY 13
#define HIDEW_CRUSTY GetHiddenW(STR_CRUSTY)
#define STR_DASHC 14
#define HIDEW_DASHC GetHiddenW(STR_DASHC)
#define STR_DASHF 15
#define HIDEW_DASHF GetHiddenW(STR_DASHF)
#define STR_DASHH 16
#define HIDEW_DASHH GetHiddenW(STR_DASHH)
#define STR_DASHN 17
#define HIDEW_DASHN GetHiddenW(STR_DASHN)
#define STR_DASHQUES 18
#define HIDEW_DASHQUES GetHiddenW(STR_DASHQUES)
#define STR_DASHR 19
#define HIDEW_DASHR GetHiddenW(STR_DASHR)
#define STR_DASHS 20
#define HIDEW_DASHS GetHiddenW(STR_DASHS)
#define STR_DASHU 21
#define HIDEW_DASHU GetHiddenW(STR_DASHU)
#define STR_DATAFILES 22
#define HIDEW_DATAFILES GetHiddenW(STR_DATAFILES)
#define STR_DONE 23
#define HIDEW_DONE GetHiddenW(STR_DONE)
#define STR_DOT 24
#define HIDEW_DOT GetHiddenW(STR_DOT)
#define HIDEA_DOT GetHiddenA(STR_DOT)
#define STR_DOT64DOTEXE 25
#define HIDEW_DOT64DOTEXE GetHiddenW(STR_DOT64DOTEXE)
#define STR_DOTCMD 26
#define HIDEW_DOTCMD GetHiddenW(STR_DOTCMD)
#define STR_DOTDOT 27
#define HIDEW_DOTDOT GetHiddenW(STR_DOTDOT)
#define HIDEA_DOTDOT GetHiddenA(STR_DOTDOT)
#define STR_ENUMPROCESSMODULES 28
#define HIDEA_ENUMPROCESSMODULES GetHiddenA(STR_ENUMPROCESSMODULES)
#define STR_EQUALS 29
#define HIDEW_EQUALS GetHiddenW(STR_EQUALS)
#define STR_FILTERED 30
#define HIDEW_FILTERED GetHiddenW(STR_FILTERED)
#define STR_FILTEREDMATCHES 31
#define HIDEW_FILTEREDMATCHES GetHiddenW(STR_FILTEREDMATCHES)
#define STR_GETCURRENTDIRECTORYW 32
#define HIDEA_GETCURRENTDIRECTORYW GetHiddenA(STR_GETCURRENTDIRECTORYW)
#define STR_GETCURRENTPROCESS 33
#define HIDEA_GETCURRENTPROCESS GetHiddenA(STR_GETCURRENTPROCESS)
#define STR_GETMODULEFILENAMEEXW 34
#define HIDEA_GETMODULEFILENAMEEXW GetHiddenA(STR_GETMODULEFILENAMEEXW)
#define STR_GETPROCADDRESS 35
#define HIDEA_GETPROCADDRESS GetHiddenA(STR_GETPROCADDRESS)
#define STR_HELP 36
#define HIDEW_HELP GetHiddenW(STR_HELP)
#define STR_HELPERDONE 37
#define HIDEW_HELPERDONE GetHiddenW(STR_HELPERDONE)
#define STR_IGNOREDIRS 38
#define HIDEW_IGNOREDIRS GetHiddenW(STR_IGNOREDIRS)
#define STR_ISWOW64PROCESS 39
#define HIDEA_ISWOW64PROCESS GetHiddenA(STR_ISWOW64PROCESS)
#define STR_ISWOW64PROCESS2 40
#define HIDEA_ISWOW64PROCESS2 GetHiddenA(STR_ISWOW64PROCESS2)
#define STR_KERNEL32 41
#define HIDEW_KERNEL32 GetHiddenW(STR_KERNEL32)
#define STR_LINEFEED 42
#define HIDEW_LINEFEED GetHiddenW(STR_LINEFEED)
#define STR_LOOKUPPRIVILEGEVALUEW 43
#define HIDEA_LOOKUPPRIVILEGEVALUEW GetHiddenA(STR_LOOKUPPRIVILEGEVALUEW)
#define STR_MINUS 44
#define HIDEW_MINUS GetHiddenW(STR_MINUS)
#define STR_MUTEXNAME 45
#define HIDEW_MUTEXNAME GetHiddenW(STR_MUTEXNAME)
#define STR_NETWORKPATH 46
#define HIDEW_NETWORKPATH GetHiddenW(STR_NETWORKPATH)
#define STR_NEWLINE 47
#define HIDEA_NEWLINE GetHiddenA(STR_NEWLINE)
#define STR_NOALLDIRS 48
#define HIDEW_NOALLDIRS GetHiddenW(STR_NOALLDIRS)
#define STR_NOALLDIRSINFO 49
#define HIDEW_NOALLDIRSINFO GetHiddenW(STR_NOALLDIRSINFO)
#define STR_NOMATCHES 50
#define HIDEW_NOMATCHES GetHiddenW(STR_NOMATCHES)
#define STR_NOMATCHESINPARENS 51
#define HIDEW_NOMATCHESINPARENS GetHiddenW(STR_NOMATCHESINPARENS)
#define STR_NOTCMD 52
#define HIDEW_NOTCMD GetHiddenW(STR_NOTCMD)
#define STR_OPENPROCESS 53
#define HIDEA_OPENPROCESS GetHiddenA(STR_OPENPROCESS)
#define STR_OPENPROCESSTOKEN 54
#define HIDEA_OPENPROCESSTOKEN GetHiddenA(STR_OPENPROCESSTOKEN)
#define STR_PERC03D 55
#define HIDEW_PERC03D GetHiddenW(STR_PERC03D)
#define STR_PERCD 56
#define HIDEW_PERCD GetHiddenW(STR_PERCD)
#define STR_PREVINST 57
#define HIDEW_PREVINST GetHiddenW(STR_PREVINST)
#define STR_PSAPI 58
#define HIDEW_PSAPI GetHiddenW(STR_PSAPI)
#define STR_QUOTE 59
#define HIDEW_QUOTE GetHiddenW(STR_QUOTE)
#define STR_QUOTESPACE 60
#define HIDEW_QUOTESPACE GetHiddenW(STR_QUOTESPACE)
#define STR_READPROCESSMEMORY 61
#define HIDEA_READPROCESSMEMORY GetHiddenA(STR_READPROCESSMEMORY)
#define STR_SCANNING 62
#define HIDEW_SCANNING GetHiddenW(STR_SCANNING)
#define STR_SETCURRENTDIRECTORYW 63
#define HIDEA_SETCURRENTDIRECTORYW GetHiddenA(STR_SETCURRENTDIRECTORYW)
#define STR_SETENVIRONMENTVARIABLEW 64
#define HIDEA_SETENVIRONMENTVARIABLEW GetHiddenA(STR_SETENVIRONMENTVARIABLEW)
#define STR_SETTINGSPATH 65
#define HIDEW_SETTINGSPATH GetHiddenW(STR_SETTINGSPATH)
#define STR_SHORTCUTDIRS 66
#define HIDEW_SHORTCUTDIRS GetHiddenW(STR_SHORTCUTDIRS)
#define STR_SLASH 67
#define HIDEW_SLASH GetHiddenW(STR_SLASH)
#define HIDEA_SLASH GetHiddenA(STR_SLASH)
#define STR_SLASHC 68
#define HIDEW_SLASHC GetHiddenW(STR_SLASHC)
#define STR_SLASHCCDDOTDOT 69
#define HIDEW_SLASHCCDDOTDOT GetHiddenW(STR_SLASHCCDDOTDOT)
#define STR_SLASHCCDSLASH 70
#define HIDEW_SLASHCCDSLASH GetHiddenW(STR_SLASHCCDSLASH)
#define STR_SLASHF 71
#define HIDEW_SLASHF GetHiddenW(STR_SLASHF)
#define STR_SLASHH 72
#define HIDEW_SLASHH GetHiddenW(STR_SLASHH)
#define STR_SLASHN 73
#define HIDEW_SLASHN GetHiddenW(STR_SLASHN)
#define STR_SLASHQUES 74
#define HIDEW_SLASHQUES GetHiddenW(STR_SLASHQUES)
#define STR_SLASHR 75
#define HIDEW_SLASHR GetHiddenW(STR_SLASHR)
#define STR_SLASHS 76
#define HIDEW_SLASHS GetHiddenW(STR_SLASHS)
#define STR_SLASHSTAR 77
#define HIDEA_SLASHSTAR GetHiddenA(STR_SLASHSTAR)
#define STR_SLASHU 78
#define HIDEW_SLASHU GetHiddenW(STR_SLASHU)
#define STR_SPACE 79
#define HIDEW_SPACE GetHiddenW(STR_SPACE)
#define STR_SPACESPACE 80
#define HIDEW_SPACESPACE GetHiddenW(STR_SPACESPACE)
#define STR_STATBLOCK 81
#define HIDEW_STATBLOCK GetHiddenW(STR_STATBLOCK)
#define STR_STATERROR 82
#define HIDEW_STATERROR GetHiddenW(STR_STATERROR)
#define STR_UNABLETOGET 83
#define HIDEW_UNABLETOGET GetHiddenW(STR_UNABLETOGET)
#define STR_UNABLETOSET 84
#define HIDEW_UNABLETOSET GetHiddenW(STR_UNABLETOSET)
#define STR_UNABLETOSETENV 85
#define HIDEW_UNABLETOSETENV GetHiddenW(STR_UNABLETOSETENV)
#define STR_VIRTUALALLOCEX 87
#define HIDEA_VIRTUALALLOCEX GetHiddenA(STR_VIRTUALALLOCEX)
#define STR_VIRTUALFREEEX 88
#define HIDEA_VIRTUALFREEEX GetHiddenA(STR_VIRTUALFREEEX)
#define STR_WAITFORSINGLEOBJECT 89
#define HIDEA_WAITFORSINGLEOBJECT GetHiddenA(STR_WAITFORSINGLEOBJECT)
#define STR_WRITEPROCESSMEMORY 90
#define HIDEA_WRITEPROCESSMEMORY GetHiddenA(STR_WRITEPROCESSMEMORY)
