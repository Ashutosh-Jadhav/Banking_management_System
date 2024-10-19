#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{


    int pas,id,sd, c ,status ;         // c : - choice
    struct sockaddr_in serv ;
    
    sd = socket(AF_UNIX,SOCK_STREAM,0);
    perror("");
    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY ;
    serv.sin_port = htons(6012);
    connect(sd,(void*)&serv,sizeof(serv));
    perror("");
    while(1){
        printf("1 : customer  2 : employee  3 : manager  4 : Admin  5 : exit\n");
        printf("login as : ");
        scanf("%d",&c);
        write(sd, &c,sizeof(c));
        perror("Write : ");
        if (c == 5) {exit(0);}
        if (c>0 && c < 5){
            break;
        }
        printf("wrong choice\n");
    }
    printf("login ID : ");
    scanf("%d",&id);
    write(sd, &id,sizeof(id));
    printf("password : ");
    scanf("%d",&pas);
    printf("password is : %d\n",pas);
    write(sd, &pas,sizeof(pas));
    perror("write : ");

    if (c == 1){               // customer

    }

    else if (c == 2){          // employee

    }
    
    else if (c == 3){          // manager

    }

    else {                    // Admin
        
        read(sd,&status,sizeof(int));
        getchar();
        getchar();
        if (status == 0){
            printf("wrong credentials\n");
            shutdown(sd,0);
            exit(0);
        }
        else if(status == 1){
            printf("Logged in\n");
        }
        else {
            printf("Already one session present... \n");
            exit(0);
        }

        while(1){
            printf("1 : Add new bank employee \n2 : Modify customer details\n3 : modify employee details\n4 : manage user Roles\n5 : change password \n6 : logout\n7 : exit\n");
            printf("Enter operation : ");
            scanf("%d",&c);
            write(sd,&c,sizeof(c));
            printf("choice is : %d\n",c);
            if (c == 1){
                char name[10];
                int pas ;
                printf("name : ");
                scanf("%s",name);
                printf("u wrote %s  \n",name);
                write(sd,name,sizeof(name));
                printf("password : ");
                scanf("%d",&pas);
                write(sd,&pas,sizeof(pas));

            }
            else if (c == 2){
                int id ,balance;
                int c1;
                char name[10];
                printf("Enter acc no. of customer : ");
                scanf("%d",&id);
                write(sd,&id,sizeof(id));
                printf("waiting...\n");
                read(sd,&balance,sizeof(balance));
                read(sd,name,sizeof(name));
                printf("current balance is : %d\n",balance);
                printf("current name is : %s\n",name);
                printf("1 : change name, 2 : change balance\n");
                scanf("%d",&c1);
                printf("choice is : %d\n",c1);
                write(sd,&c1,sizeof(c1));
                if (c1 == 1){
                    printf("new name : ");
                    scanf("%s",name);
                    write(sd,name,sizeof(name));
                    printf("waiting...\n");
                    read(sd,name,sizeof(name));
                    printf("done\n");
                    printf("new name is %s\n",name);
                }
                else if(c1 == 2){
                    printf("new balance : ");
                    scanf("%d",&balance);
                    write(sd,&balance,sizeof(balance));
                    printf("waiting...\n");
                    read(sd,&balance,sizeof(balance));
                    printf("done\n");
                    printf("new balance is : %d\n",balance);
                }
                else {printf("wrong choice you cant be Adminstrater...\n");}
            }
            else if (c == 3){
                int id ,balance,check;
                char name[10];
                printf("Enter ID of Employee : ");
                scanf("%d",&id);
                write(sd,&id,sizeof(id));

                read(sd,&check,sizeof(check));
                if (check == 0){
                    printf("No Employee with entered ID\n");
                    continue;
                }
                read(sd,name,sizeof(name));
            
                printf("current name is : %s\n",name);
                printf("new name : ");
                scanf("%s",name);
                write(sd,name,sizeof(name));
                printf("waiting...\n");
                read(sd,name,sizeof(name));
                printf("done\n");
                printf("new name is %s\n",name);

            }
            else if (c == 4){
                int emp_id,check;
                printf("Enter employee ID : ");
                scanf("%d",&emp_id);
                write(sd,&emp_id,sizeof(emp_id));

                read(sd,&check,sizeof(check));
                if (check == 0){
                    printf("No Employee with entered ID\n");
                    continue;
                }
                read(sd,&emp_id,sizeof(emp_id));
                printf("done...\n");
                printf("new id of manager is : %d\n",emp_id);


            }
            else if(c == 5){
                int new_pas;
                printf("New password : ");
                scanf("%d",&new_pas);
                write(sd,&new_pas,sizeof(new_pas));
            }
            else if(c == 6){
                printf("Logged out...\n");exit(0);
            }
            else {
                exit(0);
            }
        }
    }

        exit(0);
    

}