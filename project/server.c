#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include "temp.h"

int main()
{

    struct admin {
        char admin_name[10] ;
        int admin_id ;
        int password ;
        int count ;
    };

    struct client {
        char cust_name[10] ;
        int cust_id ;
        int password ;
        int paisa ;
        bool acc_status ;
        int count ;
    };

    struct emp {
        char emp_name[10] ;
        int emp_id ;
        int password ;
        int count ;
    };

    struct manager {
        char mg_name[10];
        int mg_id ;
        int password;
        int count ;
    };

    struct sockaddr_in serv, cli ;
    int sd,nsd,sz;
    sd = socket(AF_UNIX,SOCK_STREAM,0);
    perror("");
    serv.sin_family = AF_UNIX ;
    serv.sin_addr.s_addr = INADDR_ANY ;
    serv.sin_port = htons(6012);

    bind(sd,(void *) &serv, sizeof(serv));
    perror("");
    
    listen(sd,5);
    sz = sizeof(cli);
    while(1)
    {
        
        nsd = accept(sd,(void *) &cli,&sz);
        int pas,id,num,fd1,status = 0 ;   // status : - login, num :- role
        // perror("");
        if (!fork()){
            // close(sd);
            printf("Im in\n");


            while(1){
            read(nsd,&num,sizeof(int));
            if (num > 0 && num < 5){
                break;
                }
            }

            printf("choice is : %d\n",num);
            // login_as(num);
            if (num == 1){         // customer
                read(nsd,&id,sizeof(id));
                read(nsd,&pas,sizeof(pas));
                
            }

            else if (num == 2){     // employee
                read(nsd,&id,sizeof(id));
                read(nsd,&pas,sizeof(pas));
            }

            else if (num == 3){      // manager
                read(nsd,&id,sizeof(id));
                read(nsd,&pas,sizeof(pas));
            }

            else {                  // Admin
                read(nsd,&id,sizeof(id));
                printf("id is : %d\n",id);
                read(nsd,&pas,sizeof(pas));
                printf("password is : %d\n",pas);

                                                      // Login starts
                struct admin ad ;
                int start = id%100;
                fd1 = open("admin_db",O_RDONLY);

                struct flock lck ;
                lck.l_start = (start-1)*sizeof(ad);
                lck.l_len = sizeof(ad) ;
                lck.l_type = F_RDLCK;
                lck.l_whence = SEEK_SET;
                lck.l_pid = getpid();
                printf("before entering critical...\n");
                fcntl(fd1,F_SETLKW,&lck);
                lseek(fd1,(start-1)*sizeof(ad),SEEK_SET);
                
                read(fd1,&ad,sizeof(ad));

                // printf("enter key : \n");
                // getchar();
                // getchar();

                lck.l_type = F_UNLCK;
                fcntl(fd1,F_SETLK,&lck);
                close(fd1);
                if (ad.count == 1){
                    status = 2 ;
                    // write(nsd,&status,sizeof(int));
                }
                if (id == ad.admin_id){
                    if (ad.password == pas){
                        status = 1 ;
                        write(nsd,&status,sizeof(int));
                    }
                    else {
                        status = 0 ;
                        write(nsd,&status,sizeof(int));
                    }
                }
                else {
                    if (id != ad.admin_id){
                        status = 0 ;
                    }
                    write(nsd,&status,sizeof(int));
                }

                if (status == 0 || status == 2){exit(0);}
                else {
                    ad.count = 1 ;
                    fd1 = open("admin_db",O_RDWR);

                    struct flock lck ;
                    lck.l_start = (start-1)*sizeof(ad);
                    lck.l_len = sizeof(ad) ;
                    lck.l_type = F_WRLCK;
                    lck.l_whence = SEEK_SET;
                    lck.l_pid = getpid();
                    printf("before entering critical...\n");
                    fcntl(fd1,F_SETLKW,&lck);
                    lseek(fd1,(start-1)*sizeof(ad),SEEK_SET);
                    
                    write(fd1,&ad,sizeof(ad));

                    lck.l_type = F_UNLCK;
                    fcntl(fd1,F_SETLK,&lck);
                    close(fd1);
                }

                // login ends

                // operations starts

                int op,fd2, last ;             // last :- last id
                char name[10];
                while(1){
                    read(nsd,&op,sizeof(op));
                    printf("operation is : %d\n",op);
                    if (op == 1){
                        struct emp em;
                        char e_name[10];
                        int start1 ;
                        fd2 = open("emp_db",O_RDWR);

                        lseek(fd2,-1*sizeof(em),SEEK_END);

                        read(fd2,&em,sizeof(em));
                        last = em.emp_id ;
                        printf("last is : %d\n",last);

                        em.emp_id = last + 1 ;
                        start1 = (last + 1)%100;
                        read(nsd,e_name,sizeof(e_name));
                        perror("");
                        read(nsd,&(em.password),sizeof(em.password));
                        strcpy(em.emp_name , e_name) ;
                        em.count = 0 ;
                        // printf("name : %s\n",em.emp_name);
                        // printf("id : %d\n",em.emp_id);
                        // printf("pas : %d\n",em.password);
                        lseek(fd2,0,SEEK_SET);
                        lseek(fd2,(start1-1)*sizeof(em),SEEK_SET);
                        write(fd2,&em,sizeof(em));
                        close(fd2);

                    }
                    else if (op == 2){
                        int id,start2 ;
                        int balance ;
                        char name[10];
                        struct client cli ;
                        read(nsd,&id,sizeof(id));
                        start2 = id%100 ;
                        fd1 = open("clients_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start2-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start2-1)*sizeof(cli),SEEK_SET);
                        
                        read(fd1,&cli,sizeof(cli));
                        

                        write(nsd,&(cli.paisa),sizeof(cli.paisa));
                        write(nsd,&(cli.cust_name),sizeof(cli.cust_name));
                        int inner ;                                           // for name and balance
                        read(nsd,&inner,sizeof(inner));
                        if (inner==1){
                            read(nsd,&(cli.cust_name),sizeof(cli.cust_name));
                            lseek(fd1,(start2-1)*sizeof(cli),SEEK_SET);
                            write(fd1,&cli,sizeof(cli));
                            lseek(fd1,(start2-1)*sizeof(cli),SEEK_SET);
                            read(fd1,&cli,sizeof(cli));
                            write(nsd,&(cli.cust_name),sizeof(cli.cust_name));
                        }
                        else if (inner == 2)
                        {
                            read(nsd,&(cli.paisa),sizeof(cli.paisa));
                            lseek(fd1,(start2-1)*sizeof(cli),SEEK_SET);
                            write(fd1,&cli,sizeof(cli));
                            lseek(fd1,(start2-1)*sizeof(cli),SEEK_SET);
                            read(fd1,&cli,sizeof(cli));
                            write(nsd,&(cli.paisa),sizeof(cli.paisa));
                        }
                        // printf("enter key : \n");
                        // getchar();
                        // getchar();

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);

                    }
                    else if(op == 3){
                        int id,start3 ;
                        int last,fd2,check=0 ;
                        struct emp em ;
                        fd2 = open("emp_db",O_RDWR);

                        lseek(fd2,-1*sizeof(em),SEEK_END);

                        read(fd2,&em,sizeof(em));
                        last = em.emp_id ;
                        close(fd2);
                        char name[10];
                        
                        read(nsd,&id,sizeof(id));
                        if (id < 200 || id >last ){write(nsd,&check,sizeof(check));continue;}
                        start3 = id%100 ;
                        fd1 = open("emp_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start3-1)*sizeof(cli);
                        lck.l_len = sizeof(em) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start3-1)*sizeof(em),SEEK_SET);
                        
                        read(fd1,&em,sizeof(em));
                        if(em.count == -1){write(nsd,&check,sizeof(check));continue;} 
                        check = 1 ;
                        write(nsd,&check,sizeof(check));
                        write(nsd,&(em.emp_name),sizeof(em.emp_name));
                        read(nsd,&(em.emp_name),sizeof(em.emp_name));
                        lseek(fd1,(start3-1)*sizeof(em),SEEK_SET);
                        write(fd1,&em,sizeof(em));
                        lseek(fd1,(start3-1)*sizeof(em),SEEK_SET);
                        read(fd1,&em,sizeof(em));
                        write(nsd,&(em.emp_name),sizeof(em.emp_name));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
    
                    }
                    else if (op == 4){
                        int id,start3 ;
                        int last,fd2,check=0 ;
                        struct emp em ;
                        fd2 = open("emp_db",O_RDWR);

                        lseek(fd2,-1*sizeof(em),SEEK_END);

                        read(fd2,&em,sizeof(em));
                        last = em.emp_id ;
                        close(fd2);
                        char name[10];
                        
                        read(nsd,&id,sizeof(id));
                        if (id < 200 || id >last ){write(nsd,&check,sizeof(check));continue;}
                        start3 = id%100 ;
                        fd1 = open("emp_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start3-1)*sizeof(cli);
                        lck.l_len = sizeof(em) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start3-1)*sizeof(em),SEEK_SET);
                        
                        read(fd1,&em,sizeof(em));
                        if(em.count == -1){write(nsd,&check,sizeof(check));continue;} 
                        check = 1 ;
                        write(nsd,&check,sizeof(check));
                        
                        em.count = -1 ;
                        lseek(fd1,(start3-1)*sizeof(em),SEEK_SET);
                        write(fd1,&em,sizeof(em));
                    

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);

                        struct manager mg ;
                        int start31 ;
                        fd2 = open("manager_db",O_RDWR);

                        lseek(fd2,-1*sizeof(mg),SEEK_END);
                        read(fd2,&mg,sizeof(mg));
                        last = mg.mg_id ;
                        strcpy(mg.mg_name , em.emp_name);
                        mg.mg_id = last + 1 ;
                        mg.count = 0 ;
                        mg.password = em.password ;
                        printf("%s\n",mg.mg_name);
                        printf("%d\n",mg.mg_id);
                        printf("%d\n",mg.count);
                        printf("%d\n",mg.password);
                        start31 = (last+1)%100;
                        lseek(fd2,0,SEEK_SET);
                        lseek(fd2,(start31-1)*sizeof(mg),SEEK_SET);
                        write(fd2,&mg,sizeof(mg));


                        close(fd2);
                        write(nsd,&(mg.mg_id),sizeof(mg.mg_id));       // Ack
                        

                    }
                    else if (op == 5){
                        fd1 = open("admin_db",O_RDWR);
                        int new_pass ;
                        struct flock lck ;
                        read(nsd,&new_pass,sizeof(int));
                        lck.l_start = (start-1)*sizeof(ad);
                        lck.l_len = sizeof(ad) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(ad),SEEK_SET);
                        
                        ad.password = new_pass;
                        write(fd1,&ad,sizeof(ad));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);

                    }
                    else {
                        fd1 = open("admin_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(ad);
                        lck.l_len = sizeof(ad) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(ad),SEEK_SET);
                        ad.count = 0 ;
                        write(fd1,&ad,sizeof(ad));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        exit(0);
                    }

                    // exit(0);         // remove later for while loop

                }
                


            }
            


            exit(0);
        }
        else{
        sleep(1);
        close(nsd);
        }
    }
}