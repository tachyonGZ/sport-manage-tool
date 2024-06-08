#include "../inc/pch.h"

#include "../inc/message.h"
#include "../inc/process_athlete.h"
#include "../inc/process_match.h"
#include "../inc/process_score.h"
#include "../inc/process_statistics.h"
#include "../inc/process_password.h"
#include "../inc/process_table.h"
#include "../inc/input.h"

// ─│┌├└┘┐┤┬┴┼

// 界面层级
static int g_iUserHierarchy = 0;

void process_welcome_interface()
{
	fputs("欢迎使用大学生田径运动会管理系统\n", stdout);

	send_message(M_PASSWORD_MENU_INTERFACE, NULL, 0);
}

void process_main_menu()
{
	_print_split();

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_ATHLETE_MENU_INTERFACE, NULL, 0);
		break;
	case '2':
		send_message(M_MATCH_MENU_INTERFACE, NULL, 0);
		break;
	case '3':
		send_message(M_SCORE_MENU_INTERFACE, NULL, 0);
		break;
	
	case '4':
		send_message(M_STATISTICS_MENU_INTERFACE, NULL, 0);
		break;

	case '5':
		send_message(M_TABLE_MENU_INTERFACE, NULL, 0);
		break;

	case '6':
		send_message(M_PASSWORD_CHANGE, NULL, 0);
		break;
	case 'q':
	case 'Q':
		send_message(M_GOBACK, NULL, 0);
		break;
	default:
		fputs("无效的输入！\n", stdout);
		send_message(M_MAIN_MENU, NULL, 0);
	}
}

void process_main_menu_interface()
{
	fputs(\
		"┌───\n" \
		"│大学生田径运动会管理系统\n"\
		"├───\n" \
		"│1.管理运动员信息\n" \
		"│2.管理比赛项目信息\n" \
		"│3.管理参赛成绩信息\n" \
		"│4.统计信息(统计管理)\n" \
		"│5.汇总报表\n" \
		"│6.修改密码\n" \
		"│q.返回上一级\n" \
		"├───\n" \
		"│提示：输入Q或q可以返回上一级菜单\n" \
		"└───\n" \
	, stdout);

	send_message(M_MAIN_MENU, NULL, 0);

}

void process_startup()
{
	// 初始化输入缓冲区
	init_user_input(256);

	// 加载全局数据
	load_athletes();
	load_matches();	
	load_score_data();

	load_password();

	send_message(M_WELCOME_INTERFACE, NULL , 0);
}

void process_exit()
{
	// 释放全局数据
	clear_score_data();
	clear_matches();
	clear_athletes();
	
	// 释放缓冲区
	release_user_input();
}

void process_goback()
{

	system("cls");

	switch (g_iUserHierarchy)
	{
	case 0:
	case 1:
		send_message(M_EXIT, NULL, 0);
		break;
	case 2:
		send_message(M_MAIN_MENU_INTERFACE, NULL, 0);
		break;
	default:
		;
	}
}


int message_process(const message_t msg, const void *pParam, const size_t paramSize);

int main(int agrc, char **argv)
{
	if(!starup_message_queue(message_process))
	{
		exit(-1);
		return -1;
	}

	// 消息循环
	message_t msg;

	while (get_message(&msg))
	{
		dispatch_message(msg);
	}

	exit_message_queue();

	return 0;
}

void set_user_hierarchy(int iUserHierarchy)
{
	system("cls");
	g_iUserHierarchy = iUserHierarchy;
}

void set_user_hierarchy_by_message(const message_t msg)
{
	switch (msg)
	{
	case M_PASSWORD_MENU_INTERFACE:
		set_user_hierarchy(0);
	case M_MAIN_MENU_INTERFACE:
		set_user_hierarchy(1);
		break;
	case M_ATHLETE_MENU_INTERFACE:
	case M_MATCH_MENU_INTERFACE:
	case M_SCORE_MENU_INTERFACE:
	case M_STATISTICS_MENU_INTERFACE:
	case M_TABLE_MENU_INTERFACE:
		set_user_hierarchy(2);
		break;
	default:
		;
	}
}

typedef void (*PROCESS_CALLBACK)();

PROCESS_CALLBACK g_fnMapProcessCallback[MAX_MESSAGE_TYPE_NUM] = {
	// FrameWork
	[M_EXIT_PRE] = process_exit,
	[M_STARTUP] = process_startup,
	[M_GOBACK] = process_goback,

	// password
	[M_PASSWORD_MENU_INTERFACE] = process_password_menu_interface,
	[M_PASSWORD_MENU] = process_password_menu,
	[M_PASSWORD_LOGIN] = process_password_login,
	[M_PASSWORD_CHANGE] = process_password_change,

	[M_WELCOME_INTERFACE] = process_welcome_interface,
	[M_MAIN_MENU_INTERFACE] = process_main_menu_interface,
	[M_MAIN_MENU] = process_main_menu,

	// Athlete
	[M_ATHLETE_MENU_INTERFACE] = process_athlete_menu_interface,
	[M_ATHLETE_MENU] = process_athlete_menu,
	[M_ATHLETE_ADD] = process_add_athlete,
	[M_ATHLETE_REMOVE] = process_remove_athlete,
	[M_ATHLETE_REWRITE] = process_rewrite_athlete,
	[M_ATHLETE_QUERY] = process_query_athlete,
	[M_ATHLETE_QUERY_ALL] = process_query_all_athlete,
	[M_ATHLETE_FILTER] = process_filter_athlete,

	// match
	[M_MATCH_MENU_INTERFACE] = process_match_menu_interface,
	[M_MATCH_MENU] = process_match_menu,
	[M_MATCH_ADD] = process_add_match,
	[M_MATCH_REMOVE] = process_remove_match,
	[M_MATCH_REWRITE] = process_rewrite_match,
	[M_MATCH_QUERY] = process_query_match,
	[M_MATCH_QUERY_ALL] = process_query_all_match,
	[M_MATCH_FILTER] = process_filter_match,

	// score
	[M_SCORE_MENU_INTERFACE] = process_score_menu_interface,
	[M_SCORE_MENU] = process_score_menu,
	[M_SCORE_ADD] = process_add_score,
	[M_SCORE_REMOVE] = process_remove_score,
	[M_SCORE_REWRITE] = process_rewrite_score,
	[M_SCORE_QUERY] = process_query_score,
	[M_SCORE_QUERY_ALL] = process_query_all_score,
	[M_SCORE_FILTER] = process_filter_score,

	// statistics
	[M_STATISTICS_MENU_INTERFACE] = process_statistics_menu_interface,
	[M_STATISTICS_MENU] = process_statistics_menu,
	[M_STATISTICS_ATHLETE] = process_statistics_athlete,
	[M_STATISTICS_MATCH] = process_statistics_match,

	// table
	[M_TABLE_MENU_INTERFACE] = process_table_menu_interface,
	[M_TABLE_MENU] = process_table_menu,
	[M_TABLE_ATHLETE] = process_table_athlete,
	[M_TABLE_SCORE] = process_table_score,
};

int message_process(const message_t msg, const void *pParam, const size_t paramSize)
{
	set_user_hierarchy_by_message(msg);

	if (msg < 0 || msg >= MAX_MESSAGE_TYPE_NUM)
	{
		return -1;
	}

	g_fnMapProcessCallback[msg]();

	return 0;
}