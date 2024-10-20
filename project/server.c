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
    struct feedback {
        int f_id ;
        char feedback[50] ;
    };
    struct loan {
        int l_id ;
        int cust_id ;
        int emp_id ;
        int amount;
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
            int l_ack = 1;
            printf("Im in\n");


            while(1){
            read(nsd,&num,sizeof(int));
            if (num <= 0 && num >= 5){
                continue;
                }
            

            printf("choice is : %d\n",num);
            // login_as(num);
            if (num == 1){         // customer
                int l_ack = 1;
                read(nsd,&id,sizeof(id));
                read(nsd,&pas,sizeof(pas));

                                        // login starts
                struct client cli ;
                int start = id%100;
                fd1 = open("clients_db",O_RDONLY);

                struct flock lck ;
                lck.l_start = (start-1)*sizeof(cli);
                lck.l_len = sizeof(cli) ;
                lck.l_type = F_RDLCK;
                lck.l_whence = SEEK_SET;
                lck.l_pid = getpid();
                printf("before entering critical...\n");
                fcntl(fd1,F_SETLKW,&lck);
                lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                
                read(fd1,&cli,sizeof(cli));

                // printf("enter key : \n");
                // getchar();
                // getchar();

                lck.l_type = F_UNLCK;
                fcntl(fd1,F_SETLK,&lck);
                close(fd1);
                if (cli.count == 1){
                    status = 2 ;
                    write(nsd,&status,sizeof(int));
                }
                else if (id == cli.cust_id){
                    if (cli.password == pas){
                        status = 1 ;
                        write(nsd,&status,sizeof(int));
                    }
                    else {
                        status = 0 ;
                        write(nsd,&status,sizeof(int));
                    }
                }
                else {
                    status = 0 ;
                    write(nsd,&status,sizeof(int));
                }

                if (cli.count == -1){
                    status = 0 ;
                    write(nsd,&status,sizeof(int));
                }
                
                

                if (status == 0 || status == 2){write(nsd,&l_ack,sizeof(l_ack));continue;}
                else {
                    cli.count = 1 ;
                    fd1 = open("clients_db",O_RDWR);

                    struct flock lck ;
                    lck.l_start = (start-1)*sizeof(cli);
                    lck.l_len = sizeof(cli) ;
                    lck.l_type = F_WRLCK;
                    lck.l_whence = SEEK_SET;
                    lck.l_pid = getpid();
                    printf("before entering critical...\n");
                    fcntl(fd1,F_SETLKW,&lck);
                    lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                    
                    write(fd1,&cli,sizeof(cli));

                    lck.l_type = F_UNLCK;
                    fcntl(fd1,F_SETLK,&lck);
                    close(fd1);
                    write(nsd,&l_ack,sizeof(l_ack));
                }

                                                // login ends
                                                // operations start 

                int op,fd2, last ;             // last :- last id
                char name[10];
                while(1){
                    read(nsd,&op,sizeof(op));
                    printf("operation is : %d\n",op);

                    if (op == 1){

                        fd1 = open("clients_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_RDLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        read(fd1,&cli,sizeof(cli));
                        write(nsd,&(cli.paisa),sizeof(cli.paisa));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                    }
                    else if (op == 2){
                        int depo ;
                        int d_ack=1 ;
                        fd1 = open("clients_db",O_RDWR);
                        read(nsd,&depo,sizeof(depo));
                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        read(fd1,&cli,sizeof(cli));
                        cli.paisa = cli.paisa + depo ;
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        write(fd1,&cli,sizeof(cli));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        write(nsd,&d_ack,sizeof(d_ack));
                    }
                    else if (op == 3){
                        int with ;
                        int w_ack=1 ;
                        fd1 = open("clients_db",O_RDWR);
                        read(nsd,&with,sizeof(with));
                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        read(fd1,&cli,sizeof(cli));
                        if (cli.paisa < with){w_ack=0;write(nsd,&w_ack,sizeof(w_ack));continue;}
                        cli.paisa = cli.paisa - with ;
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        write(fd1,&cli,sizeof(cli));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        write(nsd,&w_ack,sizeof(w_ack));
                    }
                    else if (op == 4){
                        struct client cli2 ;
                        int with ,t_id,s_rec;
                        int t_ack=1 ,flag = 0;
                        fd1 = open("clients_db",O_RDWR);
                        read(nsd,&t_id,sizeof(t_id));
                        read(nsd,&with,sizeof(with));
                        s_rec = t_id %100 ;
                        struct flock lck ;                // decreasing money
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli);
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        printf("Inside critical to decrease money\n");
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        read(fd1,&cli,sizeof(cli));
                        if (cli.paisa < with){t_ack=0;write(nsd,&t_ack,sizeof(t_ack));continue;}
                        cli.paisa = cli.paisa - with ;
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        write(fd1,&cli,sizeof(cli));

                        lck.l_type = F_UNLCK;
                        printf("press enter to exit critical...\n");
                        getchar();
                        fcntl(fd1,F_SETLK,&lck);
                                                            // receiving money
                        lck.l_start = (s_rec-1)*sizeof(cli2);    
                        lck.l_len = sizeof(cli2);
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        printf("Inside critical to receive money...\n");
                        lseek(fd1,(s_rec-1)*sizeof(cli2),SEEK_SET);
                        read(fd1,&cli2,sizeof(cli2));
                        if (cli2.count < 0){t_ack=2;write(nsd,&t_ack,sizeof(t_ack));flag = 1;}
                        else {
                            cli2.paisa = cli2.paisa+ with ;
                            lseek(fd1,(s_rec-1)*sizeof(cli2),SEEK_SET);
                            write(fd1,&cli2,sizeof(cli2));
                        }

                        lck.l_type = F_UNLCK;
                        printf("press enter to exit critical...\n");
                        getchar();
                        fcntl(fd1,F_SETLK,&lck);

                        if (flag == 1){                     // refund
                            lck.l_start = (start-1)*sizeof(cli);
                            lck.l_len = sizeof(cli) ;
                            lck.l_type = F_WRLCK;
                            lck.l_whence = SEEK_SET;
                            lck.l_pid = getpid();
                            printf("before entering critical...\n");
                            fcntl(fd1,F_SETLKW,&lck);
                            printf("inside critical to refund...\n");
                            lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                            read(fd1,&cli,sizeof(cli));
                            cli.paisa = cli.paisa + with ;
                            lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                            write(fd1,&cli,sizeof(cli));

                            lck.l_type = F_UNLCK;
                            printf("enter to exit critical\n");
                            getchar();
                            getchar();
                            fcntl(fd1,F_SETLK,&lck);
                        }

                        close(fd1);


                        write(nsd,&t_ack,sizeof(t_ack));
                    }
                    else if (op == 5){
                        int l_am ,fd3;
                        int last_l ;
                        struct loan ln ,r_ln;
                        read(nsd,&l_am,sizeof(l_am));
                        ln.amount = l_am ;
                        ln.cust_id = cli.cust_id ;
                        ln.emp_id = 0 ;
                        ln.count = 0;

                        fd3 = open("loan_db", O_APPEND | O_RDWR);
                        lseek(fd3,(-1)*sizeof(ln),SEEK_END);
                        read(fd3,&r_ln,sizeof(r_ln));
                        ln.l_id = r_ln.l_id + 1;
                        close(fd3);
                        fd3 = open("loan_db", O_APPEND | O_RDWR);
                        write(fd3,&ln,sizeof(ln));
                        close(fd3);
                    }
                    else if (op == 6){

                        int ack = 1 ;
                        fd1 = open("clients_db",O_RDWR);
                        int new_pass ;
                        struct flock lck ;
                        read(nsd,&new_pass,sizeof(int));
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        
                        cli.password = new_pass;
                        write(fd1,&cli,sizeof(cli));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        write(nsd,&ack,sizeof(ack));

                    }
                    else if (op == 7){
                        struct feedback f1;
                        char feed[50];
                        int fd2 ;
                        fd2 = open("feedback_db",O_RDWR | O_APPEND);
                        f1.f_id = cli.cust_id ;
                        read(nsd,feed,sizeof(feed));
                        strcpy(f1.feedback,feed);
                        // printf("feedback : %s\n",f1.feedback);
                        // printf("f_id : %d\n",f1.f_id);
                        write(fd2,&f1,sizeof(f1));
                        close(fd2);

                    }
                    else if (op == 8){

                    }
                    else if(op == 9) {
                        fd1 = open("clients_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        cli.count = 0 ;
                        write(fd1,&cli,sizeof(cli));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        break;
                    }
                    else{
                        fd1 = open("clients_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(cli);
                        lck.l_len = sizeof(cli) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(cli),SEEK_SET);
                        cli.count = 0 ;
                        write(fd1,&cli,sizeof(cli));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        exit(0);
                    }

                }
                

                
            }

            else if (num == 2){     // employee
                read(nsd,&id,sizeof(id));
                read(nsd,&pas,sizeof(pas));

                                        // login starts
                struct emp em ;
                int start = id%100;
                fd1 = open("emp_db",O_RDONLY);

                struct flock lck ;
                lck.l_start = (start-1)*sizeof(em);
                lck.l_len = sizeof(em) ;
                lck.l_type = F_RDLCK;
                lck.l_whence = SEEK_SET;
                lck.l_pid = getpid();
                printf("before entering critical...\n");
                fcntl(fd1,F_SETLKW,&lck);
                lseek(fd1,(start-1)*sizeof(em),SEEK_SET);
                
                read(fd1,&em,sizeof(em));

                // printf("enter key : \n");
                // getchar();
                // getchar();

                lck.l_type = F_UNLCK;
                fcntl(fd1,F_SETLK,&lck);
                close(fd1);
                if (em.count == 1){
                    status = 2 ;
                    // write(nsd,&status,sizeof(int));
                }
                else if (id == em.emp_id){
                    if (em.password == pas){
                        status = 1 ;
                        // write(nsd,&status,sizeof(int));
                    }
                    else {
                        status = 0 ;
                        // write(nsd,&status,sizeof(int));
                    }
                }
                else {
                    
                    status = 0 ;
                    
                    // write(nsd,&status,sizeof(int));
                }
                if (em.count == -1){
                    status = 0 ;
                }

                write(nsd,&status,sizeof(int));

                if (status == 0 || status == 2){write(nsd,&l_ack,sizeof(l_ack));continue;}
                else {
                    em.count = 1 ;
                    fd1 = open("emp_db",O_RDWR);

                    struct flock lck ;
                    lck.l_start = (start-1)*sizeof(em);
                    lck.l_len = sizeof(em) ;
                    lck.l_type = F_WRLCK;
                    lck.l_whence = SEEK_SET;
                    lck.l_pid = getpid();
                    printf("before entering critical...\n");
                    fcntl(fd1,F_SETLKW,&lck);
                    lseek(fd1,(start-1)*sizeof(em),SEEK_SET);
                    
                    write(fd1,&em,sizeof(em));

                    lck.l_type = F_UNLCK;
                    fcntl(fd1,F_SETLK,&lck);
                    close(fd1);
                    write(nsd,&l_ack,sizeof(l_ack));
                }

                                                // login ends
                // operations start 

                int op,fd2, last ;             // last :- last id
                char name[10];
                while(1){
                    read(nsd,&op,sizeof(op));
                    printf("operation is : %d\n",op);
                    if (op == 1){
                        struct client cli;
                        char c_name[10];
                        int start1 ;
                        fd2 = open("clients_db",O_RDWR);

                        lseek(fd2,-1*sizeof(cli),SEEK_END);

                        read(fd2,&cli,sizeof(cli));
                        last = cli.cust_id ;
                        printf("last is : %d\n",last);

                        cli.cust_id = last + 1 ;
                        start1 = (last + 1)%100;
                        read(nsd,c_name,sizeof(c_name));
                        perror("");
                        read(nsd,&(cli.password),sizeof(cli.password));
                        strcpy(cli.cust_name , c_name) ;
                        cli.count = 0 ;
                        cli.paisa = 0 ;
                        cli.acc_status = false ;
                        // printf("name : %s\n",cli.cust_name);
                        // printf("id : %d\n",cli.cust_id);
                        // printf("pas : %d\n",cli.password);
                        // printf("balance : %d\n",cli.paisa);


                        lseek(fd2,0,SEEK_SET);
                        lseek(fd2,(start1-1)*sizeof(cli),SEEK_SET);
                        write(fd2,&cli,sizeof(cli));
                        write(nsd,&(cli.cust_id),sizeof(cli.cust_id));
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
                        write(nsd,&(cli.cust_id),sizeof(cli.cust_id));

                    }
                    else if(op == 3){
                        

                    }
                    else if (op == 4){

                    }
                    else if (op == 5){
                        int ack = 1 ;
                        fd1 = open("emp_db",O_RDWR);
                        int new_pass ;
                        struct flock lck ;
                        read(nsd,&new_pass,sizeof(int));
                        lck.l_start = (start-1)*sizeof(em);
                        lck.l_len = sizeof(em) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(em),SEEK_SET);
                        
                        em.password = new_pass;
                        write(fd1,&em,sizeof(em));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        write(nsd,&ack,sizeof(ack));
                    }
                    else if(op == 6) {
                        fd1 = open("emp_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(em);
                        lck.l_len = sizeof(em) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(em),SEEK_SET);
                        em.count = 0 ;
                        write(fd1,&em,sizeof(em));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        break;
                    }
                    else{
                        fd1 = open("emp_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(em);
                        lck.l_len = sizeof(em) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(em),SEEK_SET);
                        em.count = 0 ;
                        write(fd1,&em,sizeof(em));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        exit(0);
                    }
                }
                
            }

            else if (num == 3){      // manager
                read(nsd,&id,sizeof(id));
                read(nsd,&pas,sizeof(pas));

                                        // login starts
                struct manager mg ;
                int start = id%100;
                fd1 = open("manager_db",O_RDONLY);

                struct flock lck ;
                lck.l_start = (start-1)*sizeof(mg);
                lck.l_len = sizeof(mg) ;
                lck.l_type = F_RDLCK;
                lck.l_whence = SEEK_SET;
                lck.l_pid = getpid();
                printf("before entering critical...\n");
                fcntl(fd1,F_SETLKW,&lck);
                lseek(fd1,(start-1)*sizeof(mg),SEEK_SET);
                
                read(fd1,&mg,sizeof(mg));

                // printf("enter key : \n");
                // getchar();
                // getchar();

                lck.l_type = F_UNLCK;
                fcntl(fd1,F_SETLK,&lck);
                close(fd1);
                if (mg.count == 1){
                    status = 2 ;
                    write(nsd,&status,sizeof(int));
                }
                else if (id == mg.mg_id){
                    if (mg.password == pas){
                        status = 1 ;
                        write(nsd,&status,sizeof(int));
                    }
                    else {
                        status = 0 ;
                        write(nsd,&status,sizeof(int));
                    }
                }
                else {
                    if (id != mg.mg_id){
                        status = 0 ;
                    }
                    write(nsd,&status,sizeof(int));
                }

                if (status == 0 || status == 2){write(nsd,&l_ack,sizeof(l_ack));continue;}
                else {
                    mg.count = 1 ;
                    fd1 = open("manager_db",O_RDWR);

                    struct flock lck ;
                    lck.l_start = (start-1)*sizeof(mg);
                    lck.l_len = sizeof(mg) ;
                    lck.l_type = F_WRLCK;
                    lck.l_whence = SEEK_SET;
                    lck.l_pid = getpid();
                    printf("before entering critical...\n");
                    fcntl(fd1,F_SETLKW,&lck);
                    lseek(fd1,(start-1)*sizeof(mg),SEEK_SET);
                    
                    write(fd1,&mg,sizeof(mg));

                    lck.l_type = F_UNLCK;
                    fcntl(fd1,F_SETLK,&lck);
                    close(fd1);
                    write(nsd,&l_ack,sizeof(l_ack));
                }

                                                // login ends
                // operations start 

                int op,fd2, last ;             // last :- last id
                char name[10];
                while(1){
                    read(nsd,&op,sizeof(op));
                    printf("operation is : %d\n",op);
                    if (op == 1){
                        int cust_id,start31,ack = 1;
                        struct client cli ;
                        fd2 = open("clients_db",O_RDWR);
                        read(nsd,&cust_id,sizeof(cust_id));
                        start31 = cust_id % 100;
                        struct flock lck ;
                        lck.l_start = (start31-1)*sizeof(cli);
                        lck.l_len = sizeof(cli);
                        lck.l_type = F_WRLCK ;
                        lck.l_whence = SEEK_SET ;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd2,F_SETLKW,&lck);

                        lseek(fd2,(start31-1)*sizeof(cli),SEEK_SET);
                        read(fd2,&cli,sizeof(cli));
                        if (cli.count == -1)
                        {
                            cli.count = 0 ;
                        }
                        else if(cli.count == 0){
                            cli.count = -1;
                        }
                        else {
                            ack = 0 ;
                        }
                        if (ack){
                            lseek(fd2,(start31-1)*sizeof(cli),SEEK_SET);
                            write(fd2,&cli,sizeof(cli));
                        }

                        lck.l_type = F_UNLCK;
                        fcntl(fd2,F_SETLK,&lck);
                        write(nsd,&ack,sizeof(ack));
                        close(fd2);
                    }
                    else if (op == 2){
                        int al_id , a_id,start32 ;
                        struct loan ln1, ln2 ;
                        read(nsd,&a_id,sizeof(al_id));
                        read(nsd,&a_id,sizeof(a_id));

                        fd1 = open("loan_db",O_RDWR);
                        start32 = al_id %100 ;
                        
                        struct flock lck ;
                        
                        lck.l_start = (start32-1)*sizeof(ln1);
                        lck.l_len = sizeof(ln1) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start32-1)*sizeof(ln1),SEEK_SET);
                        read(fd1,&ln1,sizeof(ln1));
                        
                        ln1.emp_id = a_id ;
                        lseek(fd1,(start32-1)*sizeof(ln1),SEEK_SET);
                        write(fd1,&ln1,sizeof(ln1));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);

                    }
                    else if(op == 3){
                        int f_id,f_last ;
                        char feedback[50];
                        struct feedback f1,f2;
                        fd1 = open("feedback_db",O_RDWR);
                        // read(nsd,&f_id,sizeof(f_id));

                        lseek(fd1,-1*sizeof(f2),SEEK_END);

                        read(fd1,&f2,sizeof(f2));
                        f_last = f2.f_id ;
                        write(nsd,&f_last,sizeof(f_last));
                        lseek(fd1,0,SEEK_SET);

                        while(1){
                            read(fd1,&f2,sizeof(f2));
                            write(nsd,&(f2.f_id),sizeof(f2.f_id));
                            strcpy(feedback,f2.feedback);
                            write(nsd,feedback,sizeof(feedback));
                            // printf("client id : %d\n",f2.f_id);
                            // printf("feed back : %s\n",f2.feedback);
                            if (f2.f_id == f_last){
                                break;
                            }
                            }

                        close(fd1);

                    }
                    else if(op == 4){
                        int ack = 1 ;
                        fd1 = open("manager_db",O_RDWR);
                        int new_pass ;
                        struct flock lck ;
                        read(nsd,&new_pass,sizeof(int));
                        lck.l_start = (start-1)*sizeof(mg);
                        lck.l_len = sizeof(mg) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(mg),SEEK_SET);
                        
                        mg.password = new_pass;
                        write(fd1,&mg,sizeof(mg));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        write(nsd,&ack,sizeof(ack));

                    }
                    else if(op == 5) {
                        fd1 = open("manager_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(mg);
                        lck.l_len = sizeof(mg) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(mg),SEEK_SET);
                        mg.count = 0 ;
                        write(fd1,&mg,sizeof(mg));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        break;
                    }
                    else {
                        fd1 = open("manager_db",O_RDWR);

                        struct flock lck ;
                        lck.l_start = (start-1)*sizeof(mg);
                        lck.l_len = sizeof(mg) ;
                        lck.l_type = F_WRLCK;
                        lck.l_whence = SEEK_SET;
                        lck.l_pid = getpid();
                        printf("before entering critical...\n");
                        fcntl(fd1,F_SETLKW,&lck);
                        lseek(fd1,(start-1)*sizeof(mg),SEEK_SET);
                        mg.count = 0 ;
                        write(fd1,&mg,sizeof(mg));

                        lck.l_type = F_UNLCK;
                        fcntl(fd1,F_SETLK,&lck);
                        close(fd1);
                        exit(0);
                    }


                    // exit(0);        // remove later
                }


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
                    write(nsd,&status,sizeof(int));
                }
                else if (id == ad.admin_id){
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
                    
                    status = 0 ;
                    
                    write(nsd,&status,sizeof(int));
                }

                if (status == 0 || status == 2){write(nsd,&l_ack,sizeof(l_ack));continue;}
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
                    write(nsd,&l_ack,sizeof(l_ack));
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
                        if(em.count == 1){check = 2;write(nsd,&check,sizeof(check));continue;}
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
                        int new_pass ,ack;
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
                        write(nsd,&ack,sizeof(ack));


                    }
                    else if(op == 6) {
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
                        break;
                    }
                    else{
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
            
            }
        }
        else{
        sleep(1);
        close(nsd);
        }
    }
}