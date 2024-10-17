#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

int main()
{
    struct client {
        char *cust_name ;
        int cust_id ;
        char * password ;
        int paisa ;
        bool acc_status ;
    };

    int fd1;
    struct client c1 ,c2;
    c1.cust_name = "Ashutosh";
    c1.cust_id = 101 ;
    c1.password = "1234";
    c1.paisa = 3000;
    c1.acc_status = true ;

    fd1 = open("clients_db",O_CREAT | O_EXCL | O_RDWR , 0744);
    write(fd1,&c1,sizeof(c1));
    close(fd1);
    fd1 = open("clients_db",O_RDONLY);

    read(fd1,&c2,sizeof(c2));
    printf("%d\n",c2.paisa);
}