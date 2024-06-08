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
		fputs("����Ч�����룡\n", stdout);
		_send_menu_msg();
	}

}

void process_password_login()
{
	//_print_split();

	fputs("���������¼����...\n", stdout);

	if (is_password_right(get_user_input_by_str()))
	{
		send_message(M_MAIN_MENU_INTERFACE, NULL, 0);
		return;
	}

	fputs("��������������\n", stdout);
	_send_menu_msg();
}

void process_password_change()
{
	//_print_split();

	fputs(\
		"��������Ҫ���õ�������...\n" \
		"����ʾ���µ����볤��ҪС��32λ��\n" \
	, stdout);

	if (reset_password(get_user_input_by_str()))
	{

		fputs(\
			"���������óɹ���\n" \
			, stdout);
	}
	else
	{

		fputs(\
			"����������ʧ�ܣ�\n" \
			, stdout);
	}

	send_message(M_MAIN_MENU, NULL, 0);
}

void process_password_menu_interface()
{
	fputs(\
		"��������\n" \
		"����ӭʹ�ô�ѧ���ﾶ�˶������ϵͳ\n"\
		"��������\n" \
		"���س��������е�¼\n" \
		"��������\n" \
		"����ʾ������Q��q���Է�����һ���˵�\n" \
		"��������\n" \
	, stdout);

	_send_menu_msg();
}