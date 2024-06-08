#include "../inc/pch.h"
#include "../inc/process_password.h"

inline static void _send_menu_msg()
{
	send_message(M_PASSWORD_MENU, NULL, 0);
}

void process_password_menu()
{
	_print_split();

	switch(get_user_input_by_str()[0])
	{
	case '\0':
		send_message(M_PASSWORD_LOGIN, NULL, 0);
		break;
	case 'q':
	case 'Q':
		send_message(M_GOBACK, NULL, 0);
		break;
	default:
		fputs("│无效的输入！\n", stdout);
		_send_menu_msg();
	}

}

void process_password_login()
{
	//_print_split();

	fputs("│请输入登录密码...\n", stdout);

	if (is_password_right(get_user_input_by_str()))
	{
		send_message(M_MAIN_MENU_INTERFACE, NULL, 0);
		return;
	}

	fputs("│输入的密码错误！\n", stdout);
	_send_menu_msg();
}

void process_password_change()
{
	//_print_split();

	fputs(\
		"│请输入要设置的新密码...\n" \
		"│提示：新的密码长度要小于32位。\n" \
	, stdout);

	if (reset_password(get_user_input_by_str()))
	{

		fputs(\
			"│密码重置成功！\n" \
			, stdout);
	}
	else
	{

		fputs(\
			"│密码重置失败！\n" \
			, stdout);
	}

	send_message(M_MAIN_MENU, NULL, 0);
}

void process_password_menu_interface()
{
	fputs(\
		"┌───\n" \
		"│欢迎使用大学生田径运动会管理系统\n"\
		"├───\n" \
		"│回车键来进行登录\n" \
		"├───\n" \
		"│提示：输入Q或q可以返回上一级菜单\n" \
		"└───\n" \
	, stdout);

	_send_menu_msg();
}