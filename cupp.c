#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

/* global defination of ps_fields */
char ps_fields[MAX_EVERYTHING_SIZE];

CUPPInit cupp_init(const char *base, const char *token)
{
	CUPPInit cuppi;

	cuppi.len = base == NULL
		? strlen(TELEGRAM_BASE_URL) + strlen(token) + 3
		: strlen(base) + strlen(token) + 1;

	cuppi.url = malloc(cuppi.len);

	ZERO_MEM(cuppi.url, cuppi.len);

	append_str(cuppi.url, "%s/%s/",
		base == NULL ? TELEGRAM_BASE_URL : base
		, token
	);

	return cuppi;
}

char *cupp_get_me(CUPPInit cuppi)
{
	char *url;

	url = create_url(cuppi, "getMe");

	return cupp_get(url);
}

char *cupp_logout(CUPPInit cuppi)
{
	char *url;

	url = create_url(cuppi, "logOut");

	return cupp_get(url);
}

char *cupp_close(CUPPInit cuppi)
{
	char *url;

	url = create_url(cuppi, "close");

	return cupp_get(url);
}

char *cupp_send_message(
	CUPPInit cuppi,
	const char *chat_id,
	int message_thread_id,
	const char *text,
	const char *parse_mode,
	bool disable_web_page_preview,
	bool disable_notification,
	bool protect_content,
	int reply_to_message_id,
	bool allow_sending_without_reply,
	struct ForceReply fer
)
{
	unsigned int chat_id_len, msg_len;

	chat_id_len = chat_id != NULL ? strlen(chat_id) : 0;
	msg_len = text != NULL ? strlen(text) : 0;
 
	if (chat_id_len == 0 || msg_len == 0)
		eerr(EXIT_FAILURE,
			"\"chat_id\" and \"msg\" cannot be empty.",
			__func__
		);

	char *res, urle[cuppi.len + ENDPOINT_SIZE];

	ZERO_MEM(ps_fields, MAX_EVERYTHING_SIZE);
	ZERO_MEM(urle, sizeof(urle));

	append_str(ps_fields,
		"chat_id=@%s&text=%s"
		"&disable_web_page_preview=%s"
		"&disable_notification=%s"
		"&protect_content=%s",
		chat_id, text
		, disable_web_page_preview ? "true" : "false"
		, disable_notification ? "true" : "false"
		, protect_content ? "true" : "false"
	);

	if (message_thread_id > 0)
		append_str(ps_fields,
			"&message_thread_id=%d",
			message_thread_id
		);

	    
	if (parse_mode != NULL)
		append_str(ps_fields, "&parse_mode=%s", parse_mode);

	if (reply_to_message_id > 0)
		append_str(ps_fields,
			"&reply_to_message_id=%d",
			reply_to_message_id
		);

	append_str(ps_fields,
		"&allow_sending_without_reply=%s",
		allow_sending_without_reply ? "true" : "false"
	);

	if (fer.input_field_placeholder != NULL &&
		fer.force_reply) {
			fer.force_reply = true;
		append_str(ps_fields,
			"&force_reply=true&input_field_placeholder=%s&selective=%s"
			, fer.input_field_placeholder, fer.selective ? "true" : "false"
		);
	}

	append_str(urle, "%ssendMessage", cuppi.url);

	res = cupp_post(urle, ps_fields);

	return res;
}

char *cupp_forward_message(
	CUPPInit cuppi,
	const char *chat_id,
	int message_thread_id,
	const char *from_chat_id,
	bool disable_notification,
	bool protect_content,
	int message_id
)
{
	unsigned int chat_id_len;

	chat_id_len = chat_id != NULL ? strlen(chat_id) : 0;

	if (chat_id_len == 0)
		eerr(EXIT_FAILURE,
			"\"chat_id\" and \"msg\" cannot be empty.",
			__func__
		);

	char *res, urle[cuppi.len + ENDPOINT_SIZE];

	ZERO_MEM(ps_fields, sizeof(ps_fields));
	ZERO_MEM(urle, sizeof(urle));

	append_str(ps_fields,
		"chat_id=@%s"
		"&from_chat_id=@%s"
		"&message_id=%d",
		chat_id, from_chat_id, message_id
	);

	if (message_thread_id > 0)
		append_str(ps_fields,
			"&message_thread_id=%d",
			message_thread_id
		);

	append_str(ps_fields,
		"&disable_notification=%s&protect_content=%s",
		disable_notification ? "true" : "false"
		, protect_content ? "true" : "false"
	);

	append_str(urle, "%sforwardMessage", cuppi.url);

	res = cupp_post(urle, ps_fields);

	return res;
}

char *cupp_copy_message(
	CUPPInit cuppi,
	const char *chat_id,
	int message_thread_id,
	const char *from_chat_id,
	int message_id,
	const char *caption,
	const char *parse_mode,
	bool disable_notification,
	bool protect_content,
	int reply_to_message_id,
	bool allow_sending_without_reply,
	struct ForceReply fer
)
{
	unsigned int chat_id_len;

	chat_id_len = chat_id != NULL ? strlen(chat_id) : 0;

	if (chat_id_len == 0)
		eerr(EXIT_FAILURE,
			"\"chat_id\" and \"msg\" cannot be empty.",
			__func__
		);

	char *res, urle[cuppi.len + ENDPOINT_SIZE];

	ZERO_MEM(ps_fields, sizeof(ps_fields));
	ZERO_MEM(urle, sizeof(urle));

	append_str(ps_fields,
		"chat_id=@%s&from_chat_id=@%s&message_id=%d"
		, chat_id, from_chat_id, message_id
	);

	if (message_thread_id > 0)
		append_str(ps_fields,
			"&message_thread_id=%d",
			message_thread_id
		);

	if (caption != NULL)
		append_str(ps_fields, "&caption=%s", caption);

	if (parse_mode != NULL)
		append_str(ps_fields, "&parse_mode=%s", parse_mode);

	append_str(ps_fields,
		"&disable_notification=%s&protect_content=%s&reply_to_message_id=%d&allow_sending_without_reply=%s"
		, disable_notification ? "true" : "false"
		, protect_content ? "true" : "false"
		, reply_to_message_id
		, allow_sending_without_reply ? "true" : "false"
	);

	if (fer.input_field_placeholder != NULL &&
		fer.force_reply) {
			fer.force_reply = true;
			append_str(ps_fields,
				"&force_reply=true&input_field_placeholder=%s&selective=%s"
				, fer.input_field_placeholder, fer.selective ? "true" : "false"
			);
	}

	append_str(urle, "%scopyMessage", cuppi.url);

	res = cupp_post(urle, ps_fields);

	return res;
}

// TODO: sendPhoto

char *cupp_get_my_name(CUPPInit cuppi, const char *lang)
{
	unsigned int lang_len;

	lang_len = lang == NULL ? 0 : strlen(lang);

	char *res;
	char urle[cuppi.len + ENDPOINT_SIZE];

	ZERO_MEM(urle, sizeof(urle));
	ZERO_MEM(ps_fields, sizeof(ps_fields));

	append_str(urle, "%sgetMyName", cuppi.url);

	if (lang_len > 0)
		append_str(ps_fields, "language_code=%s", lang);
	else
		strcat(ps_fields, "language_code=");

	res = cupp_post(urle, ps_fields);

	return res;
}
