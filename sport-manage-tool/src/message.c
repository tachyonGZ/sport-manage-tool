#include "../inc/pch.h"

#include "../inc/message.h"

MESSAGE_QUEUE g_msgQueue = {
	.bAlive = false
};

bool starup_message_queue(MSG_PROC fnMsgProc)
{
	if (g_msgQueue.bAlive)
	{
		return false;
	}

	g_msgQueue.fnMsgProc = fnMsgProc;
	g_msgQueue.bAlive = true;	
	return send_message(M_STARTUP, NULL, 0);
}

void exit_message_queue()
{
	g_msgQueue.bAlive = false;
}

bool get_message(message_t *pMsg)
{
	if (NULL == pMsg || !g_msgQueue.bAlive)
	{
		return false;
	}

	*pMsg = g_msgQueue.msg;
	return M_EXIT != *pMsg;
}

bool send_message(const message_t msg, void *pParam, const size_t paramSize)
{
	if (!g_msgQueue.bAlive)
	{
		return false;
	}

	g_msgQueue.msg = msg;
	return true;
}

bool dispatch_message(const message_t msg)
{
	if (!g_msgQueue.bAlive || NULL == g_msgQueue.fnMsgProc)
	{
		return false;
	}

	return g_msgQueue.fnMsgProc(msg, NULL, 0);
}