#pragma once

#include <time.h>

class Timer
{
public:
	Timer();
	~Timer();
};
/*
const time_t timer = time(NULL);
printf("%s\n", ctime(&timer));


time_t t = time(NULL);
tm* aTm = localtime(&t);
printf("%04d/%02d/%02d %02d:%02d:%02d \n", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
getchar();
*/