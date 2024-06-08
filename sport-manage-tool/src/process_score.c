#include "../inc/pch.h"

#include "../inc/process_score.h"

inline static void _send_menu_msg()
{
	send_message(M_SCORE_MENU, NULL, 0);
}

void process_score_menu()
{
	_print_split();

	fputs("│请输入要进行的操作\n", stdout);

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_SCORE_ADD, NULL, 0);
		break;
	case '2':
		send_message(M_SCORE_REMOVE, NULL, 0);
		break;
	case '3':
		send_message(M_SCORE_REWRITE, NULL, 0);
		break;
	case '4':
		send_message(M_SCORE_QUERY, NULL, 0);
		break;
	case '5':
		send_message(M_SCORE_QUERY_ALL, NULL, 0);
		break;
	case '6':
		send_message(M_SCORE_FILTER, NULL, 0);
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

void process_score_menu_interface()
{
	fputs(
		"┌───\n" \
		"│参赛成绩管理菜单\n"\
		"├───\n" \
		"│1.添加参赛成绩\n" \
		"│2.删除参赛成绩\n"	\
		"│3.修改参赛成绩\n" \
		"│4.查询参赛成绩\n"	\
		"│5.查询所有参赛成绩\n" \
		"│6.筛选查询运动员信息\n" \
		"├───\n" \
		"│提示：输入Q或q可以返回上一级菜单\n" \
		"└───\n" \
	, stdout);

	_send_menu_msg();
}

bool _print_score_res(const DATA_RES_t res)
{
	static const char *mapScoreRes[DATA_RES_MAX] = {
		[DATA_RES_SUCCESS] = "│操作成功！\n",
		[DATA_RES_FAIL] = "│操作失败！\n",
		[DATA_RES_STORE_FAIL] = "│保存失败！\n",
		[DATA_RES_UNLOADED] = "│数据未加载！\n",
		[DATA_RES_KEY_CONFLICT] = "│输入的运动员编号和比赛项目编号组合与已存在的组合冲突！\n",
		[DATA_RES_INVALID_KEY] = "│无效的运动员编号或比赛项目编号！\n",
	};

	if (res < DATA_RES_MAX)
	{
		fputs(mapScoreRes[res], stdout);
	}

	return DATA_RES_SUCCESS == res;
}

void process_add_score()
{
	fputs( \
		"│请输入要添加的比赛成绩\n" \
		"│格式:<运动员编号> <比赛项目编号> <成绩> <名次> <是否参赛>\n" \
		"│例如:001 002 3分45秒 第二名 已参赛\n" \
	, stdout);

	// 用输入的数据初始化
	SCORE score;
	if(5 == sscanf(get_user_input_by_str(), "%hd %hd %s %s %s", &score.athleteId, &score.matchId, score.szResult, score.szRank, score.szparticipation))
	{
		if(_print_score_res(add_score(&score)))
		{
			fputs("│录入参赛成绩成功，该比赛项目的赛况已修改成功。\n", stdout);
		}
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void process_remove_score()
{
	fputs( \
		"│请输入要删除的比赛成绩对应的运动员编号和比赛项目编号的组合\n" \
		"│格式：<运动员编号> <比赛项目编号>\n" \
	, stdout);


	athleteId_t athleteId = 0;
	matchId_t matchId= 0;
	if(2 == sscanf(get_user_input_by_str(), "%hd %hd", &athleteId, &matchId))
	{
		_print_score_res(remove_score(athleteId, matchId));
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void process_rewrite_score()
{
	fputs( \
		"│请输入要修改的比赛成绩\n" \
		"│格式:<运动员编号> <比赛项目编号> <成绩> <名次> <是否参赛>\n" \
		"│例如:001 002 3分45秒 第二名 已参赛\n" \
	, stdout);

	SCORE score;
	if(5 == sscanf(get_user_input_by_str(), "%hd %hd %s %s %s", &score.athleteId, &score.matchId, score.szResult, score.szRank, score.szparticipation))
	{
		_print_score_res(rewrite_score(&score));
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void _print_score(const SCORE *pScore, void *pIgnore)
{
	// ─ │ ┌├└ ┘┐┤┬┴┼

	printf( \
		"│┌───\n" \
		"││运动员编号：%hd\n" \
		"││比赛项目编号：%hd\n" \
		"││成绩: %s\n" \
		"││名次：%s\n" \
		"││是否参赛：%s\n" \
		, pScore->athleteId, pScore->matchId, pScore->szResult, pScore->szRank, pScore->szparticipation
	);
}

void process_query_score()
{
	fputs( \
		"│请输入要查询的比赛成绩对应的运动员编号和比赛项目编号的组合\n" \
		"│格式：<运动员编号> <比赛项目编号>\n" \
	, stdout);

	athleteId_t athleteId = 0;
	matchId_t matchId = 0;
	if(2 == sscanf(get_user_input_by_str(), "%hd %hd", &athleteId, &matchId))
	{
		const SCORE *pScore = NULL;

		if (_print_score_res(query_score(athleteId, matchId, &pScore)))
		{
			_print_score(pScore, NULL);
		}
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void process_query_all_score()
{
	_print_score_res(enum_score(_print_score, NULL));

	_send_menu_msg();
}

typedef enum {
	_ATHLETE_ID = 1 << 0,
	_MATCH_ID = 1 << 1,
	_RESULT = 1 << 2,
	_RANK = 1 << 3,
	_PARTICIPATION = 1 << 4,
} _FILTER_SCORE_IGNORE_FLAG_MASK;

typedef uint32_t _FILTER_SCORE_IGNORE_FLAG;

inline  _FILTER_SCORE_IGNORE_FLAG_MASK _set_filter_score_ignore_mask(const _FILTER_SCORE_IGNORE_FLAG_MASK mask, const char *szInput)
{
	return ('*' != szInput[0])?(mask):(0);
}

_FILTER_SCORE_IGNORE_FLAG _make_filter_score_ignore_flag(const char *szAthleteId, const char *szMatchId, const SCORE *pScore)
{
	_FILTER_SCORE_IGNORE_FLAG ignoreFlag = 0;
	ignoreFlag |= _set_filter_score_ignore_mask(_ATHLETE_ID, szAthleteId);
	ignoreFlag |= _set_filter_score_ignore_mask(_MATCH_ID, szMatchId);
	ignoreFlag |= _set_filter_score_ignore_mask(_RESULT, pScore->szResult);
	ignoreFlag |= _set_filter_score_ignore_mask(_RANK, pScore->szRank);
	ignoreFlag |= _set_filter_score_ignore_mask(_PARTICIPATION, pScore->szparticipation);
	return ignoreFlag;
}


typedef struct {
	const SCORE *pScore;
	const _FILTER_SCORE_IGNORE_FLAG ignoreFlag;
} _FILTER_SCORE_ENUM_PARAM;

void _filter_score_enum_proc(const SCORE *pScore, const _FILTER_SCORE_ENUM_PARAM *pParam)
{
	if ( (pParam->ignoreFlag & _ATHLETE_ID) && (pParam->pScore->athleteId != pScore->athleteId) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _MATCH_ID) && (pParam->pScore->matchId != pScore->matchId) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _RESULT) && (0 != strcmp(pParam->pScore->szResult, pScore->szResult)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _RANK) && (0 != strcmp(pParam->pScore->szRank, pScore->szRank)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _PARTICIPATION) && (0 != strcmp(pParam->pScore->szparticipation, pScore->szparticipation)) )
	{
		return;
	}
	else
	{
		_print_score(pScore, NULL);
	}

	return;
}

void process_filter_score()
{
	fputs( \
		"│请输入筛选条件\n" \
		"│格式:<运动员编号> <比赛项目编号> <成绩> <名次> <是否参赛>\n" \
		"│忽略的筛选项可以用*替代\n"\
		"│例如:* 001 * 第一名 *\n" \
	, stdout);

	char szAthleteId[8];
	char szMatchId[8];
	SCORE score;
	if(5 != sscanf(get_user_input_by_str(), "%s %s %s %s %s", szAthleteId, szMatchId, score.szResult, score.szRank, score.szparticipation))
	{
		fputs("│输入格式有误！\n", stdout);
		_send_menu_msg();
		return;
	}

	_FILTER_SCORE_IGNORE_FLAG ignoreFlag = _make_filter_score_ignore_flag(szAthleteId, szMatchId, &score);

	if (ignoreFlag & _ATHLETE_ID)
	{
		score.athleteId = atoi(szAthleteId);

		if(errno)
		{
			fputs("│运动员编号格式有误！\n", stdout);
			_send_menu_msg();
			return;
		}
	}

	if (ignoreFlag & _MATCH_ID)
	{
		score.matchId = atoi(szMatchId);

		if(errno)
		{
			fputs("│比赛项目编号格式有误！\n", stdout);
			_send_menu_msg();
			return;
		}
	}


	_FILTER_SCORE_ENUM_PARAM enumParam = {
		.pScore = &score,
		.ignoreFlag = ignoreFlag
	};

	_print_score_res(enum_score(_filter_score_enum_proc, &enumParam));
	_send_menu_msg();
}