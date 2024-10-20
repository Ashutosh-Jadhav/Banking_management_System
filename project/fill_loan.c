#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    struct loan {
        int l_id ;
        int cust_id ;
        int emp_id ;
        int amount;
        int count ;
    };

    int fd1;
    struct loan m1 ,m2;
    char *s = "Hayat";
    // strcpy(m1.mg_name,s);
    m1.l_id = 2001 ;
    m1.cust_id = 101 ;
    m1.amount = 1000;
    m1.count = 0;
    m1.emp_id = 0 ;

    fd1 = open("loan_db", O_CREAT | O_RDWR ,0744);
    write(fd1,&m1,sizeof(m1));
    close(fd1);
    fd1 = open("loan_db",O_RDONLY);

    read(fd1,&m2,sizeof(m2));
    printf("Loan ID : %d\n",m2.l_id);
    printf("Account number : %d\n",m2.cust_id);
    printf("Employee Assigned to : %d\n",m2.emp_id);
    printf("Amount : %d\n",m2.amount);
    printf("status : %d\n",m2.count);
}