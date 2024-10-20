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

    int fd1,last;
    struct loan m1 ,m2;
    close(fd1);
    fd1 = open("loan_db",O_RDONLY);

    lseek(fd1,-1*sizeof(m2),SEEK_END);

    read(fd1,&m2,sizeof(m2));
    last = m2.l_id ;
    lseek(fd1,0,SEEK_SET);

    while(1){
    read(fd1,&m2,sizeof(m2));
    printf("Loan ID : %d\n",m2.l_id);
    printf("Account number : %d\n",m2.cust_id);
    printf("Employee Assigned to : %d\n",m2.emp_id);
    printf("Amount : %d\n",m2.amount);
    printf("status : %d\n",m2.count);
    if (m2.l_id == last){
        break;
    }
    }
}