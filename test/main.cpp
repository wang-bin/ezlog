#include "ezlog.h"

int main(int argc, char** argv)
{
	ezlog_init_output("log.txt", Append);
	ezlog_msg("hello world!");
	ezlog_error("Damn %d", 123);
	ezlog_msg();

	ezlog_log("log file!");

	ezlog_fini();
	return 0;
}

