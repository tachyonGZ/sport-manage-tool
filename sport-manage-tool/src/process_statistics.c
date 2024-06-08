#include "../inc/pch.h"

#include "../inc/process_statistics.h"

inline static void _send_menu_msg()
{
	send_message(M_STATISTICS_MENU, NULL, 0);
}

void process_statistics_menu()
{
	_print_split();

	fputs("��������Ҫ���еĲ���\n", stdout);

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_STATISTICS_ATHLETE, NULL, 0);
		break;
	case '2':
		send_message(M_STATISTICS_MATCH, NULL, 0);
		break;
	case 'q':
	case 'Q':
		send_message(M_GOBACK, NULL, 0);
		break;
	default:
		fputs("����Ч�����룡�����ԡ�\n", stdout);
		_send_menu_msg();
	}
}

void process_statistics_menu_interface()
{
	fputs(
		"��������\n" \
		"��ͳ����Ϣ��ͳ�ƹ����˵�\n"\
		"��������\n" \
		"��1.ͳ���˶�Ա��Ϣ\n" \
		"��2.ͳ�Ʊ�����Ŀ��Ϣ\n"	\
		"��������\n" \
		"����ʾ������Q��q���Է�����һ���˵�\n" \
		"��������\n" \
		, stdout);

	_send_menu_msg();
}

inline bool _statistics_athlete_is_natural_joined(const ATHLETE *pAthlete, const SCORE *pScore)
{
	return pAthlete->id == pScore->athleteId;
}

inline bool _statistics_match_is_natural_joined(const ATHLETE *pAthlete, const SCORE *pScore, const MATCH *pMatch)
{
	return pAthlete->id == pScore->athleteId && pScore->matchId == pMatch->id;
}

typedef struct{
	athleteId_t athleteId;
	uint8_t a_iRankCount[3];
} _ATHLETE_INFO;

typedef struct{
	char szCollege[32];
	uint8_t a_iRankMaleCount[3];
	uint8_t a_iRankFemaleCount[3];
} _COLLEGE_INFO;

#define MAX_ATHLETE 32
#define MAX_COLLEGE 32

typedef struct {
	_ATHLETE_INFO aAthleteInfo[MAX_ATHLETE];
	size_t iAthleteCount;
	_COLLEGE_INFO aCollegeInfo[MAX_COLLEGE];
	size_t iCollegeCount;
} _STATISTIC_ATHLETE_ENUM_PROC_PARAM;

void _statistics_athlete_param_athlete_add_rank_count(_STATISTIC_ATHLETE_ENUM_PROC_PARAM *pParam, const ATHLETE *pAthlete, const size_t iRank)
{
	size_t i = 0;

	for (i = 0; i <= pParam->iAthleteCount; i++)
	{
		if (i == pParam->iAthleteCount)
		{
			pParam->aAthleteInfo[i].athleteId = pAthlete->id;
		}

		if (pAthlete->id == pParam->aAthleteInfo[i].athleteId)
		{
			pParam->aAthleteInfo[i].a_iRankCount[iRank] += 1;
			break;
		}
	}

	if (i == pParam->iAthleteCount)
	{
		pParam->iAthleteCount++;
	}


	for ( i = 0; i <= pParam->iCollegeCount; i++)
	{
		if (i == pParam->iCollegeCount)
		{
			strcpy(pParam->aCollegeInfo[i].szCollege, pAthlete->szCollege);
		}

		if (0 == strcmp(pAthlete->szCollege, pParam->aCollegeInfo[i].szCollege))
		{
			if (0 == strcmp("��", pAthlete->szGender))
			{
				pParam->aCollegeInfo[i].a_iRankMaleCount[iRank] += 1;
			}
			else
			{
				pParam->aCollegeInfo[i].a_iRankFemaleCount[iRank] += 1;
			}
			break;
		}
	}

	if (i == pParam->iCollegeCount)
	{
		pParam->iCollegeCount++;
	}
}


void statistics_athlete_enum_proc(const ATHLETE *pAthlete, const SCORE *pScore, _STATISTIC_ATHLETE_ENUM_PROC_PARAM *pParam)
{
	if (!_statistics_athlete_is_natural_joined(pAthlete, pScore))
	{
		return;		
	}

	if (0 == strcmp("��һ��", pScore->szRank))
	{
		_statistics_athlete_param_athlete_add_rank_count(pParam, pAthlete, 0);
	}
	else if (0 == strcmp("�ڶ���", pScore->szRank))
	{
		_statistics_athlete_param_athlete_add_rank_count(pParam, pAthlete, 1);
	}
	else if (0 == strcmp("������", pScore->szRank))
	{
		_statistics_athlete_param_athlete_add_rank_count(pParam, pAthlete, 2);
	}
	else
	{
		return;
	}

	//fprintf(stdout, "���˶�Ա��ţ�%03d���˶�Ա���ƣ�%s�����Σ�%s��\n", pAthlete->id, pAthlete->szName, pScore->szRank);
}

typedef struct{ 
	matchId_t matchId;
	uint8_t a_iAthleteCount[2];
} _MATCH_INFO;

#define MAX_MATCH 32
typedef struct {
	_MATCH_INFO aMatchInfo[MAX_MATCH];
	size_t iMatchCount;
} _STATISTIC_MATCH_ENUM_PROC_PARAM;


void _statistics_match_param_add_athlete_count(_STATISTIC_MATCH_ENUM_PROC_PARAM *pParam, const ATHLETE *pAthlete, const MATCH *pMatch)
{
	size_t iGender;
	if (0 == strcmp(pAthlete->szGender, "��"))
	{
		iGender = 0;
	}
	else if(0 == strcmp(pAthlete->szGender, "Ů"))
	{
		iGender = 1;
	}
	else
	{
		return;
	}

	size_t i = 0;

	for (i = 0; i <= pParam->iMatchCount; i++)
	{
		if (i == pParam->iMatchCount)
		{
			pParam->aMatchInfo[i].matchId = pMatch->id;
		}

		if (pMatch->id == pParam->aMatchInfo[i].matchId)
		{
			pParam->aMatchInfo[i].a_iAthleteCount[iGender] += 1;
			break;
		}
	}

	if (i == pParam->iMatchCount)
	{
		pParam->iMatchCount++;
	}
}

void statistics_match_enum_proc(const ATHLETE *pAthlete, const SCORE *pScore, const MATCH *pMatch, _STATISTIC_MATCH_ENUM_PROC_PARAM *pParam)
{
	if (!_statistics_match_is_natural_joined(pAthlete, pScore, pMatch))
	{
		return;
	}

	_statistics_match_param_add_athlete_count(pParam, pAthlete, pMatch);
}

void _printf_statistics_athlete(_STATISTIC_ATHLETE_ENUM_PROC_PARAM *pParam)
{
	fputs("����������\n", stdout);

	for (size_t i = 0; i < pParam->iAthleteCount; i++)
	{
		fprintf(
			stdout,
			"�������Ϊ%03d���˶�Ա�ܹ���õ�һ��%d��ڶ���%d�������%d�\n",
			pParam->aAthleteInfo[i].athleteId,
			pParam->aAthleteInfo[i].a_iRankCount[0],
			pParam->aAthleteInfo[i].a_iRankCount[1],
			pParam->aAthleteInfo[i].a_iRankCount[2]
		);
	}

	fputs("����������\n", stdout);

	for (size_t i = 0; i < pParam->iCollegeCount; i++)
	{
		fprintf(
			stdout,
			"����%s���˶�Ա�ܹ���õ�һ��%d��ڶ���%d�������%d�\n" \
			"����������������õ�һ��%d��ڶ���%d�������%d�Ů������ù���õ�һ��%d��ڶ���%d�������%d�\n",
			pParam->aCollegeInfo[i].szCollege,
			pParam->aCollegeInfo[i].a_iRankMaleCount[0] + pParam->aCollegeInfo[i].a_iRankFemaleCount[0],
			pParam->aCollegeInfo[i].a_iRankMaleCount[1] + pParam->aCollegeInfo[i].a_iRankFemaleCount[1],
			pParam->aCollegeInfo[i].a_iRankMaleCount[2] + pParam->aCollegeInfo[i].a_iRankFemaleCount[2],
			pParam->aCollegeInfo[i].a_iRankMaleCount[0],
			pParam->aCollegeInfo[i].a_iRankMaleCount[1],
			pParam->aCollegeInfo[i].a_iRankMaleCount[2],
			pParam->aCollegeInfo[i].a_iRankFemaleCount[0],
			pParam->aCollegeInfo[i].a_iRankFemaleCount[1],
			pParam->aCollegeInfo[i].a_iRankFemaleCount[2]
		);
	}
}

void _printf_statistics_match(_STATISTIC_MATCH_ENUM_PROC_PARAM *pParam)
{
	fputs("����������\n", stdout);

	for (size_t i = 0; i < pParam->iMatchCount; i++)
	{
		fprintf(
			stdout,
			"�������Ϊ%03d�ı�����Ŀ�ܹ���%d�ˣ�������������%d�ˣ�Ů������%d�ˡ�\n",
			pParam->aMatchInfo[i].matchId,
			pParam->aMatchInfo[i].a_iAthleteCount[0] + pParam->aMatchInfo[i].a_iAthleteCount[1],
			pParam->aMatchInfo[i].a_iAthleteCount[0],
			pParam->aMatchInfo[i].a_iAthleteCount[1]
		);
	}
}


void process_statistics_athlete()
{
	CARTESIAN_PRODUCT *pCp = make_cartesian_product_2d(get_athletes_internal(), get_score_data_internal());
	
	_STATISTIC_ATHLETE_ENUM_PROC_PARAM param;
	memset(&param, 0, sizeof(param));
	enum_cartesian_product_2d(pCp, statistics_athlete_enum_proc, &param);
	
	_printf_statistics_athlete(&param);

	release_cartesian_product(pCp);
	
	_send_menu_msg();
}

void process_statistics_match()
{
	CARTESIAN_PRODUCT *pCp = make_cartesian_product_3d(get_athletes_internal(), get_score_data_internal(), get_matches_internal());
	
	_STATISTIC_MATCH_ENUM_PROC_PARAM param;
	memset(&param, 0, sizeof(param));
	enum_cartesian_product_3d(pCp, statistics_match_enum_proc, &param);
	
	_printf_statistics_match(&param);
	
	release_cartesian_product(pCp);

	_send_menu_msg();
}