#include "main.h"
#include "xtrack.h"
#include "Array.h"
#include "Ngram.h"
#include "NgramBuilder.h"
#include "NgramReader.h"
#include "crc32.h"

int _tmain(int argc, _TCHAR* argv[])
{
	XTRACK_START();

	bool write = true;
	bool read = true;

	if (write && NgramBuilder::OpenData(_T("data")))
	{
		FILE* fp = NULL;
		fopen_s(&fp, "Sample Data.txt", "r");
		XTRACK(fp);
		char line[1000];

		NgramBuilder * buildMain = new NgramBuilder();
		XTRACK(buildMain);
		NgramBuilder * buildMini = new NgramBuilder();
		XTRACK(buildMini);

		DWORD dwTick = GetTickCount();

		printf("Loading...\n");
		while (fgets(line, 1000, fp))
		{
			line[strlen(line) - 1] = 0;
			buildMain->AddWord(line);

			char * dir = line;
			for (char * cur = line; *cur; cur++)
			{
				if (*cur == '\\')
				{
					dir = cur;
					dir++;
				}
			}

			buildMini->AddWord(dir);
		}

		XUNTRACK(fp);
		fclose(fp);

		printf("Writing file...\n");

		NgramBuilder::WriteValue(NGRAM_COOKIE);
		int offset = NgramBuilder::GetOffset();
		NgramBuilder::WriteValue(0);
		NgramBuilder::WriteValue(0);

		int ptrMini = NgramBuilder::GetOffset();
		buildMini->Write();
		int ptrMain = NgramBuilder::GetOffset();
		buildMain->Write();

		NgramBuilder::Seek(offset);
		NgramBuilder::WriteValue(ptrMini);
		NgramBuilder::WriteValue(ptrMain);

		XUNTRACK(buildMini);
		delete buildMini;
		XUNTRACK(buildMain);
		delete buildMain;

		NgramBuilder::CloseData();

		dwTick = GetTickCount() - dwTick;
		printf("That took %d ms\n\n", dwTick);
	}

	if (read && NgramReader::OpenData(_T("data")))
	{
		DWORD dwTick = GetTickCount();
		printf("Opening...\n");
		int cookie = NgramReader::ReadValue(0);
		if (cookie == NGRAM_COOKIE)
		{
			NgramReader * readMini = new NgramReader(NgramReader::ReadValue(1));
			XTRACK(readMini);
			NgramReader * readMain = new NgramReader(NgramReader::ReadValue(2));
			XTRACK(readMain);

			Array<int> indexesMini;
			Array<char*> wordsMini;
			Array<char*> wordsMain;

			printf("Searching mini...\n");
			readMini->FindPhrase("gonize", &wordsMini, &indexesMini);
			printf("Searching main...\n");
			readMain->FindPhrase("gonize", &wordsMain, NULL);

			dwTick = GetTickCount() - dwTick;

			printf("Mini:\n");

			for (int i = 0; i < wordsMini.Count; i ++)
			{
				printf("  [%s] == [%s]\n", wordsMini.List[i], readMain->GetFullWord(indexesMini.List[i]));
			}

			printf("Main:\n");

			for (int i = 0; i < wordsMain.Count; i ++)
			{
				printf("  [%s]\n", wordsMain.List[i]);
			}

			XUNTRACK(readMini);
			delete readMini;
			XUNTRACK(readMain);
			delete readMain;

			printf("That took %d ms\n", dwTick);
		}

		NgramReader::CloseData();
	}

	XTRACK_STOP();

	printf("Press enter . . . ");
	getchar();

	return 0;
}

