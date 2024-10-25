#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>   
#include <sys/stat.h>
#include "clientFunctions.c"
#define PORT 9898
#define BUFF_SIZE 1024
#define PATH_LEN 64
#define UCSIZE 64
#define MAX_LEN 64

int main(){
    int sockfd;
    char buff[BUFF_SIZE];
    ssize_t bytes;
    socklen_t clilen;
    struct sockaddr_in seraddr;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){getError("Error opening Socket");} 


    bzero((char *)&seraddr,sizeof(seraddr));

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
    seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockfd,(struct sockaddr*)&seraddr,sizeof(seraddr))<0){
        getError("connection Failed");
    }


    int act,loginAct;
    while (1){
        system("clear");
        bzero(buff,BUFF_SIZE);
        char greetMsg[]="1. Login\n2. Exit\nEnter Chioce: ";
        printf("%s",greetMsg);
        scanf("%d",&loginAct);

        //Login Action
        write(sockfd,&loginAct,sizeof(int));
        

        //Login User
        if(loginAct==1){
            char username[UCSIZE],passwd[UCSIZE];
            int iden;
            bzero(buff,BUFF_SIZE);

            //Enter Username
            bytes = read(sockfd,buff,sizeof(buff)-1);
            buff[bytes]='\0';
            printf("%s",buff);
            scanf("%s",username);
            write(sockfd,username,strlen(username));

            //Enter Password
            bzero(buff,BUFF_SIZE);
            bytes = read(sockfd,buff,sizeof(buff)-1);
            buff[bytes]='\0';
            printf("%s",buff);
            scanf("%s",passwd);
            write(sockfd,passwd,strlen(passwd));

            //idenenticating
            bzero(buff,BUFF_SIZE);
            bytes = read(sockfd,&iden,sizeof(int));

            //Sync write
            write(sockfd,"Sync",strlen("Sync"));
            if(iden==0){
                printf("Account is Deactive\n");
            }
            if(iden<0){
                printf("No User Found.\n");
            }
            sleep(2);
            system("clear");
            
            if(iden>60){
                //Client Side Customer code
                int choice,currUserID;
                char customerMenu[]="1. Balance\n2. Deposit Money\n3. Widthraw Money\n4. Transfer Money\n5. Loan?\n6. History\n7. Feedback\n8. Password?\n9. my Loans\n10. Logout\nEnter Choice: ";
                currUserID = iden;

                printUserdetail(currUserID);
                while(1){
                    bzero(buff,BUFF_SIZE);
                    printf("This is a Menu\n");
                    printf("%s",customerMenu);
                    scanf("%d",&choice);

                    write(sockfd,&choice,sizeof(int));
                    
                    if(choice==1){
                        long int userBal;
                        read(sockfd,&userBal,sizeof(long int));
                        write(sockfd,"Sync",strlen("Sync"));
                        printf("Balance: \u20B9 %ld\n",userBal);
                    }else if(choice==2){
                        long int amount;

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes] = '\0';
                        printf("%s",buff);
                        scanf("%ld",&amount);

                        write(sockfd,&amount,sizeof(long int));

                    }else if(choice==3){
                        long int amount;
                        printf("Enter Amount: ");
                        scanf("%ld",&amount);
                        write(sockfd,&amount,sizeof(long int));
                        
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }else if(choice==4){
                        //Send money
                        int towhom,validUser;
                        long int amount;

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes] = '\0';
                        printf("%s",buff);
                        scanf("%d",&towhom);
                        write(sockfd,&towhom,sizeof(int));

                        read(sockfd,&validUser,sizeof(int));
                        //sync write
                        write(sockfd,"sync",strlen("sync"));
                        if(validUser!=1){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }
                        
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes] = '\0';
                        printf("%s",buff);
                        scanf("%ld",&amount);
                        write(sockfd,&amount,sizeof(long int));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==5){
                        //Apply for loan
                        long int amount;
                        printf("Amount: ");
                        scanf("%ld",&amount);;
                        write(sockfd,&amount,sizeof(long int));
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }else if(choice==6){
                        //View history
                        printTransHistory(currUserID);
                    }
                    else if(choice==7){
                        //Feedback
                        int stars;
                        char feedback[256];

                        //Give Rating
                        bytes = read(sockfd,buff,MAX_LEN-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        printf("Give Stars:");
                        scanf("%d",&stars);
                        write(sockfd,&stars,sizeof(int));

                        //Write Feedback
                        bytes = read(sockfd,buff,MAX_LEN-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        getchar();
                        fgets(feedback,sizeof(feedback),stdin);
                        feedback[strcspn(feedback, "\n")] = 0;
                        write(sockfd,feedback,strlen(feedback));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }else if(choice==8){
                        //Chnage Password
                        char username[MAX_LEN],currPass[MAX_LEN],newPass[MAX_LEN];
                        
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",username);
                        write(sockfd,username,strlen(username));

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",currPass);
                        write(sockfd,currPass,strlen(currPass));

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",newPass);
                        write(sockfd,newPass,strlen(newPass));

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        
                    }
                    else if(choice==9){
                        //View My loans
                        viewCustomerLoan(currUserID);
                    }else if(choice==10){
                        break;
                    }else{
                        printf("Invalid Choice\n");
                    }
                }
            }
            else if(iden>30 && iden<=60){
                //welcome employee
                int choice,currUserID;
                char employeeMenu[]="1. Add New Customer\n2. Modify Customer Details\n3. Process Loan Application\n4. Approve/Reject a Loan\n5. View assigned Loan\n6. change password\n7. Logout\n";
                
                currUserID = iden;
                printUserdetail(currUserID);
                while (1){
                    printf("Menu\n");
                    printf("%s",employeeMenu);

                    do{
                        printf("Enter Choice: ");
                        scanf("%d",&choice);
                        if(choice<1 || choice>7) printf("Invalid choice\n");
                    }while(choice<1 || choice>7);

                    write(sockfd,&choice,sizeof(int));

                    if(choice==1){
                        //Register New Customer
                        int UID;
                        char username[MAX_LEN];
                        char pwd[MAX_LEN];
                        char name[MAX_LEN];
                        char email[MAX_LEN];
                        char phone[MAX_LEN];
                        int crossverify;
                        
                        //Enter New User ID
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        do{
                            printf("%s",buff);
                            scanf("%d",&UID);
                            if(UID<=60) printf("Invalid Customer ID\n");
                        }while(UID<=60);

                        write(sockfd,&UID,sizeof(int));

                        read(sockfd,&crossverify,sizeof(int));
                        write(sockfd,"sync",strlen("sync"));
                        if(crossverify==1){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        //Enter New Username
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",username);
                        write(sockfd,username,strlen(username));
                        
                        //Enter New Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",pwd);
                        write(sockfd,pwd,strlen(pwd));
                        
                        //Enter New Employee Name
                        getchar();
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        fgets(name, MAX_LEN-1, stdin);
                        name[strcspn(name, "\n")] = 0;
                        write(sockfd,name,strlen(name));
                            

                        //Enter New User Email
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",email);
                        write(sockfd,email,strlen(email));

                        //Enter New User Phone
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",phone);
                        write(sockfd,phone,strlen(phone));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);                       
                    }else if(choice==2){
                        int uid,change,verifyUser,whichDetail;
                        char name[MAX_LEN];
                        char email[MAX_LEN];
                        char phone[MAX_LEN];

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        do{
                            printf("%s",buff);
                            scanf("%d",&uid);
                            if(uid<=60) printf("Invalid Customer ID\n");
                        }while(uid<=60);
                        write(sockfd,&uid,sizeof(int));

                        read(sockfd,&verifyUser,sizeof(int));
                        write(sockfd,"sync",strlen("sync"));
                        if(verifyUser!=1){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';

                        printf("%s",buff);
                        do{
                            printf("Enter: ");
                            scanf("%d",&change);
                            if(change<1 || change>3) printf("InvalidChoice\n");
                        }while(change<1 || change>3);
                        //Getting what to change
                        write(sockfd,&change,sizeof(int));
                        
                        bytes = read(sockfd,buff,BUFF_SIZE-1);

                        char clean;
                        while ((clean = getchar()) != '\n' && clean != EOF);
                        if(change==1){
                            printf("Enter New Name: ");
                            fgets(name, MAX_LEN-1, stdin);
                            name[strcspn(name, "\n")] = 0;
                            write(sockfd,name,strlen(name));
                        }else if(change==2){
                            printf("Enter New Email: ");
                            fgets(email, MAX_LEN-1, stdin);
                            name[strcspn(name, "\n")] = 0;
                            write(sockfd,email,strlen(email));
                        }else if(change==3){
                            printf("Enter New Phone: ");
                            fgets(phone, MAX_LEN-1, stdin);
                            phone[strcspn(phone, "\n")] = 0;
                            write(sockfd,phone,strlen(phone));
                        }
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==3){
                        //Process
                        char loanID[20];
                        
                        viewAssignedLoan(currUserID);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",loanID);
                        write(sockfd,loanID,sizeof(loanID));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==4){
                        //Accept/Reject Loan
                        char loanID[20];
                        int act;
                        
                        viewAssignedLoan(currUserID);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",loanID);
                        write(sockfd,loanID,sizeof(loanID));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);

                        do{
                            printf("Enter choice: ");
                            scanf("%d",&act);
                            if(act<1 || act>2) printf("Invalid Input\n");
                        }while(act<1 || act>2);
                        write(sockfd,&act,sizeof(int));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==5){
                        _Bool permission;
                        read(sockfd,&permission,sizeof(_Bool));
                        if(permission){
                            viewAssignedLoan(currUserID);
                        }
                    }
                    else if(choice==6){
                        char username[MAX_LEN],currPwd[MAX_LEN],newPwd[MAX_LEN];
                        int userExistStatus,updateStatus;

                        //Enter Username
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",username);
                        write(sockfd,username,strlen(username));

                        //Enter Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",currPwd);
                        write(sockfd,currPwd,strlen(currPwd));

                        //Verify account
                        read(sockfd,&userExistStatus,sizeof(int));
                        //sync code
                        write(sockfd,"sync",strlen("sync"));
                        if(userExistStatus<=0 || userExistStatus!=currUserID){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        //Enter new Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",newPwd);
                        write(sockfd,newPwd,strlen(newPwd));

                        //password change status
                        read(sockfd,&updateStatus,sizeof(int));
                        //sync code
                        write(sockfd,"sync",strlen("sync"));
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==7){break;}
                }
                
            }
            else if(iden>10 && iden<=30){
                //Welcome manager
                int choice,currUserID;
                char managerMenu[]="\n1. Activate Customer Account\n2. Deactivate Customer Account\n3. Assign Loan Application to Employee\n4. View all Loan Application Recieved\n5. Change Password\n6. Review Feedback\n7. Logout\n";

                currUserID = iden;
                printUserdetail(currUserID);
                while(1){
                    printf("Menu\n");
                    printf("%s",managerMenu);
                    
                    do{
                        printf("Enter Choice: ");
                        scanf("%d",&choice);
                        if(choice<1 || choice>7) printf("Invalid choice\n");
                    }while(choice<1 || choice>7);

                    write(sockfd,&choice,sizeof(int));

                    if(choice==1){
                        int custID;
                        bytes = read(sockfd,buff,BUFF_SIZE-1);;
                        buff[bytes]='\0';
                        
                        do{
                            printf("%s",buff);
                            scanf("%d",&custID);
                            if(custID<=60)printf("Invalid Customer ID\n");
                        }while(custID<=60);
                        write(sockfd,&custID,sizeof(int));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }else if(choice==2){
                        int custID;
                        bytes = read(sockfd,buff,BUFF_SIZE-1);;
                        buff[bytes]='\0';

                        do{
                            printf("%s",buff);
                            scanf("%d",&custID);
                            if(custID<=60)printf("Invalid Customer ID\n");
                        }while(custID<=60);
                        write(sockfd,&custID,sizeof(int));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }else if(choice==3){
                        //Assigning loan to employee
                        int employeeID;
                        char loanID[16];
                        
                        viewAssignedLoan(currUserID);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes] = '\0';
                        printf("%s",buff);
                        scanf("%s",loanID);
                        write(sockfd,loanID,sizeof(loanID));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        do {
                            printf("%s",buff);
                            scanf("%d",&employeeID);
                            if(employeeID<=30 || employeeID>60) printf("Invalid EmployeeID\n");
                        }while(employeeID<=30 || employeeID>60);
                        write(sockfd,&employeeID,sizeof(int));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }else if(choice==4){
                        _Bool permission;
                        read(sockfd,&permission,sizeof(_Bool));
                        if(permission){
                            viewAssignedLoan(currUserID);
                        }
                    }else if(choice==5){
                        //Updating password
                        char username[MAX_LEN],currPwd[MAX_LEN],newPwd[MAX_LEN];
                        int userExistStatus,updateStatus;

                        //Enter Username
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",username);
                        write(sockfd,username,strlen(username));

                        //Enter Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",currPwd);
                        write(sockfd,currPwd,strlen(currPwd));

                        //Verify account
                        read(sockfd,&userExistStatus,sizeof(int));
                        //sync code
                        write(sockfd,"sync",strlen("sync"));
                        if(userExistStatus<=0){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        //Enter new Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",newPwd);
                        write(sockfd,newPwd,strlen(newPwd));

                        //password change status
                        read(sockfd,&updateStatus,sizeof(int));
                        //sync code
                        write(sockfd,"sync",strlen("sync"));
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==6){
                        //review Feedback
                        _Bool permission;
                        read(sockfd,&permission,sizeof(_Bool));
                        if(permission){
                            viewAllFeedback();
                        }else printf("You don't have permission\n");
                    }
                    else if(choice==7){break;}
                }
            }
            else if(iden>=1 && iden<=10){
                //Client side Administrator Code
                int choice,currUserID;
                char adminMenu[]="\n1. Modify Employee Details\n2. Modify Customer Details\n3. Register New Employee\n4. Manage User Roles \n5. change password\n6. Logout\nEnter Choice: ";
                
                currUserID = iden;
                
                printUserdetail(currUserID);
                while(1){
                    printf("%s",adminMenu);
                    scanf("%d",&choice);

                    write(sockfd,&choice,sizeof(int));

                    if(choice==1 || choice ==2){
                        //ID
                        int uid,change;
                        int whichDetail;
                        char name[MAX_LEN];
                        char email[MAX_LEN];
                        char phone[MAX_LEN];

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        
                        if(choice==1){
                        do{
                            printf("%s",buff);
                            scanf("%d",&uid);
                            if(!(uid>30 && uid<=60)) printf("Invalid Employee ID\n");
                        }while(!(uid>30 && uid<=60));
                        }
                        else{
                        do{
                            printf("%s",buff);
                            scanf("%d",&uid);
                            if(uid<=60) printf("Invalid Customer ID\n");
                        }while(uid<=60);
                        }

                        write(sockfd,&uid,sizeof(int));

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';

                        printf("%s",buff);
                        do{
                            printf("Enter: ");
                            scanf("%d",&change);
                            if(change<1 || change>3) printf("InvalidChoice\n");
                        }while(change<1 || change>3);
                        //Geeting what to change
                        write(sockfd,&change,sizeof(int));
                        
                        bytes = read(sockfd,buff,BUFF_SIZE-1);

                        char clean;
                        while ((clean = getchar()) != '\n' && clean != EOF);
                        if(change==1){
                            printf("Enter New Name: ");
                            fgets(name, MAX_LEN-1, stdin);
                            name[strcspn(name, "\n")] = 0;
                            write(sockfd,name,strlen(name));
                            
                            bzero(buff,BUFF_SIZE);
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';

                            printf("%s",buff);
                        }else if(change==2){
                            printf("Enter New Email: ");
                            fgets(email, MAX_LEN-1, stdin);
                            name[strcspn(name, "\n")] = 0;
                            write(sockfd,email,strlen(email));
                            
                            bzero(buff,BUFF_SIZE);
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';

                            printf("%s",buff);
                        }else if(change==3){
                            printf("Enter New Phone: ");
                            fgets(phone, MAX_LEN-1, stdin);
                            phone[strcspn(phone, "\n")] = 0;
                            write(sockfd,phone,strlen(phone));
                            
                            bzero(buff,BUFF_SIZE);
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';

                            printf("%s",buff);
                        }
                    }
                    else if(choice==3){
                        //Register New Employee
                        int UID;
                        char username[MAX_LEN];
                        char pwd[MAX_LEN];
                        char name[MAX_LEN];
                        char email[MAX_LEN];
                        char phone[MAX_LEN];
                        int crossverify;
                        

                        //Enter New User ID
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        do{
                            printf("%s",buff);
                            scanf("%d",&UID);
                            if(UID<=30 || UID>60) printf("Invalid Employee ID\n");
                        }while(UID<=30 || UID>60);

                        write(sockfd,&UID,sizeof(int));

                        read(sockfd,&crossverify,sizeof(int));
                        write(sockfd,"sync",strlen("sync"));
                        if(crossverify==1){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        //Enter New Username
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",username);
                        write(sockfd,username,strlen(username));
                        
                        //Enter New Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",pwd);
                        write(sockfd,pwd,strlen(pwd));
                        
                        //Enter New Employee Name
                        getchar();
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        fgets(name, MAX_LEN-1, stdin);
                        name[strcspn(name, "\n")] = 0;
                        write(sockfd,name,strlen(name));
                            

                        //Enter New User Email
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",email);
                        write(sockfd,email,strlen(email));

                        //Enter New User Phone
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",phone);
                        write(sockfd,phone,strlen(phone));

                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);                       
                    }
                    else if(choice==4){
                        //Manage User Role
                        int oldID,newID,valid,roleChangeStatus;

                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        do{
                            printf("Enter UserID: ");
                            scanf("%d",&oldID);
                            if(oldID<=10) printf("Invalid Input\n");
                        }while(oldID<=10);
                        
                        write(sockfd,&oldID,sizeof(int));
                        bytes = read(sockfd,&valid,sizeof(int));
                        //sync write
                        write(sockfd,"sync",strlen("sync"));
                        if(valid==0){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        //User Exist
                        int role;
                        if(oldID>60){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);

                            do{
                                printf("Enter Role: ");
                                scanf("%d",&role);
                                if(role<1 || role>2) printf("Invalid selection\n");
                            }while(role<1 || role>2);
                            
                            if(role==1){
                                do{
                                    printf("Enter new ManagerID: ");
                                    scanf("%d",&newID);
                                    if(newID<=10 || newID>30) printf("Invalid ID\n");
                                }while(newID<=10 || newID>30);

                            }else if(role==2){
                                do{
                                    printf("Enter new EmployeeID: ");
                                    scanf("%d",&newID);
                                    if(newID<=30 || newID>60) printf("Invalid ID\n");
                                }while(newID<=30 || newID>60);
                            }
                            write(sockfd,&newID,sizeof(int));
                        }
                        else if(oldID>30 && oldID<=60){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);

                            do{
                                printf("Enter Role: ");
                                scanf("%d",&role);
                                if(role<1 || role>2) printf("Invalid selection\n");
                            }while(role<1 || role>2);
                            
                            if(role==1){
                                do{
                                    printf("Enter new ManagerID: ");
                                    scanf("%d",&newID);
                                    if(newID<=10 || newID>30) printf("Invalid ID\n");
                                }while(newID<=10 || newID>30);

                            }else if(role==2){
                                do{
                                    printf("Enter new Customer ID: ");
                                    scanf("%d",&newID);
                                    if(newID<=60) printf("Invalid ID\n");
                                }while(newID<=60);
                            }
                            write(sockfd,&newID,sizeof(int));
                        }else if(oldID>10 && oldID<=30){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);

                            do{
                                printf("Enter Role: ");
                                scanf("%d",&role);
                                if(role<1 || role>2) printf("Invalid selection\n");
                            }while(role<1 || role>2);
                            
                            if(role==2){
                                do{
                                    printf("Enter new EmployeeID: ");
                                    scanf("%d",&newID);
                                    if(newID<=30 || newID>60) printf("Invalid ID\n");
                                }while(newID<=30 || newID>60);

                            }else if(role==1){
                                do{
                                    printf("Enter new Customer ID: ");
                                    scanf("%d",&newID);
                                    if(newID<=60) printf("Invalid ID\n");
                                }while(newID<=60);
                            }
                            write(sockfd,&newID,sizeof(int));
                        }

                        bytes = read(sockfd,&valid,sizeof(int));
                        //sync write
                        write(sockfd,"sync",strlen("sync"));
                        if(valid==1){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        read(sockfd,&roleChangeStatus,sizeof(int));
                        //sync write
                        write(sockfd,"sync",strlen("sync"));
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==5){
                        //Change Password
                        char username[MAX_LEN],currPwd[MAX_LEN],newPwd[MAX_LEN];
                        int userExistStatus,updateStatus;

                        //Enter Username
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",username);
                        write(sockfd,username,strlen(username));

                        //Enter Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",currPwd);
                        write(sockfd,currPwd,strlen(currPwd));

                        //Verify account
                        read(sockfd,&userExistStatus,sizeof(int));
                        //sync code
                        write(sockfd,"sync",strlen("sync"));
                        if(userExistStatus<=0){
                            bytes = read(sockfd,buff,BUFF_SIZE-1);
                            buff[bytes]='\0';
                            printf("%s",buff);
                            continue;
                        }

                        //Enter new Password
                        bzero(buff,BUFF_SIZE);
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                        scanf("%s",newPwd);
                        write(sockfd,newPwd,strlen(newPwd));

                        //password change status
                        read(sockfd,&updateStatus,sizeof(int));
                        //sync code
                        write(sockfd,"sync",strlen("sync"));
                        bytes = read(sockfd,buff,BUFF_SIZE-1);
                        buff[bytes]='\0';
                        printf("%s",buff);
                    }
                    else if(choice==6){break;}
                }
            }
        }
        else if(loginAct==2){
            break;
        }
    }
    close(sockfd);
    return 0;
}