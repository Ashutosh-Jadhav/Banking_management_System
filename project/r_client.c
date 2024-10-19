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

    int fd1,last;
    struct client m1 ,m2;
    close(fd1);
    fd1 = open("clients_db",O_RDONLY);

    lseek(fd1,-1*sizeof(m2),SEEK_END);

    read(fd1,&m2,sizeof(m2));
    last = m2.cust_id ;
    lseek(fd1,0,SEEK_SET);

    while(1){
    read(fd1,&m2,sizeof(m2));

    printf("name : %s\n",m2.cust_name);
    printf("id : %d\n",m2.cust_id);
    printf("password : %d\n",m2.password);
    printf("login sessions : %d\n",m2.count);
    printf("balance : %d\n",m2.paisa);
    if (m2.cust_id == last){
        break;
    }
    }
}