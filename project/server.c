#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "temp.h"

int main()
{

    struct admin {
        char *admin_name ;
        int admin_id ;
        char * password ;
    };

    struct client {
        char *cust_name ;
        int cust_id ;
        char * password ;
        int paisa ;
        bool acc_status ;
    };

    struct emp {
        char *emp_name ;
        int emp_id ;
        char * password ;
    };

    struct manager {
        char *mg_name ;
        int mg_id ;
        char * password ;
    };

    struct sockaddr_in serv, cli ;
    int sd,nsd,sz;
    sd = socket(AF_UNIX,SOCK_STREAM,0);
    perror("");
    serv.sin_family = AF_UNIX ;
    serv.sin_addr.s_addr = INADDR_ANY ;
    serv.sin_port = htons(6011);

    bind(sd,(void *) &serv, sizeof(serv));
    perror("");

    listen(sd,5);
    while(1)
    {
        sz = sizeof(cli);
        nsd = accept(sd,(void*)&cli,&sz);
        perror("");
        if (!fork()){
            close(sd);


            int num,fd1,status = 0 ;   // status : - login, num :- role
            char *pas,*id ;
            read(nsd,&num,sizeof(int));
            // login_as(num);
            if (num == 1){         // customer
                read(nsd,id,sizeof(id));
                read(nsd,pas,sizeof(pas));
                
            }

            else if (num == 2){     // employee
                read(nsd,id,sizeof(id));
                read(nsd,pas,sizeof(pas));
            }

            else if (num == 3){      // manager
                read(nsd,id,sizeof(id));
                read(nsd,pas,sizeof(pas));
            }

            else {                  // Admin
                read(nsd,id,sizeof(id));
                read(nsd,pas,sizeof(pas));
                struct admin ad ;
                int start = id%10;
                fd1 = open("admin_db",O_RDONLY);

                struct flock lck ;
                lck.l_start = (start-1)*sizeof(ad);
                lck.l_len = sizeof(ad) ;
                lck.l_type = F_RDLCK;
                lck.l_whence = SEEK_SET;
                lck.l_pid = getpid();

                lseek(fd1,(start-1)*sizeof(db),SEEK_SET);
                printf("before entering critical...\n");
                fcntl(fd1,F_SETLKW,&lck);
                read(fd1,&ad,sizeof(ad));

                printf("enter key : \n");
                getchar();
                lck.l_type = F_UNLCK;
                fcntl(fd1,F_SETLK,&lck);

                if (ad.admin_id == id){
                    if (ad.admin_password == pas){
                        status = 1 ;
                        write(nsd,&status,sizeof(int));
                    }
                    else {
                        write(nsd,&status,sizeof(int));
                    }
                }
                else {
                    write(nsd,&status,sizeof(int));
                }

                


            }
            


            exit(0);
        }
        close(nsd);
    }
}