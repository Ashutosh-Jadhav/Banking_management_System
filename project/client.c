#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{


    int sd, c ,status ;
    struct sockaddr_in serv ;
    char *id ,*pas;
    sd = socket(AF_UNIX,SOCK_STREAM,0);
    perror("");
    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY ;
    serv.sin_port = htons(6011);
    connect(sd,(void*)&serv,sizeof(serv));
    perror("");
    while(1){
        printf("1 : customer  2 : employee  3 : manager  4 : Admin  5 : exit\n");
        printf("login as : ");
        scanf("%d",&c);
        if (c == 5) {exit(0);}
        if (c>0 && c < 5){
            break;
        }
        printf("wrong choice\n");
    }
    printf("login ID : ");
    scanf("%s",id);
    printf("password : ");
    scanf("%s",pas);
    write(sd, &c,sizeof(c));
    write(sd, id,sizeof(id));
    write(sd, pas,sizeof(pas));

    if (c == 1){               // customer

    }

    else if (c == 2){          // employee

    }
    
    else if (c == 3){          // manager

    }

    else {                    // Admin
        
        read(sd,&status,sizeof(int));
        if (status == 0){
            printf("wrong credentials\n");
            shutdown(sd,0);
        }

        while(1){
            printf("1 : Add new bank employee \n
                    2 : Modify customer / employee details\n
                    3 : manage user Roles\n
                    4 : change password \n
                    5 : logout\n
                    6 : exit\n")
            printf("Enter operation : ");
            scanf("%d",&c);

        }
    }

        exit(0);
    

}