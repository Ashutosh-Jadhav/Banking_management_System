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

    int fd1;
    struct feedback m1 ,m2;
    char *s = "Mast hai Bank";
    strcpy(m1.feedback,s);
    m1.f_id = 101 ;
    

    fd1 = open("feedback_db", O_RDWR  );
    lseek(fd1,0,SEEK_END);
    write(fd1,&m1,sizeof(m1));
    close(fd1);
    fd1 = open("feedback_db",O_RDONLY);

    read(fd1,&m2,sizeof(m2));
    printf("client id : %d\n",m2.f_id);
    printf("feed back : %s\n",m2.feedback);

}