#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    struct client {
        char cust_name[10] ;
        int cust_id ;
        int password ;
        int paisa ;
        bool acc_status ;
        int count ;
    };

    int fd1;
    char *s = "Ashutosh";
    struct client c1 ,c2;
    strcpy(c1.cust_name,s);
    c1.cust_id = 101 ;
    c1.password = 1234;
    c1.paisa = 3000;
    c1.acc_status = true ;
    c1.count = 0 ;

    fd1 = open("clients_db",O_CREAT | O_EXCL | O_RDWR , 0744);
    // lseek(fd1,sizeof(c1),SEEK_SET);
    write(fd1,&c1,sizeof(c1));
    close(fd1);
    fd1 = open("clients_db",O_RDONLY);

    read(fd1,&c2,sizeof(c2));
    printf("%s\n",c2.cust_name);
}