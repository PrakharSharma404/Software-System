#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

void main(int arg_cnt, char *arg_vct[])
{
    time_t currentEpoch, timeoutEpoch;
    struct tm *timeout;
    pid_t child;

    if (arg_cnt < 2)
        printf("Pass at least one argument\n");
    else
    {
        time(&currentEpoch);
        timeout = localtime(&currentEpoch);

        timeout->tm_hour = atoi(arg_vct[1]);
        timeout->tm_min = arg_vct[2] == NULL ? 0 : atoi(arg_vct[2]);
        timeout->tm_sec = arg_vct[3] == NULL ? 0 : atoi(arg_vct[3]);

        timeoutEpoch = mktime(timeout);

        if ((child = fork()) == 0)
        {
            setsid();
            chdir("/");
            umask(0);
            do
            {
                time(&currentEpoch);
            } while (difftime(timeoutEpoch, currentEpoch) > 0);
            printf("its over! \n");
            exit(0);
        }
        exit(0);
    }
}
