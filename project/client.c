#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{


    int pas,id,sd, c ,status,l_ack = 1 ;         // c : - choice
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
        if (c<=0 || c >= 5){
            printf("wrong choice\n");
            continue;
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
            int l_ack ;
            read(sd,&status,sizeof(int));
            getchar();
            getchar();
            printf("waiting...\n");
            read(sd,&l_ack,sizeof(int));
            if (status == 0){
                printf("wrong credentials\n");
                continue ;
                // shutdown(sd,0);
                // exit(0);
            }
            else if(status == 1){
                printf("Logged in\n");
            }
            else {
                printf("Already one session present... \n");
                continue ;
                // exit(0);
            }

            while(1){
                printf("1 : View Account Balance\n2 : Deposit Money\n3 : Withdraw Money\n4 : Transfer Funds\n5 : Apply for a Loan\n6 : Change Password\n7 : Adding Feedback\n");
                printf("8 : View Transaction History\n9 : Logout\n10 : Exit\n");
                printf("Enter operation : ");
                scanf("%d",&c);
                write(sd,&c,sizeof(c));
                printf("choice is : %d\n",c);

                if (c==1){
                    int acc_b;
                    printf("waiting...\n");
                    read(sd,&acc_b,sizeof(acc_b));
                    printf("Balance : %d\n\n\n",acc_b);
                }
                else if (c == 2){
                    int depo,d_ack ;
                    printf("Enter Amount to deposit : ");
                    scanf("%d",&depo);
                    write(sd,&depo,sizeof(depo));
                    printf("waiting...\n");
                    read(sd,&d_ack,sizeof(d_ack));
                    printf("done...\n");
                }
                else if (c == 3){
                    int with,w_ack ;
                    printf("Enter Amount to withdraw : ");
                    scanf("%d",&with);
                    write(sd,&with,sizeof(with));
                    printf("waiting...\n");
                    read(sd,&w_ack,sizeof(w_ack));
                    if (w_ack == 1){
                        printf("done...\n");}
                    else {
                        printf("Insufficient Balance\n");
                    }
                }
                else if(c == 4){
                    int t_id,t_am,t_ack ;
                    printf("Enter acc no : ");
                    scanf("%d",&t_id);
                    printf("Enter Amount : ");
                    scanf("%d",&t_am);
                    write(sd,&t_id,sizeof(t_id));
                    write(sd,&t_am,sizeof(t_am));
                    read(sd,&t_ack,sizeof(t_ack));
                    if (t_ack == 0){
                        printf("insufficient Balance\n");
                    }
                    else if(t_ack == 2){
                        printf("inactive Account holder\n");
                    }
                    else {
                        printf("done...\n");
                    }

                }
                else if(c==5){
                    int l_am ;
                    printf("Enter amount of loan : ");
                    scanf("%d"&l_am);
                    write(sd,&l_am,sizeof(l_am));
                    
                }
                else if(c == 6){
                    int new_pas,ack;
                    printf("New password : ");
                    scanf("%d",&new_pas);
                    write(sd,&new_pas,sizeof(new_pas));
                    printf("waiting...\n");
                    read(sd,&ack,sizeof(ack));
                    printf("done...\n");
                }
                else if(c==7){
                    char feedback[50];
                    printf("\n");
                    printf("Enter feedback ...\n");
                    scanf("%s",feedback);
                    write(sd,feedback,sizeof(feedback));
                    printf("you wrote : %s\n",feedback);
                    
                    printf("done...\n");
                }
                else if (c==8){

                }
                else if (c == 9){
                    printf("Logged out...\n");
                    break;
                }
                else {
                    exit(0);
                }
            }

        }

        else if (c == 2){          // employee

            read(sd,&status,sizeof(int));
            getchar();
            getchar();
            printf("waiting...\n");
            read(sd,&l_ack,sizeof(l_ack));
            if (status == 0){
                printf("wrong credentials\n");
                continue ;
                // shutdown(sd,0);
                // exit(0);
            }
            else if(status == 1){
                printf("Logged in\n");
            }
            else {
                printf("Already one session present... \n");
                continue ;
                // exit(0);
            }

            while(1){
                printf("1 : Add new customer\n2 : Modify Customer Details\n3 : Approve/Reject Loans\n4 : View Assigned Loan Applications\n5 : change password \n6 : logout\n7 : exit\n");
                printf("Enter operation : ");
                scanf("%d",&c);
                write(sd,&c,sizeof(c));
                printf("choice is : %d\n",c);
                if (c == 1){
                    char name[10];
                    int pas,cust_id ;
                    printf("name : ");
                    scanf("%s",name);
                    printf("u wrote %s  \n",name);
                    write(sd,name,sizeof(name));
                    printf("password : ");
                    scanf("%d",&pas);
                    write(sd,&pas,sizeof(pas));
                    write(sd,&cust_id,sizeof(cust_id));
                    printf("done...\n");
                    printf("Account No is : %d\n",cust_id);
                }
                else if( c == 2){
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
                    else {printf("wrong choice you cant be Employee, you are fired...\n");}
                }
                else if(c == 3){

                }
                else if (c == 4){

                }
                else if (c == 5){

                    int new_pas,ack;
                    printf("New password : ");
                    scanf("%d",&new_pas);
                    write(sd,&new_pas,sizeof(new_pas));
                    printf("waiting...\n");
                    read(sd,&ack,sizeof(ack));
                    printf("done...\n");

                }
                else if(c == 6){
                    printf("Logged out...\n");break;
                }
                else {
                    exit(0);
                }
            }

        }
        
        else if (c == 3){          // manager

            read(sd,&status,sizeof(int));
            getchar();
            getchar();
            printf("waiting...\n");
            read(sd,&l_ack,sizeof(l_ack));
            if (status == 0){
                printf("wrong credentials\n");
                continue;
                // shutdown(sd,0);
                // exit(0);
            }
            else if(status == 1){
                printf("Logged in\n");
            }
            else {
                printf("Already one session present... \n");
                continue;
                // exit(0);
            }

            while(1){
                printf("1 : Activate or deactiate customer account\n2 : Assign Loan Application Processes to Employees\n3 : Review Customer Feedback\n4 : change password \n5 : logout\n6 : exit\n");
                printf("Enter operation : ");
                scanf("%d",&c);
                write(sd,&c,sizeof(c));
                printf("choice is : %d\n",c);
                if (c == 1){
                    int c1,acc_no,ack ;
                    printf("enter account number : ");
                    scanf("%d",&acc_no);
                    write(sd,&acc_no,sizeof(acc_no));
                    printf("waiting...\n");
                    read(sd,&ack,sizeof(ack));
                    if (ack == 0){
                        printf("customer is currently logged in try later\n");
                    }
                    else{
                        printf("done...\n");
                    }
                }
                else if (c == 2){
                    
                }
                else if (c == 3){
                    char feedback[50];
                    char copy_f[50];
                    int f_id,f_last;
                    // printf("Enter cust_id : ");
                    // scanf("%d",&f_id);
                    // write(sd,&f_id,sizeof(f_id));
                    read(sd,&f_last,sizeof(f_last));
                    while(1){
                        read(sd,&f_id,sizeof(f_id));
                        printf("Acc no. : %d\n",f_id);
                        read(sd,feedback,sizeof(feedback));
                        strcpy(copy_f,feedback);
                        printf("Feedback : %s\n",copy_f);
                        if (f_last == f_id){
                            break ;
                        }
                    }

                    // read(sd,feedback,sizeof(feedback));
                    // printf("%s\n",feedback);
                }
                else if (c == 4){
                    int new_pas,ack;
                    printf("New password : ");
                    scanf("%d",&new_pas);
                    write(sd,&new_pas,sizeof(new_pas));
                    printf("waiting...\n");
                    read(sd,&ack,sizeof(ack));
                    printf("done...\n");
                }
                else if(c == 5){
                    printf("Logged out...\n");break;
                }
                else {
                    exit(0);
                }
                
            }


        }

        else {                    // Admin
            
            read(sd,&status,sizeof(int));
            getchar();
            getchar();
            printf("waiting...\n");
            read(sd,&l_ack,sizeof(int));
            if (status == 0){
                printf("wrong credentials\n");
                continue;
                // shutdown(sd,0);
                // exit(0);
            }
            else if(status == 1){
                printf("Logged in\n");
            }
            else {
                printf("Already one session present... \n");
                continue;
                // exit(0);
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
                    if (check == 2){
                        printf("Employee is currently working try later...\n");
                        continue ;
                    }
                    read(sd,&emp_id,sizeof(emp_id));
                    printf("done...\n");
                    printf("new id of manager is : %d\n",emp_id);


                }
                else if(c == 5){
                    int new_pas,ack;
                    printf("New password : ");
                    scanf("%d",&new_pas);
                    write(sd,&new_pas,sizeof(new_pas));
                    printf("waiting...\n");
                    read(sd,&ack,sizeof(ack));
                    printf("done...\n");
                }
                else if(c == 6){
                    printf("Logged out...\n");break;
                }
                else {
                    exit(0);
                }
            }
        }

        // exit(0);
    
    }
}