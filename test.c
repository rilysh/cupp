#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cupp.h"

int main()
{
	char *res;
	CUPPInit cui;
	struct ForceReply fer = { 0 };

	cui = cupp_init(
		NULL, "bot_token"
	);

    res = cupp_send_message(cui, "my_test_group", // Change the group name in where the bot exists
        0, "Hey! This is a test message", NULL, false, true, false, 0, false, fer
    );

    fprintf(stdout, "Result: %s\n", res);

	free(cui.url);
	free(res);
}
