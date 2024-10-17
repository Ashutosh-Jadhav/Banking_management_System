#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    struct admin{
        char *admin_name;
        int admin_id;
        char *password;
    };

    int fd1;
    struct admin a1 ;
    printf("Enter id : ");
    scanf("%d",&a1.admin_id);
    printf("Enter password : ");
    scanf("%s",a1.password);
    
}