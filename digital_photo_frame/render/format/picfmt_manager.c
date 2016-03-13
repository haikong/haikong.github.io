#include <config.h>
#include <pic_operation.h>
#include <picfmt_manager.h>
#include <string.h>


static PT_PicFileParser g_ptPicFileParserHead;

int RegisterPicFileParser(PT_PicFileParser ptPicFileParser)
{
	PT_PicFileParser ptTmp;

	if (!g_ptPicFileParserHead)
	{
		g_ptPicFileParserHead   = ptPicFileParser;
		ptPicFileParser->pnext = NULL;
	}
	else
	{
		ptTmp = g_ptPicFileParserHead;
		while (ptTmp->pnext)
		{
			ptTmp = ptTmp->pnext;
		}
		ptTmp->pnext	  = ptPicFileParser;
		ptPicFileParser->pnext = NULL;
	}

	return 0;
}


void ShowPicFmts(void)
{
	int i = 0;
	PT_PicFileParser ptTmp = g_ptPicFileParserHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->pnext;
	}
}

PT_PicFileParser Parser(char *pcName)
{
	PT_PicFileParser ptTmp = g_ptPicFileParserHead;

	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->pnext;
	}
	return NULL;
}

PT_PicFileParser GetParser(PT_FileMap ptFileMap)
{
	PT_PicFileParser ptTmp = g_ptPicFileParserHead;

	while (ptTmp)
	{
		if (ptTmp->isSupport(ptFileMap))
		{
			return ptTmp;
		}
		ptTmp = ptTmp->pnext;
	}
	return NULL;
}

int PicFmtsInit(void)
{
	int iError;

	iError = BMPParserInit();
	iError |= JPGParserInit();

	return iError;
}

