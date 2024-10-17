#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

int main()
{
    struct emp {
        char *emp_name ;
        int emp_id ;
        char * password ;
    };

    int fd1;
    struct emp e1 ,e2;
    e1.emp_name = "Amol";
    e1.emp_id = 501 ;
    e1.password = "1234";

    fd1 = open("emp_db",O_CREAT | O_EXCL | O_RDWR , 0744);
    write(fd1,&e1,sizeof(e1));
    close(fd1);
    fd1 = open("emp_db",O_RDONLY);

    read(fd1,&e2,sizeof(e2));
    printf("%s\n",e2.emp_name);
}