#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

int main()
{
    struct manager {
        char *mg_name ;
        int mg_id ;
        char * password ;
    };

    int fd1;
    struct manager m1 ,m2;
    m1.mg_name = "Hayat";
    m1.mg_id = 1001 ;
    m1.password = "1234";

    fd1 = open("manager_db",O_CREAT | O_EXCL | O_RDWR , 0744);
    write(fd1,&m1,sizeof(m1));
    close(fd1);
    fd1 = open("manager_db",O_RDONLY);

    read(fd1,&m2,sizeof(m2));
    printf("%s\n",m2.password);
}