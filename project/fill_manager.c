#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    struct manager {
        char mg_name[10] ;
        int mg_id ;
        int password ;
        int count ;
    };

    int fd1;
    struct manager m1 ,m2;
    char *s = "Hayat";
    strcpy(m1.mg_name,s);
    m1.mg_id = 1001 ;
    m1.password = 1234;
    m1.count = 0;

    fd1 = open("manager_db", O_RDWR );
    write(fd1,&m1,sizeof(m1));
    close(fd1);
    fd1 = open("manager_db",O_RDONLY);

    read(fd1,&m2,sizeof(m2));
    printf("%s\n",m2.mg_name);
}