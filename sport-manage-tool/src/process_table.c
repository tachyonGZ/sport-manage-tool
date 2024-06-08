#include "../inc/pch.h"
#include "../inc/process_table.h"

inline static void _send_menu_msg()
{
	send_message(M_TABLE_MENU, NULL, 0);
}

void process_table_menu()
{
	_print_split();

	fputs("│请输入要进行的操作\n", stdout);

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_TABLE_ATHLETE, NULL, 0);
		break;
	case '2':
		send_message(M_TABLE_SCORE, NULL, 0);
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

void process_table_menu_interface()
{
	fputs(
		"┌───\n" \
		"│汇总报表菜单\n"\
		"├───\n" \
		"│1.按比赛项目汇总运动员信息\n" \
		"│2.学院汇总参赛成绩的统计结果\n" \
		"├───\n" \
		"│提示：输入Q或q可以返回上一级菜单\n" \
		"└───\n" \
		, stdout);

	_send_menu_msg();
}

typedef struct {
	FILE *pFile;
} _TABLE_ATHLETE_ENUM_PROC_PARAM;

inline bool _table_athlete_is_natural_joined(const MATCH *pMatch, const SCORE *pScore, const ATHLETE *pAthlete)
{
	return pAthlete->id == pScore->athleteId && pScore->matchId == pMatch->id;
}

void _table_athlete_enum_proc(const MATCH *pMatch, const SCORE *pScore, const ATHLETE *pAthlete, _TABLE_ATHLETE_ENUM_PROC_PARAM *pParam)
{
	if (!_table_athlete_is_natural_joined(pMatch, pScore, pAthlete))
	{
		return;
	}

	fprintf(
		pParam->pFile,
		"%s,%s,%03d,%s,%s,%s\n",
		pMatch->szName, pMatch->szGender, pAthlete->id, pAthlete->szName, pAthlete->szGender, pAthlete->szCollege
	);

	//_statistics_match_param_add_athlete_count(pParam, pAthlete, pMatch);
}

void process_table_athlete()
{
	CARTESIAN_PRODUCT *pCp = make_cartesian_product_3d(get_matches_internal(), get_score_data_internal(), get_athletes_internal());

	_TABLE_ATHLETE_ENUM_PROC_PARAM param;
	memset(&param, 0, sizeof(param));
	
	param.pFile = fopen(".\\athlete.csv", "w");
	if (NULL == param.pFile)
	{
		abort();
	}

	fputs("比赛项目,项目性别,参赛编号,姓名,性别,学院\n", param.pFile);
	
	enum_cartesian_product_3d(pCp, _table_athlete_enum_proc, &param);

	//_printf_statistics_athlete(&param);

	fclose(param.pFile);

	release_cartesian_product(pCp);

	fputs("│汇总报表完成。\n", stdout);

	_send_menu_msg();
}

#define MAX_COLLEGE 32

typedef struct _sMATCH_INFO_NODE{
	matchId_t matchId;
	uint8_t a_iRankCount[3];
	struct _sMATCH_INFO_NODE *pNext;
} _MATCH_INFO_NODE;

typedef struct {
	char szCollege[32];
	_MATCH_INFO_NODE *pNode;
} _TABLE_COLLEGE_INFO;

typedef struct {
	_TABLE_COLLEGE_INFO a_collegeInfo[MAX_COLLEGE];
	size_t zCollegeInfoCount;
} _TABLE_SCORE_ENUM_PROC_PARAM;



inline bool _table_score_is_natural_joined(const ATHLETE *pAthlete, const SCORE *pScore)
{
	return pAthlete->id == pScore->athleteId;
}

void _table_score_add_rank_count(_MATCH_INFO_NODE *pMatchInfoNode, const char *szRank)
{
	if (0 == strcmp("第一名", szRank))
	{
		pMatchInfoNode->a_iRankCount[0] += 1;
		return;
	}
	else if (0 == strcmp("第二名", szRank))
	{
		pMatchInfoNode->a_iRankCount[1] += 1;
		return;
	}
	else if (0 == strcmp("第三名", szRank))
	{
		pMatchInfoNode->a_iRankCount[2] += 1;
		return;
	}
}

void _table_score_enum_proc(const ATHLETE *pAthlete, const SCORE *pScore, _TABLE_SCORE_ENUM_PROC_PARAM *pParam)
{
	if (!_table_score_is_natural_joined(pAthlete, pScore))
	{
		return;
	}

	size_t i = 0;
	for(i = 0; i <= pParam->zCollegeInfoCount; i++)
	{
		if (i == pParam->zCollegeInfoCount)
		{
			strcpy(pParam->a_collegeInfo[i].szCollege, pAthlete->szCollege);
			pParam->a_collegeInfo[i].pNode = NULL;
		}

		if (0 == strcmp(pParam->a_collegeInfo[i].szCollege, pAthlete->szCollege))
		{
			break;
		}
	}

	if (i == pParam->zCollegeInfoCount)
	{
		pParam->zCollegeInfoCount++;
	}
	/*
	for (size_t i = 0; i < pParam->zCollegeInfoCount; i++)
	{
		_MATCH_INFO_NODE *pPrev = NULL;
		
		if (0 != strcmp(pAthlete->szCollege, pParam->a_collegeInfo[i].szCollege))
		{
			continue;
		}
	*/
		_MATCH_INFO_NODE *pPrev = NULL;

		for (_MATCH_INFO_NODE *pNode = pParam->a_collegeInfo[i].pNode; pNode != NULL; pNode = pNode->pNext)
		{
			if (pScore->matchId == pNode->matchId)
			{
				_table_score_add_rank_count(pNode, pScore->szRank);
				return;
			}

			pPrev = pNode;
		}

		if (pPrev)
		{
			pPrev->pNext = malloc(sizeof(_MATCH_INFO_NODE));
			if (NULL == pPrev->pNext)
			{
				abort();
			}
			memset(pPrev->pNext, 0, sizeof(_MATCH_INFO_NODE));
			pPrev->pNext->matchId = pScore->matchId;
			_table_score_add_rank_count(pPrev->pNext, pScore->szRank);
		}
		else
		{
			pParam->a_collegeInfo[i].pNode = malloc(sizeof(_MATCH_INFO_NODE));
			if (NULL == pParam->a_collegeInfo[i].pNode)
			{
				abort();
			}
			memset(pParam->a_collegeInfo[i].pNode, 0, sizeof(_MATCH_INFO_NODE));
			pParam->a_collegeInfo[i].pNode->matchId = pScore->matchId;
			_table_score_add_rank_count(pParam->a_collegeInfo[i].pNode, pScore->szRank);
		}
		
	//}
}

void _make_table_score(_TABLE_SCORE_ENUM_PROC_PARAM *pParam)
{
	FILE *pFile = fopen(".\\score.csv", "w");
	if (NULL == pFile)
	{
		abort();
	}

	fputs("学院,比赛项目编号,第一名,第二名,第三名\n", pFile);

	for (size_t i = 0; i < pParam->zCollegeInfoCount; i++)
	{
		uint8_t aTotalRankCount[3] = {0, 0, 0};
		for (_MATCH_INFO_NODE *pNode = pParam->a_collegeInfo[i].pNode; pNode != NULL; pNode = pNode->pNext)
		{
			fprintf(
				pFile,
				"%s,%d,%d,%d,%d\n",
				pParam->a_collegeInfo[i].szCollege,
				pNode->matchId,
				pNode->a_iRankCount[0],
				pNode->a_iRankCount[1],
				pNode->a_iRankCount[2]
			);

			aTotalRankCount[0] += pNode->a_iRankCount[0];
			aTotalRankCount[1] += pNode->a_iRankCount[1];
			aTotalRankCount[2] += pNode->a_iRankCount[2];
		}

		fprintf(
			pFile,
			",合计,%d,%d,%d\n",
			aTotalRankCount[0],
			aTotalRankCount[1],
			aTotalRankCount[2]
		);
	}

	fclose(pFile);
}

void process_table_score()
{
	CARTESIAN_PRODUCT *pCp = make_cartesian_product_2d(get_athletes_internal(), get_score_data_internal());
	
	_TABLE_SCORE_ENUM_PROC_PARAM param;
	memset(&param, 0, sizeof(param));

	enum_cartesian_product_2d(pCp, _table_score_enum_proc, &param);

	_make_table_score(&param);

	release_cartesian_product(pCp);

	fputs("│汇总报表完成。\n", stdout);
	_send_menu_msg();
}