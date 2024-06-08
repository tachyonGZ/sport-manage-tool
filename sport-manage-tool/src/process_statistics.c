#include "../inc/pch.h"

#include "../inc/process_statistics.h"

inline static void _send_menu_msg()
{
	send_message(M_STATISTICS_MENU, NULL, 0);
}

void process_statistics_menu()
{
	_print_split();

	fputs("│请输入要进行的操作\n", stdout);

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
		fputs("│无效的输入！请重试。\n", stdout);
		_send_menu_msg();
	}
}

void process_statistics_menu_interface()
{
	fputs(
		"┌───\n" \
		"│统计信息（统计管理）菜单\n"\
		"├───\n" \
		"│1.统计运动员信息\n" \
		"│2.统计比赛项目信息\n"	\
		"├───\n" \
		"│提示：输入Q或q可以返回上一级菜单\n" \
		"└───\n" \
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
			if (0 == strcmp("男", pAthlete->szGender))
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

	if (0 == strcmp("第一名", pScore->szRank))
	{
		_statistics_athlete_param_athlete_add_rank_count(pParam, pAthlete, 0);
	}
	else if (0 == strcmp("第二名", pScore->szRank))
	{
		_statistics_athlete_param_athlete_add_rank_count(pParam, pAthlete, 1);
	}
	else if (0 == strcmp("第三名", pScore->szRank))
	{
		_statistics_athlete_param_athlete_add_rank_count(pParam, pAthlete, 2);
	}
	else
	{
		return;
	}

	//fprintf(stdout, "│运动员编号：%03d，运动员名称：%s，名次：%s，\n", pAthlete->id, pAthlete->szName, pScore->szRank);
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
	if (0 == strcmp(pAthlete->szGender, "男"))
	{
		iGender = 0;
	}
	else if(0 == strcmp(pAthlete->szGender, "女"))
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
	fputs("│┌───\n", stdout);

	for (size_t i = 0; i < pParam->iAthleteCount; i++)
	{
		fprintf(
			stdout,
			"││编号为%03d的运动员总共获得第一名%d项，第二名%d项，第三名%d项。\n",
			pParam->aAthleteInfo[i].athleteId,
			pParam->aAthleteInfo[i].a_iRankCount[0],
			pParam->aAthleteInfo[i].a_iRankCount[1],
			pParam->aAthleteInfo[i].a_iRankCount[2]
		);
	}

	fputs("│├───\n", stdout);

	for (size_t i = 0; i < pParam->iCollegeCount; i++)
	{
		fprintf(
			stdout,
			"││%s的运动员总共获得第一名%d项，第二名%d项，第三名%d项。\n" \
			"││其中男生共获得第一名%d项，第二名%d项，第三名%d项；女生共获得共获得第一名%d项，第二名%d项，第三名%d项。\n",
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
	fputs("│┌───\n", stdout);

	for (size_t i = 0; i < pParam->iMatchCount; i++)
	{
		fprintf(
			stdout,
			"││编号为%03d的比赛项目总共有%d人，其中男生共有%d人，女生共有%d人。\n",
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