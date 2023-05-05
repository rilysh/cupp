#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <curl/curl.h>
#include "cupp.h"

#define ZERO_MEM(src, size)		\
	memset(src, '\0', (unsigned long long)size)

struct store_node {
	char *memory;
	unsigned int size;
};

static void eerr(
	int code, const char *err,
	const char *fn
)
{
	fprintf(stderr, "%s(): %s\n", fn, err);
	exit(code);
}

static void perr(const char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

static unsigned long int write_to_mem(
	const void *data, unsigned long int size,
	unsigned long int nmemb, void *cbp
)
{
	unsigned long int rsize;
	struct store_node *mem;

	rsize = size * nmemb;
	mem = (struct store_node *)cbp;
	mem->memory = malloc(rsize + 1);
	mem->size = 0;

	ZERO_MEM(mem->memory, rsize + 1);

	memcpy(&mem->memory[mem->size], data, rsize);
	mem->size += rsize;
	mem->memory[mem->size] = 0;

	return rsize;
}

static char *create_url(CUPPInit cuppi, const char *endp)
{
	char *urle = malloc(cuppi.len + ENDPOINT_SIZE);

	ZERO_MEM(urle, cuppi.len + ENDPOINT_SIZE);
	strcat(urle, cuppi.url);
	strcat(urle, endp);

	return urle;
}

static char *append_str(char *ps_fields, const char *fmt, ...)
{
	char fmt_str[MAX_EVERYTHING_SIZE];
	va_list vlist;
    va_start(vlist, fmt);

	ZERO_MEM(fmt_str, sizeof(fmt_str));
	vsprintf(fmt_str, fmt, vlist);
	strcat(ps_fields, fmt_str);
	va_end(vlist);

	return ps_fields; 
}

static char *cupp_get(char *url)
{
    CURL *curl;
	CURLcode ret;
	struct store_node mem;

	curl = curl_easy_init();

	if (!curl)
		perr("curl_easy_init()");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_mem);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);
	ret = curl_easy_perform(curl);

	if (ret != CURLE_OK) {
		curl_easy_cleanup(curl);

		return (char *)curl_easy_strerror(ret);
	}

    curl_easy_cleanup(curl);

    return mem.memory;
}

static char *cupp_post(const char *url, const char *fields)
{
	CURL *curl;
	CURLcode ret;
	struct store_node mem;

	curl = curl_easy_init();

	if (!curl)
		perr("curl_easy_init()");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_mem);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);
	ret = curl_easy_perform(curl);

	if (ret != CURLE_OK) {
		curl_easy_cleanup(curl);

		return (char *)curl_easy_strerror(ret);
	}

    curl_easy_cleanup(curl);

	return mem.memory;
}

#endif /* UTILS_H */
