#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    struct feedback {
        int f_id ;
        char feedback[50] ;
    };

    int fd1,last;
    struct feedback m1 ,m2;
    close(fd1);
    fd1 = open("feedback_db",O_RDONLY);

    lseek(fd1,-1*sizeof(m2),SEEK_END);

    read(fd1,&m2,sizeof(m2));
    last = m2.f_id ;
    lseek(fd1,0,SEEK_SET);
    int c = 0 ;
    while(1){
    read(fd1,&m2,sizeof(m2));
    c++ ;
    printf("client id : %d\n",m2.f_id);
    printf("feed back : %s\n",m2.feedback);
    if (c == 2){break;}
    // if (m2.f_id == last){
    //     break;
    // }
    }
}