#include "main.h"
#include "Hidden.h"
WCHAR* GetHiddenW(int index)
{
    WCHAR* buffer;
    CHAR* pad1;
    CHAR* pad2;
    int len = 0;
    int i = 0;
    switch (index)
    {

        /* Advapi32.dll */
    case STR_ADVAPI32:
        {
            static WCHAR localBuffer[13];
            len = 12;
            pad1 = "\x1a\x9b\x46\xf5\xe0\xc7\x7c\xec\xd2\xd9\xd9\xfc";
            pad2 = "\x5b\xff\x30\x94\x90\xae\x4f\xde\xfc\xbd\xb5\x90";
            buffer = localBuffer;
        }
        break;

        /* \AllDirs.db */
    case STR_ALLDIRS:
        {
            static WCHAR localBuffer[12];
            len = 11;
            pad1 = "\x06\x85\xb9\xb6\x35\xd5\xfb\xd8\xe2\x26\x7c";
            pad2 = "\x5a\xc4\xd5\xda\x71\xbc\x89\xab\xcc\x42\x1e";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Unable to write 64-bit executable, verify CCD.exe is running in writeable folder
         *  */
    case STR_CANTWRITE64:
        {
            static WCHAR localBuffer[89];
            len = 88;
            pad1 = "\x11\x1c\x1f\x23\x1c\x94\xe0\xd9\x21\xe3\xf6\x12\xa8\x6c\x53\xcf\xd4\xa2\x69\x3e"
                   "\x7b\xeb\xb8\x68\xde\x0a\x60\x88\x0c\x84\xaa\x0f\x64\xae\xae\x64\xf3\x5f\x9d\x0e"
                   "\xc9\x79\xff\x17\xfa\x72\x43\x22\xae\x77\x53\x08\x1f\x6c\x0f\x26\x63\xb6\x45\x11"
                   "\x33\x57\xcc\x9d\x39\xe6\xae\xfa\xbe\xee\xdb\x74\x04\x5b\x74\x01\xeb\xa1\x6e\x6b"
                   "\xc7\x3e\xd9\x94\x31\xa1\xf0\xf7";
            pad2 = "\x54\x4e\x4d\x6c\x4e\xae\xc0\x8c\x4f\x82\x94\x7e\xcd\x4c\x27\xa0\xf4\xd5\x1b\x57"
                   "\x0f\x8e\x98\x5e\xea\x27\x02\xe1\x78\xa4\xcf\x77\x01\xcd\xdb\x10\x92\x3d\xf1\x6b"
                   "\xe5\x59\x89\x72\x88\x1b\x25\x5b\x8e\x34\x10\x4c\x31\x09\x77\x43\x43\xdf\x36\x31"
                   "\x41\x22\xa2\xf3\x50\x88\xc9\xda\xd7\x80\xfb\x03\x76\x32\x00\x64\x8a\xc3\x02\x0e"
                   "\xe7\x58\xb6\xf8\x55\xc4\x82\xfd";
            buffer = localBuffer;
        }
        break;

        /* %S */
    case STR_CAPPERCS:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x90\x90";
            pad2 = "\xb5\xc3";
            buffer = localBuffer;
        }
        break;

        /* ccd.64.exe */
    case STR_CCDDOT64DOTEXE:
        {
            static WCHAR localBuffer[11];
            len = 10;
            pad1 = "\x97\x67\xd2\x5a\x8f\x74\x53\x59\x41\x41";
            pad2 = "\xf4\x04\xb6\x74\xb9\x40\x7d\x3c\x39\x24";
            buffer = localBuffer;
        }
        break;

        /* Change folder to:  */
    case STR_CHANGETO:
        {
            static WCHAR localBuffer[19];
            len = 18;
            pad1 = "\xfb\x98\x28\x96\x8d\xa2\x1a\x74\xed\x1e\x60\x41\xf7\x33\x6c\x28\x4f\xb1";
            pad2 = "\xb8\xf0\x49\xf8\xea\xc7\x3a\x12\x82\x72\x04\x24\x85\x13\x18\x47\x75\x91";
            buffer = localBuffer;
        }
        break;

        /* Change folder to: %s
         *  */
    case STR_CHANGETOPARAM:
        {
            static WCHAR localBuffer[22];
            len = 21;
            pad1 = "\x07\x2e\x8e\x03\x74\x62\x00\x75\x2c\xdb\xd9\xd1\x57\x3e\xd2\x67\x26\xef\x2c\x69"
                   "\x1e";
            pad2 = "\x44\x46\xef\x6d\x13\x07\x20\x13\x43\xb7\xbd\xb4\x25\x1e\xa6\x08\x1c\xcf\x09\x1a"
                   "\x14";
            buffer = localBuffer;
        }
        break;

        /* cmd.exe */
    case STR_CMDEXE:
        {
            static WCHAR localBuffer[8];
            len = 7;
            pad1 = "\x1d\xb2\x85\x91\x03\x53\xc5";
            pad2 = "\x7e\xdf\xe1\xbf\x66\x2b\xa0";
            buffer = localBuffer;
        }
        break;

        /* WARNING: "\AllDirs.db" is older than 2 months, suggest running "CCD -r"
         *  */
    case STR_CRUSTY:
        {
            static WCHAR localBuffer[73];
            len = 72;
            pad1 = "\xa3\xfd\x37\xc7\x76\x33\x38\x5a\x80\xfe\x2b\x6a\xb8\xfc\x6c\xaf\x16\x7e\x7e\x67"
                   "\xe1\xa7\x3a\x75\x29\xf8\xf5\x64\xe0\xef\xc5\xda\x99\xc1\xba\x85\x70\x6a\x20\xc2"
                   "\x16\x4f\xb7\x6e\x8f\x17\x49\x2e\xfa\xdd\xac\x63\x4d\x08\xa8\xfb\xc3\xc2\x9e\x7e"
                   "\x3e\xd3\x79\x91\x38\x51\xd0\xf6\x41\xe7\x3f\xac";
            pad2 = "\xf4\xbc\x65\x89\x3f\x7d\x7f\x60\xa0\xdc\x77\x2b\xd4\x90\x28\xc6\x64\x0d\x50\x03"
                   "\x83\x85\x1a\x1c\x5a\xd8\x9a\x08\x84\x8a\xb7\xfa\xed\xa9\xdb\xeb\x50\x58\x00\xaf"
                   "\x79\x21\xc3\x06\xfc\x3b\x69\x5d\x8f\xba\xcb\x06\x3e\x7c\x88\x89\xb6\xac\xf0\x17"
                   "\x50\xb4\x59\xb3\x7b\x12\x94\xd6\x6c\x95\x1d\xa6";
            buffer = localBuffer;
        }
        break;

        /* -c */
    case STR_DASHC:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xf7\xd8";
            pad2 = "\xda\xbb";
            buffer = localBuffer;
        }
        break;

        /* -f */
    case STR_DASHF:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x03\x6e";
            pad2 = "\x2e\x08";
            buffer = localBuffer;
        }
        break;

        /* -h */
    case STR_DASHH:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xd5\x16";
            pad2 = "\xf8\x7e";
            buffer = localBuffer;
        }
        break;

        /* -n */
    case STR_DASHN:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x79\x0c";
            pad2 = "\x54\x62";
            buffer = localBuffer;
        }
        break;

        /* -? */
    case STR_DASHQUES:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xe0\xac";
            pad2 = "\xcd\x93";
            buffer = localBuffer;
        }
        break;

        /* -r */
    case STR_DASHR:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xec\x42";
            pad2 = "\xc1\x30";
            buffer = localBuffer;
        }
        break;

        /* -s */
    case STR_DASHS:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xe8\x2b";
            pad2 = "\xc5\x58";
            buffer = localBuffer;
        }
        break;

        /* -u */
    case STR_DASHU:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x0e\x05";
            pad2 = "\x23\x70";
            buffer = localBuffer;
        }
        break;

        /* DataFiles */
    case STR_DATAFILES:
        {
            static WCHAR localBuffer[10];
            len = 9;
            pad1 = "\x28\x48\xea\xfe\xea\x13\xab\xda\xa0";
            pad2 = "\x6c\x29\x9e\x9f\xac\x7a\xc7\xbf\xd3";
            buffer = localBuffer;
        }
        break;

        /* \nDone\n */
    case STR_DONE:
        {
            static WCHAR localBuffer[7];
            len = 6;
            pad1 = "\xae\x93\x9d\x34\x5e\xf3";
            pad2 = "\xa4\xd7\xf2\x5a\x3b\xf9";
            buffer = localBuffer;
        }
        break;

        /* . */
    case STR_DOT:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\x34";
            pad2 = "\x1a";
            buffer = localBuffer;
        }
        break;

        /* .64.exe */
    case STR_DOT64DOTEXE:
        {
            static WCHAR localBuffer[8];
            len = 7;
            pad1 = "\xba\x29\x03\xa2\x45\xb2\x03";
            pad2 = "\x94\x1f\x37\x8c\x20\xca\x66";
            buffer = localBuffer;
        }
        break;

        /* .cmd */
    case STR_DOTCMD:
        {
            static WCHAR localBuffer[5];
            len = 4;
            pad1 = "\x3f\x75\xb6\xd8";
            pad2 = "\x11\x16\xdb\xbc";
            buffer = localBuffer;
        }
        break;

        /* .. */
    case STR_DOTDOT:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xc5\xc0";
            pad2 = "\xeb\xee";
            buffer = localBuffer;
        }
        break;

        /* = */
    case STR_EQUALS:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\x4b";
            pad2 = "\x76";
            buffer = localBuffer;
        }
        break;

        /* %d matches */
    case STR_FILTERED:
        {
            static WCHAR localBuffer[11];
            len = 10;
            pad1 = "\x40\x1d\x8b\x25\xf8\x1f\x7b\x79\x69\x8b";
            pad2 = "\x65\x79\xab\x48\x99\x6b\x18\x11\x0c\xf8";
            buffer = localBuffer;
        }
        break;

        /* %d matches, filtered with "%s" */
    case STR_FILTEREDMATCHES:
        {
            static WCHAR localBuffer[31];
            len = 30;
            pad1 = "\xba\xd2\xd8\xef\x85\x3f\x08\xf9\x49\x2d\x7a\xd8\x87\xec\x1f\x06\xef\x83\x6a\xe5"
                   "\x50\x12\x39\x42\x1e\xa3\x00\x62\x7d\x1d";
            pad2 = "\x9f\xb6\xf8\x82\xe4\x4b\x6b\x91\x2c\x5e\x56\xf8\xe1\x85\x73\x72\x8a\xf1\x0f\x81"
                   "\x70\x65\x50\x36\x76\x83\x22\x47\x0e\x3f";
            buffer = localBuffer;
        }
        break;

        /* 
         *   Cursorial Change Directory
         * 
         *     Usage: ccd "partial directory name"
         * 
         *   .. = Go up one level (... for two levels, etc)
         *   -c = Create ccd helper batch files for .., ..., etc
         *   -f = Don't use smart directory matching
         *   -n = Don't change folder, just show matches
         *   -r = Force a rescan of all directories
         *   -s = Show stats about the AllDirs.db file
         *   -u = Only consider directories under this one
         * 
         *     Data files:
         * 
         *   AllDirs.db       = Database of all dirs, created during a "-r" scan
         *   IgnoreDirs.txt   = Directories to ignore during a "-r" scan
         *   ShortcutDirs.txt = Each line contains a shortcut of the 
         *                      format "shortcut=fullpath"
         * 
         *     Data directory:
         *     
         *   The first directory found of the following list will be used to
         *   store data files for the current drive:
         *   
         *     "%s"
         *     "%c:%s"
         *     "%c:\"
         * 
         *   Any characters pressed during the display of potential directories
         *   will be used to filter the list.
         * 
         *     Build 38, created by Scott Seligman
         *               <scott@scottandmichelle.net>
         * 
         *  */
    case STR_HELP:
        {
            static WCHAR localBuffer[1013];
            len = 1012;
            pad1 = "\xd1\x56\xcf\x7c\x13\x31\xce\x5d\x7e\xf1\x10\xa8\x38\x26\x06\xae\x73\x74\xb6\x80"
                   "\xf1\x9e\x94\x40\xf2\x1e\xd6\x59\x16\x60\xb9\xf5\xd0\xf5\x85\xde\x10\xde\x7f\x4d"
                   "\x3b\x88\x4b\x55\xc7\x22\x01\x4e\xc2\xeb\x9a\xfe\x1e\xaa\x43\x33\xc2\x8e\x8a\xf5"
                   "\x5b\x74\x47\x1d\x38\x1b\xd8\x43\xa0\x27\x9d\x95\xf4\x69\x45\x69\x53\x3e\x79\xcf"
                   "\x1c\x87\x97\x2a\xc6\xbb\xd7\x26\xb1\x52\x1c\x81\x53\x8a\xf1\xc6\x40\x2d\x1d\xac"
                   "\x4a\xbe\xae\x9a\x4e\x04\x09\xb5\x64\xd9\xdf\x83\x15\xbb\xd9\xd3\xdc\x1c\x57\x7c"
                   "\x43\xb1\x91\x4d\x0b\x1c\x42\x6a\x77\x7e\x28\x25\x20\xcd\x84\x5d\xd9\xfd\xdc\xc1"
                   "\xb2\x22\xc1\xd8\x72\x98\x6c\x97\xb0\x1e\xea\x23\xd5\xa1\x68\x98\x2c\x60\x8e\x31"
                   "\xc2\x9e\x3d\xe6\xb1\xba\x63\x47\x36\x7b\xfa\xdf\x40\x96\xca\x21\xef\xb9\xda\x72"
                   "\xaf\xaa\xb9\x59\x94\x04\x50\x7f\x65\xeb\x30\x78\x6e\xaa\xfe\x13\xe4\xdb\x26\xb8"
                   "\x35\x24\x61\x35\x23\x0a\xe3\x3f\xd6\x47\xa8\x73\x86\x1b\x82\x18\xe4\xe4\x51\xad"
                   "\x6a\xe4\xe2\x4a\x4a\x81\x70\x95\x97\xb7\x3d\x14\x92\x21\x3d\x8a\x48\x84\x35\x89"
                   "\x20\x10\xa8\x1d\x43\xbd\xf2\x53\x28\xe6\x2e\xff\x8a\x79\x4e\x50\x42\x16\xea\xa2"
                   "\x9a\x4e\xa8\x1f\x0a\x94\xe0\x64\xdd\xf7\xcf\x60\xaf\xc7\x8c\x59\x3c\xc4\x07\x8d"
                   "\x8e\x1d\x6d\xb0\x88\x3d\x8a\x18\xd3\xed\x48\x32\x9a\x92\xef\xd2\x67\x88\x13\xae"
                   "\xdc\x8e\x75\x31\x16\xce\x50\xc2\xed\xf4\x14\x7d\x0e\x5d\x13\x7c\xe0\xf7\x96\xcc"
                   "\x0a\xc8\x90\xf0\xae\x9d\xc5\x1c\xd7\xb9\xf1\xb3\xb0\x59\xb0\x00\xa8\x3c\x9a\xba"
                   "\xed\xc7\x2b\xde\xd8\xcb\x23\x3c\x7e\x72\xf1\x70\xf1\x47\xa4\x14\xfd\x84\x82\x79"
                   "\x08\x33\xea\x33\x03\x60\xcc\xb5\xac\x13\xd4\xa4\x5d\xdc\xdf\x42\x9c\xb2\x17\xe3"
                   "\x7a\x89\x9e\x65\xc7\x81\xc2\x69\xb4\x9c\x03\x23\xb9\x20\xb1\xb9\xdc\xf7\xdb\x77"
                   "\xfe\x6d\x8f\x6b\x9e\x4d\x12\x14\x67\x61\x77\x4a\x6c\x9e\xff\x7f\x95\x85\x6b\x81"
                   "\x9e\x63\x00\x10\x0f\xb1\xeb\x3d\x2a\x26\x66\xa4\xd6\xa0\xe5\x0c\xf9\x32\xc6\x7a"
                   "\x6f\xad\x02\x53\x24\xb8\xb6\x1a\x21\x32\x24\x56\x8b\x5a\xd9\x9a\x6c\x7f\x5c\xec"
                   "\x61\xd5\xea\x41\xfa\xd3\x74\xcc\xb9\xf2\x86\x62\xbe\xf1\x11\xf1\x61\xad\xec\x56"
                   "\xfa\xd0\x1b\xf8\x01\x0f\x18\x7c\xc7\x4a\x9f\x79\xd6\xda\x19\x9a\xc3\xc0\x59\x51"
                   "\x44\xea\x60\x2e\xac\xc1\x97\x9a\x48\xe7\x39\xbf\x93\x05\xf2\x90\xb9\xf1\x3f\xea"
                   "\xba\xf7\x05\x6f\x53\x0c\x8c\xe4\xd8\x77\x2e\xa0\x54\x9a\x12\x10\x6f\xec\x4c\x3f"
                   "\x78\x7d\x33\xdf\x10\xdf\xe6\xd1\xe7\x88\xe0\xd1\x80\x6e\x96\x4c\xe4\xf1\xdb\xa0"
                   "\x35\xb3\xc2\xd1\xb8\x99\x6b\xf8\xf3\xe1\x21\xcc\xa5\x00\x72\x3c\xc4\x25\x5e\x73"
                   "\x25\xbb\xfc\x4f\x96\x5c\x1f\xe5\x04\xc5\x2b\x7e\x11\xac\x0a\xc4\xbb\x62\x26\x77"
                   "\x74\xee\xde\x05\x67\x13\x2c\x80\xfb\x59\x88\xbe\x92\x68\x7a\x7a\xb6\x72\x3a\x5c"
                   "\x79\x85\xf3\x2d\xb6\xa2\xbb\xf9\xf6\xad\x81\x61\xff\xe7\xff\x37\xdd\xf7\xe8\x82"
                   "\x30\x9f\xea\xab\x3d\x6d\xec\xa8\x31\x8f\x0e\x48\x28\xca\xfe\xee\xfa\xb0\x4f\xc4"
                   "\x58\x33\x8a\x4d\xd7\x27\xbf\xaa\x69\x43\x9d\xbf\x89\xb7\x2b\xd9\x9b\x48\xf0\x48"
                   "\xb6\x12\xc7\x66\x9f\x39\x39\x34\xe9\x10\x3d\x2c\xa7\x24\x2a\xfd\x15\x9d\xd3\x16"
                   "\xad\x41\x3f\xee\xf2\x4f\xbe\x70\x9e\x24\x52\xb1\xc0\x07\x0e\x45\x6b\x23\xae\x9e"
                   "\xed\x8a\xc1\x58\x05\x70\x18\x65\x8c\x2e\xc4\xfe\x75\xda\x9a\x74\xd2\xd9\x74\x4f"
                   "\x7a\xc8\x96\xc4\xbd\x8b\x6c\x9f\x16\xe5\x91\xf1\xef\x34\x28\x23\x7f\xe7\xe8\x81"
                   "\x37\x59\xe3\x6f\x60\x54\x3c\xe5\x6c\x99\x72\x92\xaf\x5e\x24\x24\xf4\xfc\x9a\x7a"
                   "\x04\x78\x4f\xa7\x9d\xd3\x0e\x40\xb1\x77\xf5\xeb\xf0\xf2\x17\x21\xe5\x26\x64\x68"
                   "\x4e\x86\x33\x7c\x72\xff\x41\x90\x10\x03\x2f\x82\x4c\x6d\x7c\xbf\x09\x3c\xd2\x8c"
                   "\x45\xfc\x33\xf5\xa4\x87\xa1\xbc\x46\x03\x3f\xcd\x98\x31\x2b\x7d\x44\xd2\x35\x9d"
                   "\x91\x7e\x30\xae\x47\x72\x9e\x82\x75\x58\x17\xa9\x53\xd7\x23\x52\x52\x3e\xd8\xeb"
                   "\x5d\x84\x49\x0d\xc4\x61\xea\x1b\x42\xfe\x7a\x37\x07\x35\xfb\xc7\xcb\x85\xad\x31"
                   "\xe8\x1e\x46\xed\xeb\x96\x7a\xc0\x0e\xd9\x2b\x06\xfa\x49\x3f\x0c\x34\x23\xea\xe6"
                   "\x88\x83\x40\x76\xde\xe0\xa1\x1e\xdc\x5b\x87\xcd\x90\xa7\x02\x3f\x3d\x5b\x21\x2d"
                   "\xe7\x29\x0c\x68\xd3\x73\x3d\xbd\xf5\xee\x92\xa8\x9b\xc7\xa9\x65\xde\xd0\x0f\x0b"
                   "\xf4\x5b\xe3\xb2\x52\x5d\x76\x52\x46\x00\xcf\xd8\x02\xbc\xfe\x5e\x76\xd7\xa3\x6a"
                   "\xa3\xe4\x28\xee\xc8\xde\x31\xc9\xfd\x9d\x60\x6d\x4b\x15\x39\x8b\x45\x59\x16\x76"
                   "\xfd\xea\x76\xf4\x1e\xb9\xa6\xdf\xc8\x4a\xee\x06\x2c\xa5\x68\x0f\x32\xf5\xea\x73"
                   "\x2b\xad\x55\xa9\x9f\x61\x8c\xec\x83\x72\x18\x1c";
            pad2 = "\xdb\x76\xef\x3f\x66\x43\xbd\x32\x0c\x98\x71\xc4\x18\x65\x6e\xcf\x1d\x13\xd3\xa0"
                   "\xb5\xf7\xe6\x25\x91\x6a\xb9\x2b\x6f\x6a\xb3\xd5\xf0\xd5\xa5\x8b\x63\xbf\x18\x28"
                   "\x01\xa8\x28\x36\xa3\x02\x23\x3e\xa3\x99\xee\x97\x7f\xc6\x63\x57\xab\xfc\xef\x96"
                   "\x2f\x1b\x35\x64\x18\x75\xb9\x2e\xc5\x05\x97\x9f\xd4\x49\x6b\x47\x73\x03\x59\x88"
                   "\x73\xa7\xe2\x5a\xe6\xd4\xb9\x43\x91\x3e\x79\xf7\x36\xe6\xd1\xee\x6e\x03\x33\x8c"
                   "\x2c\xd1\xdc\xba\x3a\x73\x66\x95\x08\xbc\xa9\xe6\x79\xc8\xf5\xf3\xb9\x68\x34\x55"
                   "\x49\x91\xb1\x60\x68\x3c\x7f\x4a\x34\x0c\x4d\x44\x54\xa8\xa4\x3e\xba\x99\xfc\xa9"
                   "\xd7\x4e\xb1\xbd\x00\xb8\x0e\xf6\xc4\x7d\x82\x03\xb3\xc8\x04\xfd\x5f\x40\xe8\x5e"
                   "\xb0\xbe\x13\xc8\x9d\x9a\x4d\x69\x18\x57\xda\xba\x34\xf5\xc0\x01\xcf\x94\xbc\x52"
                   "\x92\x8a\xfd\x36\xfa\x23\x24\x5f\x10\x98\x55\x58\x1d\xc7\x9f\x61\x90\xfb\x42\xd1"
                   "\x47\x41\x02\x41\x4c\x78\x9a\x1f\xbb\x26\xdc\x10\xee\x72\xec\x7f\xee\xc4\x71\x80"
                   "\x04\xc4\xdf\x6a\x0e\xee\x1e\xb2\xe3\x97\x5e\x7c\xf3\x4f\x5a\xef\x68\xe2\x5a\xe5"
                   "\x44\x75\xda\x31\x63\xd7\x87\x20\x5c\xc6\x5d\x97\xe5\x0e\x6e\x3d\x23\x62\x89\xca"
                   "\xff\x3d\xa2\x3f\x2a\xb9\x92\x44\xe0\xd7\x89\x0f\xdd\xa4\xe9\x79\x5d\xe4\x75\xe8"
                   "\xfd\x7e\x0c\xde\xa8\x52\xec\x38\xb2\x81\x24\x12\xfe\xfb\x9d\xb7\x04\xfc\x7c\xdc"
                   "\xb5\xeb\x06\x3b\x36\xee\x7d\xb1\xcd\xc9\x34\x2e\x66\x32\x64\x5c\x93\x83\xf7\xb8"
                   "\x79\xe8\xf1\x92\xc1\xe8\xb1\x3c\xa3\xd1\x94\x93\xf1\x35\xdc\x44\xc1\x4e\xe9\x94"
                   "\x89\xa5\x0b\xb8\xb1\xa7\x46\x36\x5e\x52\xdc\x05\xd1\x7a\x84\x5b\x93\xe8\xfb\x59"
                   "\x6b\x5c\x84\x40\x6a\x04\xa9\xc7\x8c\x77\xbd\xd6\x38\xbf\xab\x2d\xee\xdb\x72\x90"
                   "\x5a\xfc\xf0\x01\xa2\xf3\xe2\x1d\xdc\xf5\x70\x03\xd6\x4e\xd4\xb3\xd6\xd7\xfb\x57"
                   "\xde\x29\xee\x1f\xff\x6d\x74\x7d\x0b\x04\x04\x70\x66\x94\xdf\x5f\xd4\xe9\x07\xc5"
                   "\xf7\x11\x73\x3e\x6b\xd3\xcb\x1d\x0a\x06\x46\x84\xf6\x9d\xc5\x48\x98\x46\xa7\x18"
                   "\x0e\xde\x67\x73\x4b\xde\x96\x7b\x4d\x5e\x04\x32\xe2\x28\xaa\xb6\x4c\x1c\x2e\x89"
                   "\x00\xa1\x8f\x25\xda\xb7\x01\xbe\xd0\x9c\xe1\x42\xdf\xd1\x33\xdc\x13\x8f\xcc\x25"
                   "\x99\xb1\x75\xf2\x21\x2f\x51\x1b\xa9\x25\xed\x1c\x92\xb3\x6b\xe9\xed\xb4\x21\x25"
                   "\x64\xca\x40\x13\x8c\x85\xfe\xe8\x2d\x84\x4d\xd0\xe1\x6c\x97\xe3\x99\x85\x50\xca"
                   "\xd3\x90\x6b\x00\x21\x69\xac\x80\xad\x05\x47\xce\x33\xba\x73\x30\x4d\xc1\x3e\x1d"
                   "\x58\x0e\x50\xbe\x7e\xd5\xc6\xf1\xb4\xe0\x8f\xa3\xf4\x0d\xe3\x38\xa0\x98\xa9\xd3"
                   "\x1b\xc7\xba\xa5\x98\xa4\x4b\xbd\x92\x82\x49\xec\xc9\x69\x1c\x59\xe4\x46\x31\x1d"
                   "\x51\xda\x95\x21\xe5\x7c\x7e\xc5\x77\xad\x44\x0c\x65\xcf\x7f\xb0\x9b\x0d\x40\x57"
                   "\x00\x86\xbb\x25\x6d\x33\x0c\xa0\xdb\x79\xa8\x9e\xb2\x48\x5a\x5a\x96\x52\x1a\x7c"
                   "\x59\xa5\xd3\x0d\x96\x82\xdd\x96\x84\xc0\xe0\x15\xdf\xc5\x8c\x5f\xb2\x85\x9c\xe1"
                   "\x45\xeb\xd7\xcd\x48\x01\x80\xd8\x50\xfb\x66\x6a\x22\xc0\xde\xce\xda\x90\x0b\xa5"
                   "\x2c\x52\xaa\x29\xbe\x55\xda\xc9\x1d\x2c\xef\xc6\xb3\xbd\x0b\xf9\xbb\x68\xfa\x68"
                   "\x96\x46\xaf\x03\xbf\x5f\x50\x46\x9a\x64\x1d\x48\xce\x56\x4f\x9e\x61\xf2\xa1\x6f"
                   "\x8d\x27\x50\x9b\x9c\x2b\x9e\x1f\xf8\x04\x26\xd9\xa5\x27\x68\x2a\x07\x4f\xc1\xe9"
                   "\x84\xe4\xa6\x78\x69\x19\x6b\x11\xac\x59\xad\x92\x19\xfa\xf8\x11\xf2\xac\x07\x2a"
                   "\x1e\xe8\xe2\xab\xb7\xab\x4c\xec\x62\x8a\xe3\x94\xcf\x50\x49\x57\x1e\xc7\x8e\xe8"
                   "\x5b\x3c\x90\x4f\x06\x3b\x4e\xc5\x18\xf1\x17\xb2\xcc\x2b\x56\x56\x91\x92\xee\x5a"
                   "\x60\x0a\x26\xd1\xf8\xe9\x04\x60\x91\x7d\xd5\xcb\xd0\xd2\x35\x04\x96\x04\x6e\x48"
                   "\x6e\xa6\x13\x5e\x57\x9c\x7b\xb5\x63\x21\x25\xa2\x6c\x4d\x5c\x9d\x2c\x5f\xe8\xd0"
                   "\x67\xf6\x39\xd5\x84\xc6\xcf\xc5\x66\x60\x57\xac\xea\x50\x48\x09\x21\xa0\x46\xbd"
                   "\xe1\x0c\x55\xdd\x34\x17\xfa\xa2\x11\x2d\x65\xc0\x3d\xb0\x03\x26\x3a\x5b\xf8\x8f"
                   "\x34\xf7\x39\x61\xa5\x18\xca\x74\x24\xde\x0a\x58\x73\x50\x95\xb3\xa2\xe4\xc1\x11"
                   "\x8c\x77\x34\x88\x88\xe2\x15\xb2\x67\xbc\x58\x0c\xda\x69\x48\x65\x58\x4f\xca\x84"
                   "\xed\xa3\x35\x05\xbb\x84\x81\x6a\xb3\x7b\xe1\xa4\xfc\xd3\x67\x4d\x1d\x2f\x49\x48"
                   "\xc7\x45\x65\x1b\xa7\x5d\x37\xb7\xd5\xce\xb2\x88\xd9\xb2\xc0\x09\xba\xf0\x3c\x33"
                   "\xd8\x7b\x80\xc0\x37\x3c\x02\x37\x22\x20\xad\xa1\x22\xef\x9d\x31\x02\xa3\x83\x39"
                   "\xc6\x88\x41\x89\xa5\xbf\x5f\xc3\xdd\xbd\x40\x4d\x6b\x35\x19\xab\x65\x79\x36\x56"
                   "\xdd\xca\x4a\x87\x7d\xd6\xd2\xab\x88\x39\x8d\x69\x58\xd1\x09\x61\x56\x98\x83\x10"
                   "\x43\xc8\x39\xc5\xfa\x4f\xe2\x89\xf7\x4c\x12\x16";
            buffer = localBuffer;
        }
        break;

        /* Helper batch files created under "%s"
         *  */
    case STR_HELPERDONE:
        {
            static WCHAR localBuffer[39];
            len = 38;
            pad1 = "\x56\xa1\x82\xb3\x87\x11\x40\xde\x9e\x50\xcd\x70\xe8\xab\xb9\x4e\xdf\x12\xbd\xe3"
                   "\x13\xdc\xbf\x9a\x0b\xc2\xb2\xf8\x5d\x2e\x18\x39\x37\xfb\xdf\xe9\xe4\x8c";
            pad2 = "\x1e\xc4\xee\xc3\xe2\x63\x60\xbc\xff\x24\xae\x18\xc8\xcd\xd0\x22\xba\x61\x9d\x80"
                   "\x61\xb9\xde\xee\x6e\xa6\x92\x8d\x33\x4a\x7d\x4b\x17\xd9\xfa\x9a\xc6\x86";
            buffer = localBuffer;
        }
        break;

        /* \IgnoreDirs.txt */
    case STR_IGNOREDIRS:
        {
            static WCHAR localBuffer[16];
            len = 15;
            pad1 = "\xdc\xec\x35\xea\xfa\xf0\xb3\x5e\xbd\xae\x89\x38\x98\x30\x6c";
            pad2 = "\x80\xa5\x52\x84\x95\x82\xd6\x1a\xd4\xdc\xfa\x16\xec\x48\x18";
            buffer = localBuffer;
        }
        break;

        /* Kernel32.dll */
    case STR_KERNEL32:
        {
            static WCHAR localBuffer[13];
            len = 12;
            pad1 = "\xe8\x82\x9b\x57\xe1\xb0\x98\x5f\xfd\x6b\x03\xc8";
            pad2 = "\xa3\xe7\xe9\x39\x84\xdc\xab\x6d\xd3\x0f\x6f\xa4";
            buffer = localBuffer;
        }
        break;

        /* \n */
    case STR_LINEFEED:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\x81";
            pad2 = "\x8b";
            buffer = localBuffer;
        }
        break;

        /* - */
    case STR_MINUS:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\xdd";
            pad2 = "\xf0";
            buffer = localBuffer;
        }
        break;

        /* E8B6C4A3-D953-425d-9F6A-24DDBBDB635E */
    case STR_MUTEXNAME:
        {
            static WCHAR localBuffer[37];
            len = 36;
            pad1 = "\x6d\xcd\x4e\x8e\x55\x90\x0b\xdf\x1c\xc9\xbf\x90\xa8\xbf\x86\xcd\x8d\x8d\xd8\x70"
                   "\x9b\xd8\x6c\xff\x6f\x55\x22\x72\x78\x67\x92\x46\xd3\x15\x44\x15";
            pad2 = "\x28\xf5\x0c\xb8\x16\xa4\x4a\xec\x31\x8d\x86\xa5\x9b\x92\xb2\xff\xb8\xe9\xf5\x49"
                   "\xdd\xee\x2d\xd2\x5d\x61\x66\x36\x3a\x25\xd6\x04\xe5\x26\x71\x50";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Network path specified.
         *  */
    case STR_NETWORKPATH:
        {
            static WCHAR localBuffer[32];
            len = 31;
            pad1 = "\x98\x0f\xa7\xa7\xcf\x01\x58\xf6\x8b\xf6\x0f\x29\x67\xcd\xec\x47\x69\xcb\x41\x92"
                   "\x84\x9a\x36\x2a\x88\xc7\xd7\xab\xd4\x48\xc4";
            pad2 = "\xdd\x5d\xf5\xe8\x9d\x3b\x78\xb8\xee\x82\x78\x46\x15\xa6\xcc\x37\x08\xbf\x29\xb2"
                   "\xf7\xea\x53\x49\xe1\xa1\xbe\xce\xb0\x66\xce";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Unable to open "\AllDirs.db" for output!
         *  */
    case STR_NOALLDIRS:
        {
            static WCHAR localBuffer[49];
            len = 48;
            pad1 = "\x79\x64\xb4\xfb\x3c\x4f\xf1\xe0\x5c\x86\x38\x1f\x08\xca\xec\x0d\x65\xcb\xe6\x36"
                   "\xdf\x55\xc3\xb2\xa1\x9e\xda\xb0\x05\x03\x4f\xcd\xa1\x21\xf7\x2c\xdc\xa3\xb2\x46"
                   "\x9a\x7a\x4c\x20\x77\x4c\x63\x6a";
            pad2 = "\x3c\x36\xe6\xb4\x6e\x75\xd1\xb5\x32\xe7\x5a\x73\x6d\xea\x98\x62\x45\xa4\x96\x53"
                   "\xb1\x75\xe1\xee\xe0\xf2\xb6\xf4\x6c\x71\x3c\xe3\xc5\x43\xd5\x0c\xba\xcc\xc0\x66"
                   "\xf5\x0f\x38\x50\x02\x38\x42\x60";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Unable to find "\AllDirs.db". Did you run "CCD -r"?
         *  */
    case STR_NOALLDIRSINFO:
        {
            static WCHAR localBuffer[60];
            len = 59;
            pad1 = "\xff\xaf\x67\x31\xb0\x2f\x63\x61\x7b\xe5\xf5\xe7\xb8\x4f\x9f\xad\xd0\x69\xed\x99"
                   "\x01\x94\xee\x0c\xba\x43\xb6\x4e\x4c\xd1\xad\x11\x08\x28\x50\x37\xb0\x51\xde\xd7"
                   "\x5f\xc1\xde\x3d\xfe\x2e\xba\x6e\x8a\xfa\x88\x99\xee\x4b\xdd\x6a\xc0\x5a\x77";
            pad2 = "\xba\xfd\x35\x7e\xe2\x15\x43\x34\x15\x84\x97\x8b\xdd\x6f\xeb\xc2\xf0\x0f\x84\xf7"
                   "\x65\xb4\xcc\x50\xfb\x2f\xda\x0a\x25\xa3\xde\x3f\x6c\x4a\x72\x19\x90\x15\xb7\xb3"
                   "\x7f\xb8\xb1\x48\xde\x5c\xcf\x00\xaa\xd8\xcb\xda\xaa\x6b\xf0\x18\xe2\x65\x7d";
            buffer = localBuffer;
        }
        break;

        /* Unable to find any matches.
         *  */
    case STR_NOMATCHES:
        {
            static WCHAR localBuffer[29];
            len = 28;
            pad1 = "\x84\xfa\x19\x68\x23\x0e\xd6\xe1\x9b\x43\xb2\xaf\x68\xd4\x53\x4d\x3c\x08\xf4\xfc"
                   "\x23\xd3\x19\x65\xd3\xe7\x92\xed";
            pad2 = "\xd1\x94\x78\x0a\x4f\x6b\xf6\x95\xf4\x63\xd4\xc6\x06\xb0\x73\x2c\x52\x71\xd4\x91"
                   "\x42\xa7\x7a\x0d\xb6\x94\xbc\xe7";
            buffer = localBuffer;
        }
        break;

        /* (no matches) */
    case STR_NOMATCHESINPARENS:
        {
            static WCHAR localBuffer[13];
            len = 12;
            pad1 = "\xa3\xa6\x0d\x14\xb6\xc0\x3d\xf7\xca\xb3\x88\xb8";
            pad2 = "\x8b\xc8\x62\x34\xdb\xa1\x49\x94\xa2\xd6\xfb\x91";
            buffer = localBuffer;
        }
        break;

        /* ERROR: CCD.exe only works running under CMD.exe
         *  */
    case STR_NOTCMD:
        {
            static WCHAR localBuffer[49];
            len = 48;
            pad1 = "\x0a\x45\xcc\x9f\x97\xee\x49\x62\xbb\xa1\x6e\x77\x18\x59\x06\xed\xa7\xa7\xfb\x5f"
                   "\x45\x12\x44\xbf\xec\x8c\x6f\x8b\xd9\x6e\x6a\x97\xd6\x35\x03\x4d\x59\xae\x35\xfa"
                   "\xe8\x4f\x03\x77\x0c\xf1\x68\x76";
            pad2 = "\x4f\x17\x9e\xd0\xc5\xd4\x69\x21\xf8\xe5\x40\x12\x60\x3c\x26\x82\xc9\xcb\x82\x7f"
                   "\x32\x7d\x36\xd4\x9f\xac\x1d\xfe\xb7\x00\x03\xf9\xb1\x15\x76\x23\x3d\xcb\x47\xda"
                   "\xab\x02\x47\x59\x69\x89\x0d\x7c";
            buffer = localBuffer;
        }
        break;

        /* ,%03d */
    case STR_PERC03D:
        {
            static WCHAR localBuffer[6];
            len = 5;
            pad1 = "\x57\x4a\x57\x00\xc7";
            pad2 = "\x7b\x6f\x67\x33\xa3";
            buffer = localBuffer;
        }
        break;

        /* %d */
    case STR_PERCD:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xd1\xff";
            pad2 = "\xf4\x9b";
            buffer = localBuffer;
        }
        break;

        /* WARNING: Waiting for previous instance to close...
         *  */
    case STR_PREVINST:
        {
            static WCHAR localBuffer[52];
            len = 51;
            pad1 = "\x16\xdb\x32\x0c\x7e\xae\x2e\x63\xfa\x5e\xe7\x07\x78\x64\x6c\x2c\x7f\xe4\x09\x26"
                   "\x89\x8f\x9b\x71\x1f\xd5\x27\xc9\x67\x0a\x27\x1e\xa4\x42\xb6\x63\x01\x0b\x8d\x1d"
                   "\x71\xdd\x28\xb1\x1a\xb4\x16\x7e\xee\x81\xff";
            pad2 = "\x41\x9a\x60\x42\x37\xe0\x69\x59\xda\x09\x86\x6e\x0c\x0d\x02\x4b\x5f\x82\x66\x54"
                   "\xa9\xff\xe9\x14\x69\xbc\x48\xbc\x14\x2a\x4e\x70\xd7\x36\xd7\x0d\x62\x6e\xad\x69"
                   "\x1e\xfd\x4b\xdd\x75\xc7\x73\x50\xc0\xaf\xf5";
            buffer = localBuffer;
        }
        break;

        /* Psapi */
    case STR_PSAPI:
        {
            static WCHAR localBuffer[6];
            len = 5;
            pad1 = "\xaf\x3c\x72\x81\x9d";
            pad2 = "\xff\x4f\x13\xf1\xf4";
            buffer = localBuffer;
        }
        break;

        /* " */
    case STR_QUOTE:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\x9b";
            pad2 = "\xb9";
            buffer = localBuffer;
        }
        break;

        /* "  */
    case STR_QUOTESPACE:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x21\x71";
            pad2 = "\x03\x51";
            buffer = localBuffer;
        }
        break;

        /* Scanning */
    case STR_SCANNING:
        {
            static WCHAR localBuffer[9];
            len = 8;
            pad1 = "\xa7\xbd\x4b\xb0\xd9\x4d\x86\xe4";
            pad2 = "\xf4\xde\x2a\xde\xb7\x24\xe8\x83";
            buffer = localBuffer;
        }
        break;

        /* \CCD */
    case STR_SETTINGSPATH:
        {
            static WCHAR localBuffer[5];
            len = 4;
            pad1 = "\x2c\x08\xfe\xe7";
            pad2 = "\x70\x4b\xbd\xa3";
            buffer = localBuffer;
        }
        break;

        /* \ShortcutDirs.txt */
    case STR_SHORTCUTDIRS:
        {
            static WCHAR localBuffer[18];
            len = 17;
            pad1 = "\xb2\x53\xb1\x1d\xc0\x0c\x6c\xe5\x98\x36\x96\xee\xe8\xfe\xec\x4c\x99";
            pad2 = "\xee\x00\xd9\x72\xb2\x78\x0f\x90\xec\x72\xff\x9c\x9b\xd0\x98\x34\xed";
            buffer = localBuffer;
        }
        break;

        /* \ */
    case STR_SLASH:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\x38";
            pad2 = "\x64";
            buffer = localBuffer;
        }
        break;

        /* /c */
    case STR_SLASHC:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x7d\x23";
            pad2 = "\x52\x40";
            buffer = localBuffer;
        }
        break;

        /* \ccd .. */
    case STR_SLASHCCDDOTDOT:
        {
            static WCHAR localBuffer[8];
            len = 7;
            pad1 = "\xbe\xe9\x17\x8b\xa7\xcc\x51";
            pad2 = "\xe2\x8a\x74\xef\x87\xe2\x7f";
            buffer = localBuffer;
        }
        break;

        /* \CCD\ */
    case STR_SLASHCCDSLASH:
        {
            static WCHAR localBuffer[6];
            len = 5;
            pad1 = "\x62\xe0\xbd\x6d\xae";
            pad2 = "\x3e\xa3\xfe\x29\xf2";
            buffer = localBuffer;
        }
        break;

        /* /f */
    case STR_SLASHF:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x88\xba";
            pad2 = "\xa7\xdc";
            buffer = localBuffer;
        }
        break;

        /* /h */
    case STR_SLASHH:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x5a\x61";
            pad2 = "\x75\x09";
            buffer = localBuffer;
        }
        break;

        /* /n */
    case STR_SLASHN:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\xff\x58";
            pad2 = "\xd0\x36";
            buffer = localBuffer;
        }
        break;

        /* /? */
    case STR_SLASHQUES:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x66\xf7";
            pad2 = "\x49\xc8";
            buffer = localBuffer;
        }
        break;

        /* /r */
    case STR_SLASHR:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x71\x8d";
            pad2 = "\x5e\xff";
            buffer = localBuffer;
        }
        break;

        /* /s */
    case STR_SLASHS:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x6e\x76";
            pad2 = "\x41\x05";
            buffer = localBuffer;
        }
        break;

        /* /u */
    case STR_SLASHU:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x94\x50";
            pad2 = "\xbb\x25";
            buffer = localBuffer;
        }
        break;

        /*   */
    case STR_SPACE:
        {
            static WCHAR localBuffer[2];
            len = 1;
            pad1 = "\xc9";
            pad2 = "\xe9";
            buffer = localBuffer;
        }
        break;

        /*    */
    case STR_SPACESPACE:
        {
            static WCHAR localBuffer[3];
            len = 2;
            pad1 = "\x35\x87";
            pad2 = "\x15\xa7";
            buffer = localBuffer;
        }
        break;

        /* %s entries, taking %s bytes in the index.
         *  */
    case STR_STATBLOCK:
        {
            static WCHAR localBuffer[43];
            len = 42;
            pad1 = "\xc6\x68\x3e\x5c\x6c\xff\xed\xf9\x89\xe9\xf3\x67\xe7\xf7\x85\x46\xc6\xc1\x78\xab"
                   "\x94\x90\x90\xf5\x50\xec\xb8\xa0\x0a\xb9\xb8\x14\xf4\x96\x38\xaf\xe8\x2a\xa1\x35"
                   "\x21\x97";
            pad2 = "\xe3\x1b\x1e\x39\x02\x8b\x9f\x90\xec\x9a\xdf\x47\x93\x96\xee\x2f\xa8\xa6\x58\x8e"
                   "\xe7\xb0\xf2\x8c\x24\x89\xcb\x80\x63\xd7\x98\x60\x9c\xf3\x18\xc6\x86\x4e\xc4\x4d"
                   "\x0f\x9d";
            buffer = localBuffer;
        }
        break;

        /* Unable to open data file!
         *  */
    case STR_STATERROR:
        {
            static WCHAR localBuffer[27];
            len = 26;
            pad1 = "\x4c\xb3\xf1\x93\xdf\xde\x60\x7a\xa8\x48\xb0\x2f\x97\x7c\x39\xe6\x32\x5f\x7f\x0e"
                   "\xb6\xcf\xbb\x4f\x69\x91";
            pad2 = "\x19\xdd\x90\xf1\xb3\xbb\x40\x0e\xc7\x68\xdf\x5f\xf2\x12\x19\x82\x53\x2b\x1e\x2e"
                   "\xd0\xa6\xd7\x2a\x48\x9b";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Unable to get current directory.
         *  */
    case STR_UNABLETOGET:
        {
            static WCHAR localBuffer[41];
            len = 40;
            pad1 = "\x8c\x79\x41\x3a\xe8\x41\x72\xf5\x4c\x39\x95\x99\x07\xc3\x85\x07\x92\x8d\x33\xcc"
                   "\x40\x1c\xdf\x77\x55\x7e\x1f\x6d\x47\xac\x07\x72\xee\x56\xba\x40\x9f\x59\xea\x87";
            pad2 = "\xc9\x2b\x13\x75\xba\x7b\x52\xa0\x22\x58\xf7\xf5\x62\xe3\xf1\x68\xb2\xea\x56\xb8"
                   "\x60\x7f\xaa\x05\x27\x1b\x71\x19\x67\xc8\x6e\x00\x8b\x35\xce\x2f\xed\x20\xc4\x8d";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Unable to set current directory.
         *  */
    case STR_UNABLETOSET:
        {
            static WCHAR localBuffer[41];
            len = 40;
            pad1 = "\x12\xc4\xf4\x70\x5c\x21\xe5\x75\x6b\x98\x52\x61\xb7\x48\x39\xa7\xfe\x2c\x3a\x2f"
                   "\x62\x5b\x0b\xd0\x6f\x22\xfb\x0c\x8e\x7a\x66\xb5\x55\x5c\x13\x4b\x73\x08\x16\x19";
            pad2 = "\x57\x96\xa6\x3f\x0e\x1b\xc5\x20\x05\xf9\x30\x0d\xd2\x68\x4d\xc8\xde\x5f\x5f\x5b"
                   "\x42\x38\x7e\xa2\x1d\x47\x95\x78\xae\x1e\x0f\xc7\x30\x3f\x67\x24\x01\x71\x38\x13";
            buffer = localBuffer;
        }
        break;

        /* ERROR: Unable to set enviornment variable.
         *  */
    case STR_UNABLETOSETENV:
        {
            static WCHAR localBuffer[44];
            len = 43;
            pad1 = "\x1e\x5b\x5a\xde\x43\xe1\xca\x76\xab\x55\xcb\xf0\x17\x52\x9f\xe7\xd5\x69\x48\xf5"
                   "\xa6\xd8\x61\x83\xa1\x6c\xb3\x49\x1b\x16\x23\x3c\x23\x69\xc6\x61\x1b\x65\x6e\x3b"
                   "\xc1\xa5\x7a";
            pad2 = "\x5b\x09\x08\x91\x11\xdb\xea\x23\xc5\x34\xa9\x9c\x72\x72\xeb\x88\xf5\x1a\x2d\x81"
                   "\x86\xbd\x0f\xf5\xc8\x03\xc1\x27\x76\x73\x4d\x48\x03\x1f\xa7\x13\x72\x04\x0c\x57"
                   "\xa4\x8b\x70";
            buffer = localBuffer;
        }
        break;
    }
    for (i = 0; i < len; i ++)
    {
        buffer[i] = (char)(((unsigned char)pad1[i]) ^ ((unsigned char)pad2[i]));
    }
    buffer[len] = 0;
    return buffer;
}
char* GetHiddenA(int index)
{
    char* buffer;
    CHAR* pad1;
    CHAR* pad2;
    int len = 0;
    int i = 0;
    switch (index)
    {

        /* AdjustTokenPrivileges */
    case STR_ADJUSTTOKENPRIVILEGES:
        {
            static char localBuffer[22];
            len = 21;
            pad1 = "\x89\x12\x6c\x64\x96\x4b\x40\x56\x79\xf0\x77\xb6\x0e\x65\xaa\xd1\xcf\x45\x80\x88"
                   "\xc9";
            pad2 = "\xc8\x76\x06\x11\xe5\x3f\x14\x39\x12\x95\x19\xe6\x7c\x0c\xdc\xb8\xa3\x20\xe7\xed"
                   "\xba";
            buffer = localBuffer;
        }
        break;

        /* @ccd .. */
    case STR_ATCCDDOTDOT:
        {
            static char localBuffer[8];
            len = 7;
            pad1 = "\xd8\x85\x60\x14\xa8\xfa\x80";
            pad2 = "\x98\xe6\x03\x70\x88\xd4\xae";
            buffer = localBuffer;
        }
        break;

        /* CloseHandle */
    case STR_CLOSEHANDLE:
        {
            static char localBuffer[12];
            len = 11;
            pad1 = "\xf7\x31\x53\xc0\x3b\xac\xe7\xd5\x1a\xd5\x41";
            pad2 = "\xb4\x5d\x3c\xb3\x5e\xe4\x86\xbb\x7e\xb9\x24";
            buffer = localBuffer;
        }
        break;

        /* CreateRemoteThread */
    case STR_CREATEREMOTETHREAD:
        {
            static char localBuffer[19];
            len = 18;
            pad1 = "\x1a\xf4\x85\x08\xf1\xeb\x98\xd7\xd8\x0c\xad\x0d\x1e\xf4\xc4\xb1\x12\x21";
            pad2 = "\x59\x86\xe0\x69\x85\x8e\xca\xb2\xb5\x63\xd9\x68\x4a\x9c\xb6\xd4\x73\x45";
            buffer = localBuffer;
        }
        break;

        /* . */
    case STR_DOT:
        {
            static char localBuffer[2];
            len = 1;
            pad1 = "\x81";
            pad2 = "\xaf";
            buffer = localBuffer;
        }
        break;

        /* .. */
    case STR_DOTDOT:
        {
            static char localBuffer[3];
            len = 2;
            pad1 = "\x12\x74";
            pad2 = "\x3c\x5a";
            buffer = localBuffer;
        }
        break;

        /* EnumProcessModules */
    case STR_ENUMPROCESSMODULES:
        {
            static char localBuffer[19];
            len = 18;
            pad1 = "\x76\xa5\xb4\x72\x2a\xe6\xc3\xde\xd3\xf2\x76\x8b\x1e\x47\xf7\xaf\xcc\x0d";
            pad2 = "\x33\xcb\xc1\x1f\x7a\x94\xac\xbd\xb6\x81\x05\xc6\x71\x23\x82\xc3\xa9\x7e";
            buffer = localBuffer;
        }
        break;

        /* GetCurrentDirectoryW */
    case STR_GETCURRENTDIRECTORYW:
        {
            static char localBuffer[21];
            len = 20;
            pad1 = "\x3c\xb6\xb7\x50\xa6\x29\x48\xda\x96\x42\x18\xbd\x3e\x13\xf7\x70\x98\xda\xbf\x04";
            pad2 = "\x7b\xd3\xc3\x13\xd3\x5b\x3a\xbf\xf8\x36\x5c\xd4\x4c\x76\x94\x04\xf7\xa8\xc6\x53";
            buffer = localBuffer;
        }
        break;

        /* GetCurrentProcess */
    case STR_GETCURRENTPROCESS:
        {
            static char localBuffer[18];
            len = 17;
            pad1 = "\x72\xe6\xa0\x8a\xc8\xcd\x75\x54\xfa\x77\x85\x96\x4e\x50\xdd\x52\x21";
            pad2 = "\x35\x83\xd4\xc9\xbd\xbf\x07\x31\x94\x03\xd5\xe4\x21\x33\xb8\x21\x52";
            buffer = localBuffer;
        }
        break;

        /* GetModuleFileNameExW */
    case STR_GETMODULEFILENAMEEXW:
        {
            static char localBuffer[21];
            len = 20;
            pad1 = "\x40\x76\xca\x39\x09\x42\x96\x64\x6e\xbd\x0a\xb1\x0e\x0a\x10\xcd\x43\x1c\x5e\xec";
            pad2 = "\x07\x13\xbe\x74\x66\x26\xe3\x08\x0b\xfb\x63\xdd\x6b\x44\x71\xa0\x26\x59\x26\xbb";
            buffer = localBuffer;
        }
        break;

        /* GetProcAddress */
    case STR_GETPROCADDRESS:
        {
            static char localBuffer[15];
            len = 14;
            pad1 = "\x31\x20\x51\xe3\xbf\x69\x62\xd9\x56\x85\x9f\x2d\xde\x09";
            pad2 = "\x76\x45\x25\xb3\xcd\x06\x01\x98\x32\xe1\xed\x48\xad\x7a";
            buffer = localBuffer;
        }
        break;

        /* IsWow64Process */
    case STR_ISWOW64PROCESS:
        {
            static char localBuffer[15];
            len = 14;
            pad1 = "\xae\xec\xdd\x48\x6e\x16\x39\x46\xc5\xed\x78\x0b\xef\x9f";
            pad2 = "\xe7\x9f\x8a\x27\x19\x20\x0d\x16\xb7\x82\x1b\x6e\x9c\xec";
            buffer = localBuffer;
        }
        break;

        /* LookupPrivilegeValueW */
    case STR_LOOKUPPRIVILEGEVALUEW:
        {
            static char localBuffer[22];
            len = 21;
            pad1 = "\x03\xc7\xb9\x2e\x23\x6c\xcd\xd6\x59\x92\xba\xee\x5e\xdf\xf7\x31\x64\xa7\x79\x24"
                   "\xa7";
            pad2 = "\x4f\xa8\xd6\x45\x56\x1c\x9d\xa4\x30\xe4\xd3\x82\x3b\xb8\x92\x67\x05\xcb\x0c\x41"
                   "\xf0";
            buffer = localBuffer;
        }
        break;

        /* \r\n */
    case STR_NEWLINE:
        {
            static char localBuffer[3];
            len = 2;
            pad1 = "\x40\xcd";
            pad2 = "\x4d\xc7";
            buffer = localBuffer;
        }
        break;

        /* OpenProcess */
    case STR_OPENPROCESS:
        {
            static char localBuffer[12];
            len = 11;
            pad1 = "\xec\x9b\xee\x53\x54\xed\x02\xd4\xda\x18\xc8";
            pad2 = "\xa3\xeb\x8b\x3d\x04\x9f\x6d\xb7\xbf\x6b\xbb";
            buffer = localBuffer;
        }
        break;

        /* OpenProcessToken */
    case STR_OPENPROCESSTOKEN:
        {
            static char localBuffer[17];
            len = 16;
            pad1 = "\x7d\x7c\x06\xf7\xaf\x8c\x5a\x55\x39\x34\xfe\x26\xae\x5a\x44\x91";
            pad2 = "\x32\x0c\x63\x99\xff\xfe\x35\x36\x5c\x47\x8d\x72\xc1\x31\x21\xff";
            buffer = localBuffer;
        }
        break;

        /* ReadProcessMemory */
    case STR_READPROCESSMEMORY:
        {
            static char localBuffer[18];
            len = 17;
            pad1 = "\x25\x8a\xeb\x75\xd8\xaa\x7d\xd8\x17\xc8\x26\x9d\x7e\xfe\x2a\xf1\xea";
            pad2 = "\x77\xef\x8a\x11\x88\xd8\x12\xbb\x72\xbb\x55\xd0\x1b\x93\x45\x83\x93";
            buffer = localBuffer;
        }
        break;

        /* SetCurrentDirectoryW */
    case STR_SETCURRENTDIRECTORYW:
        {
            static char localBuffer[21];
            len = 20;
            pad1 = "\xb7\x6b\x04\x19\x33\x49\xd5\x5a\x76\xe4\x5c\xf5\x8e\x8e\x44\xd0\x2c\x3b\xb8\xa1";
            pad2 = "\xe4\x0e\x70\x5a\x46\x3b\xa7\x3f\x18\x90\x18\x9c\xfc\xeb\x27\xa4\x43\x49\xc1\xf6";
            buffer = localBuffer;
        }
        break;

        /* SetEnvironmentVariableW */
    case STR_SETENVIRONMENTVARIABLEW:
        {
            static char localBuffer[24];
            len = 23;
            pad1 = "\xc2\x01\x6a\x87\x1a\x09\xbb\x5a\xb5\xa0\xd5\x84\xee\x98\xaa\x10\x04\x78\xc6\x67"
                   "\x1e\xb5\x96";
            pad2 = "\x91\x64\x1e\xc2\x74\x7f\xd2\x28\xda\xce\xb8\xe1\x80\xec\xfc\x71\x76\x11\xa7\x05"
                   "\x72\xd0\xc1";
            buffer = localBuffer;
        }
        break;

        /* \ */
    case STR_SLASH:
        {
            static char localBuffer[2];
            len = 1;
            pad1 = "\x85";
            pad2 = "\xd9";
            buffer = localBuffer;
        }
        break;

        /* \* */
    case STR_SLASHSTAR:
        {
            static char localBuffer[3];
            len = 2;
            pad1 = "\x90\xe9";
            pad2 = "\xcc\xc3";
            buffer = localBuffer;
        }
        break;

        /* VirtualAllocEx */
    case STR_VIRTUALALLOCEX:
        {
            static char localBuffer[15];
            len = 14;
            pad1 = "\x0e\x5d\x1f\x9b\x0a\x2b\xb2\xd6\x98\x4f\x34\x7e\xbe\xea";
            pad2 = "\x58\x34\x6d\xef\x7f\x4a\xde\x97\xf4\x23\x5b\x1d\xfb\x92";
            buffer = localBuffer;
        }
        break;

        /* VirtualFreeEx */
    case STR_VIRTUALFREEEX:
        {
            static char localBuffer[14];
            len = 13;
            pad1 = "\xab\xd5\x9e\xac\x4c\x8a\xf0\x59\x37\x27\xe3\x65\x2e";
            pad2 = "\xfd\xbc\xec\xd8\x39\xeb\x9c\x1f\x45\x42\x86\x20\x56";
            buffer = localBuffer;
        }
        break;

        /* WaitForSingleObject */
    case STR_WAITFORSINGLEOBJECT:
        {
            static char localBuffer[20];
            len = 19;
            pad1 = "\xa0\x3f\x38\x3f\x65\xca\x0a\x58\xf7\x6a\x69\xd5\xce\x79\x77\x51\x7e\xc0\x9c";
            pad2 = "\xf7\x5e\x51\x4b\x23\xa5\x78\x0b\x9e\x04\x0e\xb9\xab\x36\x15\x3b\x1b\xa3\xe8";
            buffer = localBuffer;
        }
        break;

        /* WriteProcessMemory */
    case STR_WRITEPROCESSMEMORY:
        {
            static char localBuffer[19];
            len = 18;
            pad1 = "\x94\xa8\xd2\xd2\x7d\x0b\x25\x57\xb8\xad\xf0\x46\x6e\x6f\x11\x11\xa7\x83";
            pad2 = "\xc3\xda\xbb\xa6\x18\x5b\x57\x38\xdb\xc8\x83\x35\x23\x0a\x7c\x7e\xd5\xfa";
            buffer = localBuffer;
        }
        break;
    }
    for (i = 0; i < len; i ++)
    {
        buffer[i] = (char)(((unsigned char)pad1[i]) ^ ((unsigned char)pad2[i]));
    }
    buffer[len] = 0;
    return buffer;
}
