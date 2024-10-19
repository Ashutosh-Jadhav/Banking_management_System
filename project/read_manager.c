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

    int fd1,last;
    struct manager m1 ,m2;
    close(fd1);
    fd1 = open("manager_db",O_RDONLY);

    lseek(fd1,-1*sizeof(m2),SEEK_END);

    read(fd1,&m2,sizeof(m2));
    last = m2.mg_id ;
    lseek(fd1,0,SEEK_SET);

    while(1){
    read(fd1,&m2,sizeof(m2));

    printf("%s\n",m2.mg_name);
    printf("%d\n",m2.mg_id);
    printf("%d\n",m2.password);
    printf("%d\n",m2.count);
    if (m2.mg_id == last){
        break;
    }
    }
}