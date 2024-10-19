#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    struct emp {
        char emp_name[10] ;
        int emp_id ;
        int password;
        int count ;
    };

    int fd1;
    char *s = "Amol";
    struct emp e1 ,e2;
    strcpy(e1.emp_name,s);
    e1.emp_id = 501 ;
    e1.password = 1234;
    e1.count = 0;

    fd1 = open("emp_db",O_CREAT | O_EXCL | O_RDWR , 0744);
    write(fd1,&e1,sizeof(e1));
    close(fd1);
    fd1 = open("emp_db",O_RDONLY);
    // lseek(fd1,sizeof(e1),SEEK_SET);
    read(fd1,&e2,sizeof(e2));
    printf("%s\n",e2.emp_name);
    printf("%d\n",e2.password);
    printf("%d\n",e2.emp_id);
}