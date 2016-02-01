/*****************************************************************************
 split_compounds - splits Estonian compounds into morphemes, 
                   uses Estonian morphological analyzer Vabamorf (by Filosoft)
 Copyright (C) 2016 Tilde

 *****************************************************************************/
#include "lib/etana/etmrf.h"
#include <algorithm>
#include <string>
#include <wchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ctype.h>
#include <cstdarg>
#include <cstdio>
#include <stdint.h>

using namespace std;

wstring SplitInWords(wstring & instring, wstring & basestring, map<wchar_t, wchar_t> & UpperLowermap, bool includeArrows)
{
	wstring result=L"";

	int idx=basestring.find_last_of(L'_');

	int j=0;
	int i=0;
	if ((idx ==string::npos) || (idx==basestring.length()-1))
	{
		result=instring;
	}
	else
	{
		bool bStart=true;
		wstring tmp;

		for(i=0, j=0; i<=idx+1 && j<instring.length(); i++)
		{
			if(wcschr(L"<]?~+=",basestring[i])) continue;

			if(basestring[i]==L'_')
			{
				if (!tmp.empty())
				{ //kahekümne k<aks_kümmend
					result += tmp;
					tmp.clear();
				}
				if ((!wcschr(L"<]?~+=",basestring[i+1])) && (basestring[i+1] != instring[j]))
				{ 
					//seitsmeteistkümne seitse_t<eist
					//paarisaja p<aar_sada
					result += instring[j];
					
					j++;
				}
				if (includeArrows) result += L'γ';
				result += L' ';
				if (includeArrows) result += L'γ';
				bStart=true;
			}
			else if(bStart && ((basestring[i]==instring[j]) || (basestring[i]==UpperLowermap[instring[j]])))
			{ //start of stem the same
				result += instring[j];
				
				j++;
			}
			else
			{
				bStart=false;
				
				tmp += instring[j];
				
				j++;
			}
		}
		result += tmp;
		result += instring.substr(j);		
	}
//	wprintf(L"%s %s %s\n",instring.c_str(), basestring.c_str(), result.c_str());
	return result;
}

wstring SplitInLemmas(wstring & instring, wstring & basestring, map<wchar_t, wchar_t> & UpperLowermap, bool includeArrows)
{
	wstring result=L"";

	int idx=basestring.find_last_of(L'_');

	int j=0;
	int i=0;
	if ((idx ==string::npos) || (idx==basestring.length()-1))
	{
		result=instring;
	}
	else
	{
		bool bStart=true;
		wstring tmp;

		for(i=0, j=0; i<=idx+1 && j<instring.length(); i++)
		{
			if(wcschr(L"<]?~+=",basestring[i])) continue;

			if(basestring[i]==L'_')
			{
				if (!tmp.empty())
				{ //kahekümne k<aks_kümmend
					tmp.clear();
				}
				if ((!wcschr(L"<]?~+=",basestring[i+1])) && (basestring[i+1] != instring[j]))
				{ 
					//seitsmeteistkümne seitse_t<eist
					//paarisaja p<aar_sada
					j++;
				}
				if (includeArrows) result += L'γ';
				result += L' ';
				if (includeArrows) result += L'γ';
				bStart=true;
			}
			else if(bStart && ((basestring[i]==instring[j]) || (basestring[i]==UpperLowermap[instring[j]])))
			{ //start of stem the same
				result += basestring[i];
				
				j++;
			}
			else
			{
				bStart=false;
				
				tmp += instring[j];
				
				j++;
			}
		}
		result += tmp;
		result += instring.substr(j);		

//		wprintf(L"%s %s %s\n",instring.c_str(), basestring.c_str(), result.c_str());
	}
	
	return result;
}

wstring SplitInLemmasOnly(wstring & instring, wstring & basestring, map<wchar_t, wchar_t> & UpperLowermap, bool includeArrows)
{
	wstring result=L"";

	int idx=basestring.find_last_of(L'_');

	int j=0;
	int i=0;
	if ((idx ==string::npos) || (idx==basestring.length()-1))
	{
		result=instring;
	}
	else
	{
		for(i=0; i<basestring.length(); i++)
		{
			if(wcschr(L"<]?~+=",basestring[i])) continue;

			if(basestring[i]==L'_')
			{
				if (includeArrows) result += L'γ';
				result += L' ';
				if (includeArrows) result += L'γ';
			}
			else
			{
				result += basestring[i];
			}

		}
	}
	
	return result;
}
/*******************************************************************/
/*                                                                 */
/*  main                                                           */
/*                                                                 */
/*******************************************************************/
int main( int argc, char **argv )
{
	setbuf(stdout, NULL);

	FSCInit();

    // Command line parsing
    if ((argc > 1 && (!strcmp(argv[1],"-h") ||
       !strcmp(argv[1],"-help") ||
       !strcmp(argv[1],"-?"))) || (argc < 2))
    {
      fprintf(stderr,"\nUsage: %s et_lex_file [-a|-f|-af|-l|-al]\n-a\tarrows separating compound words\n-f\tlemmas for the first n-1 words\n-af\tarrows, lemmas for the first n-1 words\n-l\tall lemmas\n-al\tarrows, all lemmas\n", argv[0]);
      exit(1);
    }
	bool includeArrows = false;
	if (argc>2 && !strcmp(argv[2],"-a")) includeArrows = true;
	
	bool blemmas=false;
	if (argc>2 && !strcmp(argv[2],"-f")) blemmas = true;

	if (argc>2 && !strcmp(argv[2],"-af")) 
	{	
		includeArrows = true;
		blemmas = true;
	}

	bool balllemmas=false;
	if (argc>2 && !strcmp(argv[2],"-l")) balllemmas = true;

	if (argc>2 && !strcmp(argv[2],"-al")) 
	{	
		includeArrows = true;
		balllemmas = true;
	}

	CFSString lexPathCfa = argv[1];

    ETMRFA m_Morf;
	m_Morf.Start(lexPathCfa, MF_DFLT_MORFA | MF_KR6NKSA);
    m_Morf.SetMaxTasand();

   // m_Morf.Clr();

	setbuf(stdout,NULL);

		//Baltic, English and German letters
	wstring UpperChars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ\x0100\x010C\x0112\x0122\x012A\x0136\x013B\x0145\x0160\x016A\x017D\x00D5\x00C4\x00D6\x00DC\x0104\x0118\x0116\x012E\x0172";
	wstring LowerChars = L"abcdefghijklmnopqrstuvwxyz\x0101\x010D\x0113\x0123\x012B\x0137\x013C\x0146\x0161\x016B\x017E\x00F5\x00E4\x00F6\x00FC\x0105\x0119\x0117\x012F\x0173";

	map<wchar_t, wchar_t> UpperLowermap;
	for (unsigned int i = 0; i < UpperChars.length();i++)
	{
		UpperLowermap[UpperChars[i]]=LowerChars[i];
		UpperLowermap[LowerChars[i]]=LowerChars[i];
	}

	char buffer[10000];
	char *pStart=buffer;

	const std::wistream::int_type ZERO_WIDTH_NO_BREAK_SPACE = 0xFEFF;
    while (fgets(buffer, 9999, stdin))
	{  

		if (buffer[0] == ZERO_WIDTH_NO_BREAK_SPACE)
		{ 
			//removing BOM
			pStart= buffer+1; 
		}
		else
			pStart= buffer;


		string line_end= "\n";

		int32_t l=strlen(pStart)-1;  // delete the newline character
		if (pStart[l] == '\n')
			pStart[l] = '\0';
		if ((l>0) && (pStart[l-1] == '\x0D'))
		{
			pStart[l-1] = '\0';
		}
		string token;
		string newstring;

		std::basic_istringstream<char>  in(pStart);

		in >> token;

		while(!token.empty())
		{
			if (!newstring.empty())
			{
				newstring += ' ';
			}
			bool bPrinted = false;

			if (token.length()>5)
			{
				m_Morf.Clr();
				CFSAString tok=token.c_str();

				m_Morf.Set1(tok);
				m_Morf.Tag<int>(0, PRMS_TAGSINT);
				LYLI Lyli;

				while (m_Morf.Flush(Lyli)) 
				{
					if (Lyli.lipp & PRMS_MRF) 
					{
						Lyli.ptr.pMrfAnal->StrctKomadLahku();
						
						if(Lyli.ptr.pMrfAnal->idxLast>0)
						{
							MRFTUL Tul = *(*Lyli.ptr.pMrfAnal)[(int)0];
							wstring tempLemma1 = Tul.tyvi.GetBuffer(Tul.tyvi.GetLength());
						
							wstring original =(*(CFSBaseString<wchar_t,CFSStrFunctions<wchar_t> >*)(&(*Lyli.ptr.pMrfAnal).s6na)).GetBuffer(0);
							wstring tempLemma2;
							if (balllemmas)
							{
								tempLemma2=SplitInLemmasOnly(original, tempLemma1, UpperLowermap, includeArrows);
							}
							else if (blemmas)
							{
								tempLemma2=SplitInLemmas(original, tempLemma1, UpperLowermap, includeArrows);
							}
							else
							{
								tempLemma2=SplitInWords(original, tempLemma1, UpperLowermap, includeArrows);
							}
							CFSWString wide = tempLemma2.c_str();
							CFSAString ansi = FSStrWtoA(wide, FSCP_UTF8);

							newstring += ansi.GetBuffer(0);
							bPrinted=true;
						}
					}

				}
			}

			if (!bPrinted)
			{
				newstring += token;
			}
			token.clear();
			in >> token;
		}

		printf("%s\n", newstring.c_str());
	//	flushall();
	}

	return 0;
}