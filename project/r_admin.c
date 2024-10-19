#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    struct admin {
        char admin_name[10] ;
        int admin_id ;
        int password ;
        int count ;
    };

    int fd1,last;
    struct admin m1 ,m2;
    close(fd1);
    fd1 = open("admin_db",O_RDONLY);

    lseek(fd1,-1*sizeof(m2),SEEK_END);

    read(fd1,&m2,sizeof(m2));
    last = m2.admin_id ;
    lseek(fd1,0,SEEK_SET);

    while(1){
    read(fd1,&m2,sizeof(m2));

    printf("%s\n",m2.admin_name);
    printf("%d\n",m2.admin_id);
    printf("%d\n",m2.password);
    printf("%d\n",m2.count);
    if (m2.admin_id == last){
        break;
    }
    }
}