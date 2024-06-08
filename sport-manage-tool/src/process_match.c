#include "../inc/pch.h"

#include "../inc/process_match.h"

inline static void _send_menu_msg()
{
	send_message(M_MATCH_MENU, NULL, 0);
}

void process_match_menu()
{
	_print_split();

	fputs("│请输入要进行的操作\n", stdout);

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_MATCH_ADD, NULL, 0);
		break;
	case '2':
		send_message(M_MATCH_REMOVE, NULL, 0);
		break;
	case '3':
		send_message(M_MATCH_REWRITE, NULL, 0);
		break;
	case '4':
		send_message(M_MATCH_QUERY, NULL, 0);
		break;
	case '5':
		send_message(M_MATCH_QUERY_ALL, NULL, 0);
		break;
	case '6':
		send_message(M_MATCH_FILTER, NULL, 0);
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

void process_match_menu_interface()
{
	fputs(
		"┌───\n" \
		"│比赛项目管理菜单\n"\
		"├───\n" \
		"│1.添加比赛项目\n" \
		"│2.删除比赛项目\n"	\
		"│3.修改比赛项目\n" \
		"│4.查询比赛项目\n"	\
		"│5.查询所有比赛项目\n" \
		"│6.筛选查询比赛项目\n" \
		"├───\n" \
		"│提示：输入Q或q可以返回上一级菜单\n" \
		"└───\n" \
		, stdout);

	_send_menu_msg();
}

static bool _print_res(const DATA_RES_t res)
{
	static const char *mapMatchRes[DATA_RES_MAX] = {
		[DATA_RES_SUCCESS] = "│操作成功！\n",
		[DATA_RES_FAIL] = "│操作失败！\n",
		[DATA_RES_STORE_FAIL] = "│保存失败！\n",
		[DATA_RES_UNLOADED] = "│数据未加载！\n",
		[DATA_RES_KEY_CONFLICT] = "│输入的比赛项目编号与已存在的比赛项目编号冲突！\n",
		[DATA_RES_INVALID_KEY] = "│无效的比赛项目编号！\n",
	};

	if (res < DATA_RES_MAX)
	{
		fputs(mapMatchRes[res], stdout);
	}

	return DATA_RES_SUCCESS == res;
}

void process_add_match()
{
	fputs( \
		"│请输入要添加的比赛项目信息\n" \
		"│格式:<编号> <名称> <项目性别> <比赛时间> <比赛地点>\n" \
		"│例如:1 跳远 男 10:50AM 操场北门附近\n" \
	, stdout);

	// 用输入的数据初始化
	MATCH match;

	if(5 == sscanf(get_user_input_by_str(), "%hd %s %s %s %s", &match.id, match.szName, match.szGender, match.szTime, match.szLocation))
	{
		if (NULL == strcpy(match.szStatus, "未赛"))
		{
			abort();
		}

		_print_res(add_match(&match));
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void process_remove_match()
{
	fputs( \
		"│请输入要删除的比赛项目信息对应的比赛项目编号\n" \
	, stdout);

	matchKey_t iMatchId = 0;

	if(1 == sscanf(get_user_input_by_str(), "%hd", &iMatchId))
	{
		_print_res(remove_match(iMatchId));
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void process_rewrite_match()
{
	fputs( \
		"│请输入要修改的比赛项目信息\n" \
		"│格式:<编号> <名称> <项目性别> <比赛时间> <比赛地点> <赛况>\n" \
		"│例如:1 跳远 男 10:50AM 操场北门附近 已赛\n" \
	, stdout);

	// 用输入的数据初始化
	MATCH match;

	if(6 == sscanf(get_user_input_by_str(), "%hd %s %s %s %s %s", &match.id, match.szName, match.szGender, match.szTime, match.szLocation, match.szStatus))
	{
		_print_res(rewrite_match(&match));
	}
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void _print_match(const MATCH *pMatch, const void *pIgnore)
{
	// ─ │ ┌├└ ┘┐┤┬┴┼

	printf( \
		"│┌───\n" \
		"││编号：%d\n" \
		"││名称：%s\n" \
		"││项目性别: %s\n" \
		"││比赛时间：%s\n" \
		"││比赛地点：%s\n" \
		"││赛况：%s\n" \
		, pMatch->id, pMatch->szName, pMatch->szGender, pMatch->szTime, pMatch->szLocation, pMatch->szStatus
	);
}

void process_query_match()
{
	fputs( \
		"│请输入要查询的比赛项目信息对应的比赛项目编号\n" \
	, stdout);

	matchKey_t iAthleteId = 0;
	if(1 == sscanf(get_user_input_by_str(), "%hd", &iAthleteId))
	{
		const MATCH *pMatch = NULL;

		if (_print_res(query_match(iAthleteId, &pMatch)))
		{
			_print_match(pMatch, NULL);
		}
	}	
	else
	{
		fputs("│输入格式有误！\n", stdout);
	}

	_send_menu_msg();
}

void process_query_all_match()
{
	_print_res(enum_match(_print_match, NULL));
	
	_send_menu_msg();
}

typedef enum {
	_ID = 1 << 0,
	_NAME = 1 << 1,
	_GENDER = 1 << 2,
	_TIME = 1 << 3,
	_LOCATION = 1 << 4,
	_STATUS = 1 << 5
} _FILTER_MATCH_IGNORE_FLAG_MASK;

typedef uint32_t _FILTER_MATCH_IGNORE_FLAG;

inline  _FILTER_MATCH_IGNORE_FLAG_MASK _set_filter_match_ignore_mask(const _FILTER_MATCH_IGNORE_FLAG_MASK mask, const char *szInput)
{
	return ('*' != szInput[0])?(mask):(0);
}

_FILTER_MATCH_IGNORE_FLAG _make_filter_match_ignore_flag(const char *szMatchId, const MATCH *pMatch)
{
	_FILTER_MATCH_IGNORE_FLAG ignoreFlag = 0;
	ignoreFlag |= _set_filter_match_ignore_mask(_ID, szMatchId);
	ignoreFlag |= _set_filter_match_ignore_mask(_NAME, pMatch->szName);
	ignoreFlag |= _set_filter_match_ignore_mask(_GENDER, pMatch->szGender);
	ignoreFlag |= _set_filter_match_ignore_mask(_TIME, pMatch->szTime);
	ignoreFlag |= _set_filter_match_ignore_mask(_LOCATION, pMatch->szLocation);
	ignoreFlag |= _set_filter_match_ignore_mask(_STATUS, pMatch->szStatus);
	return ignoreFlag;
}

typedef struct {
	const MATCH *pMatch;
	const _FILTER_MATCH_IGNORE_FLAG ignoreFlag;
} _FILTER_MATCH_ENUM_PARAM;

void _filter_match_enum_proc(const MATCH *pMatch, const _FILTER_MATCH_ENUM_PARAM *pParam)
{
	if ( (pParam->ignoreFlag & _ID) && (pParam->pMatch->id != pMatch->id) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _NAME) && (0 != strcmp(pParam->pMatch->szName, pMatch->szName)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _GENDER) && (0 != strcmp(pParam->pMatch->szGender, pMatch->szGender)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _TIME) && (0 != strcmp(pParam->pMatch->szTime, pMatch->szTime)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _LOCATION) && (0 != strcmp(pParam->pMatch->szLocation, pMatch->szLocation)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _STATUS) && (0 != strcmp(pParam->pMatch->szStatus, pMatch->szStatus)) )
	{
		return;
	}
	else
	{
		_print_match(pMatch, NULL);
	}

	return;
}

void process_filter_match()
{
	fputs( \
		"│请输入筛选条件\n" \
		"│格式:<编号> <名称> <项目性别> <比赛时间> <比赛地点> <赛况>\n" \
		"│忽略的筛选项可以用*替代\n"\
		"│例如:* 跳远 男 * * *\n" \
		, stdout);

	char szMatchId[8];
	MATCH match;
	if(6 != sscanf(get_user_input_by_str(), "%s %s %s %s %s %s", szMatchId, match.szName, match.szGender, &match.szTime, match.szLocation, match.szStatus))
	{
		fputs("│输入格式有误！\n", stdout);
		_send_menu_msg();
		return;
	}
	_FILTER_MATCH_IGNORE_FLAG ignoreFlag = _make_filter_match_ignore_flag(szMatchId, &match);

	if (ignoreFlag & _ID)
	{
		match.id = atoi(szMatchId);

		if(errno)
		{
			fputs("│编号格式有误！\n", stdout);
			_send_menu_msg();
			return;
		}
	}

	_FILTER_MATCH_ENUM_PARAM enumParam = {
		.pMatch = &match,
		.ignoreFlag = ignoreFlag
	};

	_print_res(enum_match(_filter_match_enum_proc, &enumParam));
	_send_menu_msg();
}