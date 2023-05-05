#ifndef CUPP_H
#define CUPP_H

#define MAX_USERNAMES_SIZE			10243
#define MAX_EVERYTHING_SIZE			MAX_USERNAMES_SIZE * 10
#define ENDPOINT_SIZE				50
#define TELEGRAM_BASE_URL			"https://api.telegram.org"

#ifdef parson_parson_h
	#define JSON_PARSE_STRING(obj)				\
		json_parse_string(obj)

	#define JSON_GET_BOOL(obj, name)		\
		json_object_dotget_boolean(json_object(obj), name);

	#define JSON_GET_STRING(obj, name)	\
		json_object_dotget_string(json_object(obj), name);

	#define JSON_GET_NUMBER(obj, name)	\
		json_object_dotget_number(json_object(obj), name);

#endif

typedef struct {
	char *url;
	unsigned int len;
} CUPPInit;

struct ForceReply {
	bool force_reply; /* Must be true */
	char *input_field_placeholder;
	bool selective;
};

CUPPInit cupp_init(const char *base, const char *token);
char *cupp_get_me(CUPPInit cuppi);
char *cupp_logout(CUPPInit cuppi);
char *cupp_close(CUPPInit cuppi);
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
);
char *cupp_forward_message(
	CUPPInit cuppi,
	const char *chat_id,
	int message_thread_id,
	const char *from_chat_id,
	bool disable_notification,
	bool protect_content,
	int message_id
);
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
);
char *cupp_get_my_name(CUPPInit cuppi, const char *lang);

#endif /* CUPP_H */
