#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

int main()
{
    struct admin {
        char *admin_name ;
        int admin_id ;
        char * password ;
    };

    int fd1;
    struct admin a1 ,a2;
    a1.admin_name = "Anand";
    a1.admin_id = 1501 ;
    a1.password = "1234";

    fd1 = open("admin_db",O_CREAT | O_EXCL | O_RDWR , 0744);
    write(fd1,&a1,sizeof(a1));
    close(fd1);
    fd1 = open("admin_db",O_RDONLY);

    read(fd1,&a2,sizeof(a2));
    printf("%d\n",a2.admin_id);
}