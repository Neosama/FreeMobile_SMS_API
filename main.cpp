
// Send SMS notification with Free-Mobile API

#include <Windows.h>
#include <iostream>
#include <string>

#include "lib.h"

int main()
{
	printf("Free-Mobile SMS API C++\n");
	send_notification_SMS("API_USER", "API_PASS", "Hello World!");

	getchar();
	return 0;
}