#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>

#define BUFF_SIZE 1024
#define MAX_LEN 64
#define MAX_LEN 64
#define PATH_LEN 64

//Server component start
struct credential {
    int UID;
    _Bool active;
    char username[MAX_LEN];
    char password[MAX_LEN];
};

struct userDetails{
    int UID;
    char name[MAX_LEN];
    char email[MAX_LEN];
    char phone[MAX_LEN];
    char designation[MAX_LEN];
};

struct acbalance{
    long int balance;
};

struct transHistory{
    int fromID;
    int toID;
    char type[MAX_LEN];
    long int balance;
};

struct feedback{
    int star;
    char fb[256];
};

struct assignedLoan {
    char loanID[MAX_LEN];
    int uid;
    char username[MAX_LEN];
    long int amount;
    char status[MAX_LEN];
};

struct loanApplication{
    char loanID[MAX_LEN];
    int applicationNo;
    long int amount;
    char status[MAX_LEN];
};
int modifyUserDetail(int userID,int whichEntry,char *newEntry){
    struct userDetails udetail,fileUser;
    off_t offset = 0;
    _Bool found = 0;
    char path[PATH_LEN];

    snprintf(path,PATH_LEN,"%d/userdetail.dat",userID);
    int fd = open(path,O_RDWR);
    if(fd<0){
        perror("Error opening User Detail file");
        return -1;
    }

    while(read(fd,&fileUser,sizeof(struct userDetails))>0){
        if(fileUser.UID==userID){
            found = 1;
            break;
        }
        offset += sizeof(struct userDetails);
    }

    if(!found){
        printf("No User Found\n");
        close(fd);
        return -1;
    }

    udetail.UID = userID;
    snprintf(udetail.name,MAX_LEN,"%s",fileUser.name);
    snprintf(udetail.email,MAX_LEN,"%s",fileUser.email);
    snprintf(udetail.phone,MAX_LEN,"%s",fileUser.phone);
    snprintf(udetail.designation,MAX_LEN,"%s",fileUser.designation);

    if(whichEntry==1){
        snprintf(udetail.name,MAX_LEN,"%s",newEntry);
    }else if(whichEntry==2){
        snprintf(udetail.email,MAX_LEN,"%s",newEntry);
    }else if(whichEntry==3){
        snprintf(udetail.phone,MAX_LEN,"%s",newEntry);
    }

    lseek(fd,offset,SEEK_SET);
    if(write(fd,&udetail,sizeof(struct userDetails))<0){
        perror("Error writing on user file");
        close(fd);
        return -1;
    }
    close(fd);
    return 1;
}

int login(char uname[MAX_LEN],char upwd[MAX_LEN]) {
    struct credential file_user;

    int fd = open("docu.dat", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }

    while (read(fd, &file_user, sizeof(struct credential)) > 0) {
        if (strcmp(uname, file_user.username) == 0 &&
            strcmp(upwd, file_user.password) == 0) {
            close(fd);
            if(file_user.active==0) return 0;
            return file_user.UID;
        }
    }

    close(fd);
    return -1;
}

int register_user(int uid,char *username,char *pwd,char* name,char* uemail,char* phone) {
    struct credential user;
    struct userDetails udetail;
    struct acbalance ubal;
    struct transHistory utrans;
    struct feedback ufeed;
    char path[PATH_LEN];

    //Input credentials
    user.UID = uid;
    user.active =1;
    snprintf(user.username,MAX_LEN,"%s",username);
    snprintf(user.password,MAX_LEN,"%s",pwd);
    
    
    udetail.UID = user.UID;
    snprintf(udetail.name,MAX_LEN,"%s",name);
    snprintf(udetail.email,MAX_LEN,"%s",uemail);
    snprintf(udetail.phone,MAX_LEN,"%s",phone);
    
    if(user.UID>60){
        snprintf(udetail.designation,MAX_LEN,"%s","Customer");
    }
    else if(user.UID>30 && user.UID<=60){
        snprintf(udetail.designation,MAX_LEN,"%s","Employee");
    }
    else if(user.UID>10 && user.UID<=30){
        snprintf(udetail.designation,MAX_LEN,"%s","Manager");
    }
    else if(user.UID>=1 && user.UID<=10){
        snprintf(udetail.designation,MAX_LEN,"%s","Administrator");
    }
    //User bank balance Intialize
    ubal.balance = 0;

    int fd = open("docu.dat", O_WRONLY | O_APPEND | O_CREAT, 0600);
    if (fd < 0) {
        perror("Error opening file");
        return 0;
    }

    if (write(fd, &user, sizeof(struct credential)) < 0) {
        perror("Error writing to file");
        close(fd);
        return 0;
    }
    close(fd);

    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d",user.UID);
    mkdir(path,0766);


    //Creating User detail File
    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/userdetail.dat",user.UID);

    fd = open(path,O_WRONLY|O_CREAT,0766);
    if(fd<0) fprintf(stderr,"Error creating <%s> files",path);
    write(fd,&udetail,sizeof(struct userDetails));
    close(fd);

    //Creating User File for account balance
    if(user.UID>60){
    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/balance.dat",user.UID);

    fd = open(path,O_WRONLY|O_CREAT,0766);
    if(fd<0) fprintf(stderr,"Error creating <%s> files",path);
    write(fd,&ubal,sizeof(struct acbalance));
    close(fd);

    //Creating User Transaction History file
    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/transactionHist.dat",user.UID);

    fd = open(path,O_WRONLY|O_CREAT,0766);
    if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
    close(fd);

    //Creating User Feedback File
    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/feedback.dat",user.UID);

    fd = open(path,O_WRONLY|O_CREAT,0766);
    if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
    close(fd);

    //Creating User Loan detail file
    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/userloandetail.dat",user.UID);

    fd = open(path,O_WRONLY|O_CREAT,0766);
    if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
    close(fd);
    }

    //Create Employee assigned loan file
    if(user.UID>30 && user.UID<=60){
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/assignedloan.dat",user.UID);

        fd = open(path,O_WRONLY|O_CREAT,0766);
        if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
        close(fd);
    }
    return 1;
}
//change password
int change_password(char *username,char *currpass,char *newpass) {
    struct credential user, file_user;
    int found = 0;

    snprintf(user.username,MAX_LEN,"%s",username);
    snprintf(user.password,MAX_LEN,"%s",currpass);

    int fd = open("docu.dat", O_RDWR);
    if (fd < 0) {
        perror("Error opening file");
        return -1;
    }

    // Search for the user and get the file offset
    off_t offset = 0;
    while (read(fd, &file_user, sizeof(struct credential)) > 0) {
        if (strcmp(user.username, file_user.username) == 0 &&
            strcmp(user.password, file_user.password) == 0) {
            found = 1;
            break;
        }
        offset += sizeof(struct credential);
    }

    if (!found) {
        printf("No User Found\n");
        close(fd);
        return 0;
    }

    //Setting new Password
    snprintf(user.password,MAX_LEN,"%s",newpass);
    user.UID=file_user.UID;
    user.active=file_user.active;

    lseek(fd, offset, SEEK_SET);

    if (write(fd, &user, sizeof(struct credential)) < 0) {
        perror("Error writing to file");
        close(fd);
        return -1;
    }
    close(fd);
    return user.UID;
}
void transactionEntry(int fromUserID,int toUserID,char *type,long int bal){
    struct transHistory tranHist;
    char path[PATH_LEN];

    tranHist.fromID = fromUserID;
    tranHist.toID = toUserID;
    snprintf(tranHist.type,MAX_LEN,"%s",type);
    tranHist.balance = bal;

    snprintf(path,PATH_LEN,"%d/transactionHist.dat",fromUserID);
    int fd = open(path,O_WRONLY|O_CREAT|O_APPEND,0766);
    if(fd<0) perror("Error opening transaction History file");
    
    if(write(fd,&tranHist,sizeof(struct transHistory))<0){
        perror("Error writing transaction history\n");
        return;
    }
    close(fd);
}

long int checkUserBalance(int userID){
    struct acbalance bal;
    char path[PATH_LEN];

    snprintf(path,PATH_LEN,"%d/balance.dat",userID);
    int fd = open(path,O_RDONLY);
    if(fd<0) perror("Error opening balance file");
    
    read(fd,&bal,sizeof(struct acbalance));
    
    close(fd);
    return bal.balance;
}

int depositFund(int fromUserID,int toUserID,long int addAmount){
    struct acbalance bal;
    char path[PATH_LEN];

    snprintf(path,PATH_LEN,"%d/balance.dat",fromUserID);
    int fd = open(path,O_RDWR);
    if(fd<0) perror("Error opening balance file");
    
    read(fd,&bal,sizeof(struct acbalance));
    bal.balance = bal.balance + addAmount;
    lseek(fd,0,SEEK_SET);

    write(fd,&bal,sizeof(struct acbalance));
    close(fd);

    transactionEntry(fromUserID,toUserID,"credit",addAmount);
    return 1;
}

int withdrawFund(int fromUserID,int touserID,long int debitAmount){
    struct acbalance bal;
    char path[PATH_LEN];

    snprintf(path,PATH_LEN,"%d/balance.dat",fromUserID);
    int fd = open(path,O_RDWR);
    if(fd<0) perror("Error opening balance file");
    
    read(fd,&bal,sizeof(struct acbalance));
    
    if(bal.balance<0 || bal.balance<debitAmount){return 0;}
    bal.balance = bal.balance - debitAmount;
    
    lseek(fd,0,SEEK_SET);
    write(fd,&bal,sizeof(struct acbalance));
    close(fd);

    transactionEntry(fromUserID,touserID,"debit",debitAmount);
    return 1;
}

int userIdExist(int userID){
    struct credential ucred;
    
    int fd = open("docu.dat",O_RDONLY);
    while (read(fd,&ucred,sizeof(struct credential))>0){
        if(ucred.UID==userID){
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}

int managerUserRoles(int oldID,int newID){
    struct userDetails currDetail;
    struct credential currCred;
    struct acbalance bal;
    char path[PATH_LEN];
    _Bool found = 0;
    off_t offset = 0;

    int fd = open("docu.dat",O_RDWR);
    if(fd<0){
        perror("Error Opening docu.dat");
        return -1;
    }

    while (read(fd,&currCred,sizeof(struct credential))>0){
        if(currCred.UID==oldID){
            found = 1;
            break;
        }
        offset+=sizeof(struct credential);
    }

    if(!found){
        printf("No User Found\n");
        close(fd);
        return -1;
    }

    currCred.UID = newID;
    lseek(fd,offset,SEEK_SET);
    if(write(fd,&currCred,sizeof(struct credential))<0){
        perror("Error updating credential");
        close(fd);
        return -1;
    }
    close(fd);

    //reading old detail
    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/userdetail.dat",oldID);
    fd = open(path,O_RDWR);
    if(fd<0){
        perror("Error Opening userdetail.dat");
        return -1;
    }
    if(read(fd,&currDetail,sizeof(struct userDetails))<0){
        perror("Error Reading User Info");
        close(fd);
        return -1;
    }
    currDetail.UID=newID;
    if(newID>60){
        snprintf(currDetail.designation,MAX_LEN,"%s","Customer");
    }
    else if(newID>30 && newID<=60){
        snprintf(currDetail.designation,MAX_LEN,"%s","Employee");
    }
    else if(newID>10 && newID<=30){
        snprintf(currDetail.designation,MAX_LEN,"%s","Manager");
    }
    else if(newID>=1 && newID<=10){
        snprintf(currDetail.designation,MAX_LEN,"%s","Administrator");
    }

    lseek(fd,0,SEEK_SET);
    if(write(fd,&currDetail,sizeof(struct userDetails))<0){
        perror("Error writing new detail");
        close(fd);
        return -1;
    }
    close(fd);
    
    if(newID>60){
        //Creating User balance sheet
        bal.balance=0;
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/balance.dat",oldID);

        fd = open(path,O_RDWR|O_CREAT,0766);
        if(fd<0) fprintf(stderr,"Error creating <%s> files",path);
        write(fd,&bal,sizeof(struct acbalance));
        close(fd);

        //Creating User Transaction History file
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/transactionHist.dat",oldID);

        fd = open(path,O_WRONLY|O_CREAT,0766);
        if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
        close(fd);

        //Creating User Feedback File
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/feedback.dat",oldID);

        fd = open(path,O_WRONLY|O_CREAT,0766);
        if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
        close(fd);

        //Creating User Loan detail file
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/userloandetail.dat",oldID);

        fd = open(path,O_WRONLY|O_CREAT,0766);
        if(fd<0) fprintf(stderr,"Error creating <%s> file",path);
        close(fd);

        //Deleting files not related to user
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/assignedloan.dat",oldID);
        unlink(path);
    }
    else if(newID>30 && newID<=60){
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/balance.dat",oldID);
        unlink(path);

        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/transactionHist.dat",oldID);
        unlink(path);

        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/feedback.dat",oldID);
        unlink(path);

        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/userloandetail.dat",oldID);
        unlink(path);
        
        //Creating new file specific for employee
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/assignedloan.dat",oldID);
        open(path,O_RDONLY|O_CREAT,0766);
    }
    else if(newID>10 && newID<=30){
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/balance.dat",oldID);
        unlink(path);

        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/transactionHist.dat",oldID);
        unlink(path);

        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/feedback.dat",oldID);
        unlink(path);

        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/userloandetail.dat",oldID);
        unlink(path);
    }

    char oldDir[MAX_LEN],newDir[MAX_LEN];
    snprintf(oldDir,MAX_LEN,"%d",oldID);
    snprintf(newDir,MAX_LEN,"%d",newID);
    rename(oldDir,newDir);
    return 1; 
}

//Loan Component
int sendLoanAppToManager(struct userDetails user,struct loanApplication loanForm){
    struct assignedLoan recievedForm;

    recievedForm.amount=loanForm.amount;
    snprintf(recievedForm.loanID,MAX_LEN,"%s",loanForm.loanID);
    snprintf(recievedForm.status,MAX_LEN,"%s",loanForm.status);

    recievedForm.uid=user.UID;
    snprintf(recievedForm.username,MAX_LEN,"%s",user.name);

    int fd = open("managerDB/loanApplication.dat",O_WRONLY|O_CREAT|O_APPEND);
    if(fd<0) {
        perror("Error opening managerDB/loanApplication.dat");
        return -1;
    }
    if(write(fd,&recievedForm,sizeof(struct assignedLoan))<0){
        perror("Error assiging loan application");
        close(fd);
        return -1;
    }
    return 1;
}
int applytoLoan(int userID,long int amount){
    struct loanApplication loanForm;
    struct userDetails user;
    char path[PATH_LEN];
    off_t offset;

    snprintf(path,PATH_LEN,"%d/userloandetail.dat",userID);
    int fd = open(path,O_RDONLY);
    if(fd<0) {
        perror("Error opening userloandetail.dat");
        return -1;
    }

    ssize_t bytes = read(fd,&loanForm,sizeof(struct loanApplication));
    if(bytes==0){
        loanForm.applicationNo = 1;
    }else if(bytes>0){
        lseek(fd, 0, SEEK_END);
        lseek(fd, -sizeof(struct loanApplication), SEEK_END);
        if(read(fd,&loanForm,sizeof(struct loanApplication))<0){
            perror("Error reading user loan detail");
            close(fd);
            return -1;
        }
        loanForm.applicationNo+=1;
    }else{
        perror("Error reading userloandetail.dat");
        close(fd);
        return -1;
    }
    close(fd);
    snprintf(loanForm.loanID,MAX_LEN,"%d%d",userID,loanForm.applicationNo);
    loanForm.amount=amount;
    snprintf(loanForm.status,MAX_LEN,"%s","pending");

    fd = open(path,O_WRONLY|O_APPEND);
    if(fd<0) {
        perror("Error opening userloandetail.dat");
        return -1;
    }
    if(write(fd,&loanForm,sizeof(struct loanApplication))<0){
        perror("Error applying to Loan");
        close(fd);
        return -1;
    }
    close(fd);

    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/userdetail.dat",userID);
    fd = open(path,O_RDONLY);
    if(fd<0) {
        perror("Error opening userdetail.dat");
        return -1;
    }
    if(read(fd,&user,sizeof(struct userDetails))<0){
        perror("Error reading user detail");
        close(fd);
        return -1;
    }
    sendLoanAppToManager(user,loanForm);
    return 1;
}
int addFeedback(int starRating, const char *feedbackMessage) {
    struct feedback fbEntry;
    int fd;

    fbEntry.star = starRating;
    snprintf(fbEntry.fb, sizeof(fbEntry.fb), "%s", feedbackMessage);

    fd = open("managerDB/feedback.dat", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening feedback.dat");
        return -1;
    }

    if (write(fd, &fbEntry, sizeof(struct feedback)) < 0) {
        perror("Error writing feedback");
        return -1;
    }
    close(fd);
    return 1;
}

int active_deactiveUser(int userID,int act){
    struct credential user;
    _Bool found = 0;
    off_t offset=0;

    
    int fd = open("docu.dat",O_RDWR);
    if(fd<0) perror("Error opening credentials file");

    while (read(fd, &user, sizeof(struct credential)) > 0) {
        if(user.UID==userID){
            found =1;
            break;
        };
        offset+=sizeof(struct credential);
    }

    if(!found){
        printf("No User Found\n");
        close(fd);
        return -1;
    }

    //User Found
    if(user.active==act){
        close(fd);
        return 0;
    }
    user.active = !(user.active);
    lseek(fd,offset,SEEK_SET);
    if(write(fd,&user,sizeof(struct credential))<0){
        perror("Couldn't Activate/deactivate");
        close(fd);
        return -2;
    }
    close(fd);
    return 1;
}
int assignLoanToEmployee(int emplyID, char *loanID){
    struct assignedLoan loanEntry;
    char path[PATH_LEN];
    _Bool found = 0;
    
    int fd1 = open("managerDB/loanApplication.dat",O_RDWR);
    ssize_t bytes = read(fd1,&loanEntry,sizeof(struct assignedLoan));
    if(bytes<=0){
        printf("No Loan Application\n");
        close(fd1);
        return 0;
    }

    lseek(fd1,0,SEEK_SET);
    while(read(fd1,&loanEntry,sizeof(struct assignedLoan))>0){
        if(strcmp(loanID,loanEntry.loanID)==0){
            close(fd1);
            found=1;
            break;
        }
    }
    if(!found){
        fprintf(stderr,"No such LoanID Exist\n");
        return -1;
    }
    //Assiging to Employee
    snprintf(path,PATH_LEN,"%d/assignedloan.dat",emplyID);
    int fd2 = open(path,O_RDWR|O_CREAT|O_APPEND,0766);
    if(fd2<0){
        perror("Failed to open Employee loan file");
        close(fd2);
        return -2;
    }

    if(write(fd2,&loanEntry,sizeof(struct assignedLoan))<0){
        perror("Failed to assign");
        close(fd2);
        return -2;
    }
    close(fd2);

    fd1 = open("managerDB/loanApplication.dat",O_RDONLY);
    fd2 = open("managerDB/temp.dat",O_WRONLY|O_CREAT|O_TRUNC,0766);

    while (read(fd1,&loanEntry,sizeof(struct assignedLoan))>0){
        if(strcmp(loanID,loanEntry.loanID)==0) {
            continue;
        }
        write(fd2,&loanEntry,sizeof(struct assignedLoan));
    }

    remove("managerDB/loanApplication.dat");
    rename("managerDB/temp.dat","managerDB/loanApplication.dat");
    
    close(fd1);
    close(fd2);
    return 1;
}
int processLoan(int employID,char *loanID){
    struct assignedLoan loanEntry;
    struct loanApplication loanForm;
    char path[PATH_LEN];
    _Bool found = 0;
    off_t offset = 0;

    snprintf(path,PATH_LEN,"%d/assignedloan.dat",employID);
    int fd1 = open(path,O_RDWR);
    if(fd1<0){
        perror("Failed to open assignedloan.dat");
        return -1;
    }

    while(read(fd1,&loanEntry,sizeof(struct assignedLoan))>0){
        if(strcmp(loanID,loanEntry.loanID)==0){
            found = 1;
            break;
        }
        offset+=sizeof(struct assignedLoan);
    }

    if(!found){
        perror("No such loan Application");
        close(fd1);
        return 0;
    }

    snprintf(loanEntry.status,sizeof(loanEntry.status),"%s","Process");
    lseek(fd1,offset,SEEK_SET);
    if(write(fd1,&loanEntry,sizeof(struct assignedLoan))<0){
        perror("Failed to process");
        close(fd1);
        return -1;
    }
    close(fd1);

    bzero(path,PATH_LEN);
    snprintf(path,PATH_LEN,"%d/userloandetail.dat",loanEntry.uid);
    int fd2 = open(path,O_RDWR);
    if(fd1<0){
        perror("Failed to open userloandetail.dat");
        return -1;
    }
    offset = 0;
    while(read(fd2,&loanForm,sizeof(struct loanApplication))>0){
        if(strcmp(loanID,loanForm.loanID)==0){break;}
        offset+=sizeof(struct loanApplication);
    }
    snprintf(loanForm.status,sizeof(loanForm.status),"%s","Process");
    lseek(fd2,offset,SEEK_SET);
    if(write(fd2,&loanForm,sizeof(struct loanApplication))<0){
        perror("Failed to process");
        close(fd1);
        return -1;
    }
    close(fd2);
    return 1;
}
int accept_rejectLoanApp(int employID,char *loanID,int act){
    struct assignedLoan loanEntry;
    struct loanApplication loanForm;
    char path[PATH_LEN],path2[PATH_LEN],path3[PATH_LEN];
    int userID;
    _Bool found = 0;
    off_t offset = 0;

    snprintf(path,PATH_LEN,"%d/assignedloan.dat",employID);
    snprintf(path2,PATH_LEN,"%d/temp.dat",employID);
    int fd1 = open(path,O_RDWR);
    int fd2 = open(path2,O_RDWR|O_CREAT|O_TRUNC,0766);
    if(fd1<0 || fd2<0){
        perror("Failed to open assignedloan.dat");
        return -1;
    }

    while(read(fd1,&loanEntry,sizeof(struct assignedLoan))>0){
        if(strcmp(loanID,loanEntry.loanID)==0){
            found = 1;
            userID=loanEntry.uid;
            continue;
        }
        write(fd2,&loanEntry,sizeof(struct assignedLoan));
    }
    close(fd1);
    close(fd2);

    if(!found){
        perror("No such loan Application");
        return 0;
    }

    bzero(path3,PATH_LEN);
    snprintf(path3,PATH_LEN,"%d/userloandetail.dat",userID);
    fd2 = open(path3,O_RDWR);
    if(fd2<0){
        perror("Failed to open userloandetail.dat");
        return -1;
    }
    offset = 0;
    while(read(fd2,&loanForm,sizeof(struct loanApplication))>0){
        if(strcmp(loanID,loanForm.loanID)==0){break;}
        offset+=sizeof(struct loanApplication);
    }
    if(act==1)
        snprintf(loanForm.status,sizeof(loanForm.status),"%s","Accepted");
    else if(act==2)
        snprintf(loanForm.status,sizeof(loanForm.status),"%s","Rejected");
    lseek(fd2,offset,SEEK_SET);
    if(write(fd2,&loanForm,sizeof(struct loanApplication))<0){
        perror("Failed to process");
        close(fd1);
        return -1;
    }
    close(fd2);

    remove(path);
    rename(path2,path);

    return 1;
}


//Server component ends




int employeeZone(int sockfd,int currUserID,sem_t *s){
    char buff[BUFF_SIZE];
    int choice;
    ssize_t bytes;

    while(1){
        read(sockfd,&choice,sizeof(int));

        if(choice==1){
            int UID;
            char username[MAX_LEN];
            char pwd[MAX_LEN];
            char name[MAX_LEN];
            char email[MAX_LEN];
            char phone[MAX_LEN];

            write(sockfd,"Enter Customer ID: ",strlen("Enter Customer ID: "));
            bytes = read(sockfd,&UID,sizeof(int));

            int crossverify = userIdExist(UID);
            write(sockfd,&crossverify,sizeof(int));
            //sync wait
            read(sockfd,buff,BUFF_SIZE-1);
            if(crossverify==1){
                write(sockfd,"This ID is not available\n",strlen("This ID is not available\n"));
                continue;
            }

            write(sockfd,"Enter Username: ",strlen("Enter Username: "));
            bytes = read(sockfd,username,MAX_LEN-1);
            username[bytes]='\0';

            write(sockfd,"Enter Password: ",strlen("Enter Password: "));
            bytes = read(sockfd,pwd,MAX_LEN-1);
            pwd[bytes]='\0';

            write(sockfd,"Enter Name: ",strlen("Enter Name: "));
            bytes = read(sockfd,name,MAX_LEN-1);
            name[bytes]='\0';

            write(sockfd,"Enter Email: ",strlen("Enter Email: "));
            bytes = read(sockfd,email,MAX_LEN-1);
            email[bytes]='\0';

            write(sockfd,"Enter phone: ",strlen("Enter phone: "));
            bytes = read(sockfd,phone,MAX_LEN-1);
            phone[bytes]='\0';

            sem_wait(s);
            int registerExitStatus = register_user(UID,username,pwd,name,email,phone);
            if(registerExitStatus!=1)
                write(sockfd,"Failed to Register\n",strlen("Failed to Register\n"));
            else
                write(sockfd,"Registraion Success\n",strlen("Registraion Success\n"));
            sem_post(s);
        }else if(choice==2){
            int uid,change,whichDetail,checkUpdateStatus,verifyUser;
            char name[MAX_LEN];
            char email[MAX_LEN];
            char phone[MAX_LEN];

            write(sockfd,"Enter Customer ID: ",strlen("Enter Customer ID: "));
            bytes = read(sockfd,&uid,sizeof(uid));

            verifyUser = userIdExist(uid);
            write(sockfd,&verifyUser,sizeof(int));
            read(sockfd,buff,BUFF_SIZE-1);
            if(verifyUser!=1){
                write(sockfd,"User doesn't Exit\n",strlen("User doesn't Exit\n"));
                continue;
            }

            write(sockfd,"Which detail to change?\n1. Name\n2. Email\n3. Phone\n",strlen("Which detail to change?\n1. Name\n2. Email\n3. Phone\n"));
            bytes = read(sockfd,&change,sizeof(int));

            write(sockfd,"sync",strlen("sync"));
            sem_wait(s);
            if(change==1){
                //Get new Name
                bytes = read(sockfd,name,MAX_LEN-1);
                name[bytes]='\0';

                checkUpdateStatus = modifyUserDetail(uid,change,name);
            }else if(change==2){
                //Get new Email
                bytes = read(sockfd,email,MAX_LEN-1);
                name[bytes]='\0';

                checkUpdateStatus = modifyUserDetail(uid,change,email);
            }else if(change==3){
                //Get new phone
                bytes = read(sockfd,phone,MAX_LEN-1);
                name[bytes]='\0';

                checkUpdateStatus = modifyUserDetail(uid,change,phone);                
            }
            if(checkUpdateStatus!=1) write(sockfd,"Failed to update\n",strlen("Failed to update\n"));
            else write(sockfd,"Successfully Updated\n",strlen("Successfully Updated\n"));
            sem_post(s);
        }
        else if(choice==3){
            //Process Loan
            char loanID[20];

            write(sockfd,"Enter LoanID: ",strlen("Enter LoanID: "));
            read(sockfd,loanID,sizeof(loanID));

            sem_wait(s);
            int processStatus = processLoan(currUserID,loanID);
            sem_post(s);
            if(processStatus==0)
                write(sockfd,"No Such LoanID Exist\n",strlen("No Such LoanID Exist\n"));
            else if(processStatus==1)
                write(sockfd,"Success!\n",strlen("Success!\n"));
            else
                write(sockfd,"Failed to process Loan\n",strlen("Failed to process Loan\n"));
        }
        else if(choice==4){
            //Acccpt/Reject Loan
            char loanID[20];
            int act;

            write(sockfd,"Enter LoanID: ",strlen("Enter LoanID: "));
            read(sockfd,loanID,sizeof(loanID));

            write(sockfd,"1. Accept\n2. Reject\n",strlen("1. Accept\n2. Reject\n"));
            read(sockfd,&act,sizeof(int));

            sem_wait(s);
            int acceptanceStatus = accept_rejectLoanApp(currUserID,loanID,act);
            sem_post(s);
            if(acceptanceStatus==0)
                write(sockfd,"No Such LoanID Exist\n",strlen("No Such LoanID Exist\n"));
            else if(acceptanceStatus==1)
                write(sockfd,"Success!\n",strlen("Success!\n"));
            else
                write(sockfd,"Failed to process Loan\n",strlen("Failed to process Loan\n"));
        }
        else if(choice==5){
            _Bool permission = 1;
            write(sockfd,&permission,sizeof(_Bool));
        }
        else if(choice==6){
            char username[MAX_LEN],currPwd[MAX_LEN],newPwd[MAX_LEN];

            write(sockfd,"Enter Username: ",strlen("Enter Username: "));
            bytes = read(sockfd,username,MAX_LEN);
            username[bytes]='\0';

            write(sockfd,"Enter Current Password: ",strlen("Enter Current Password: "));
            bytes = read(sockfd,currPwd,MAX_LEN);
            currPwd[bytes]='\0';

            //verify account
            int userExistStatus = login(username,currPwd);
            write(sockfd,&userExistStatus,sizeof(int));
            //Sync code
            read(sockfd,buff,BUFF_SIZE-1);
            if(userExistStatus==0){
                write(sockfd,"Account is Deactive\n",strlen("Account is Deactive\n"));
                continue;
            }
            else if(userExistStatus!=currUserID){
                write(sockfd,"Wrong Credentials!\n",strlen("Wrong Credentials!\n"));
                continue;
            }

            //Get new password
            write(sockfd,"Enter New Password: ",strlen("Enter New Password: "));
            bytes = read(sockfd,newPwd,MAX_LEN);
            newPwd[bytes]='\0';

            //password change status
            sem_wait(s);
            int updateStatus = change_password(username,currPwd,newPwd);
            sem_post(s);
            write(sockfd,&updateStatus,sizeof(int));
            //Sync code
            read(sockfd,buff,BUFF_SIZE-1);
            if(updateStatus!=currUserID){
                write(sockfd,"Failed to Update password\n",strlen("Failed to Update password\n"));
            }
            else
                write(sockfd,"Password Updated successfully\n",strlen("Password Updated successfully\n"));
        }
        else if(choice==7){break;}
    }
    return 1;
}

int managerZone(int sockfd,int currUserID,sem_t *s){
    char buff[BUFF_SIZE];
    int choice;
    ssize_t bytes;

    while (1){
        read(sockfd,&choice,sizeof(int));
        
        if(choice==1){
            int custID;
            
            write(sockfd,"Enter Customer ID: ",strlen("Enter Customer ID: "));
            read(sockfd,&custID,sizeof(int));
            
            sem_wait(s);
            int status = active_deactiveUser(custID,1);
            if(status==1){
                write(sockfd,"Success!\n",strlen("Success!\n"));
            }else if(status==0){
                write(sockfd,"Already Activated\n",strlen("Already Activated\n"));
            }else if(status==-1){
                write(sockfd,"No User Found!\n",strlen("No User Found!\n"));
            }
            else if(status==-2){
                write(sockfd,"Failed to activate\n",strlen("Failed to activate\n"));
            }
            sem_post(s);
        }else if(choice==2){
            int custID;
            write(sockfd,"Enter Customer ID: ",strlen("Enter Customer ID: "));
            read(sockfd,&custID,sizeof(int));

            sem_wait(s);
            int status = active_deactiveUser(custID,0);
            if(status==1){
                write(sockfd,"Success!\n",strlen("Success!\n"));
            }else if(status==0){
                write(sockfd,"Already deactivated\n",strlen("Already deactivated\n"));
            }else if(status==-1){
                write(sockfd,"No User Found!\n",strlen("No User Found!\n"));
            }
            else if(status==-2){
                write(sockfd,"Failed to deactivate\n",strlen("Failed to deactivate\n"));
            }
            sem_post(s);
        }
        else if (choice==3){
            //Assigning loan to employee
            int employeeID;
            char loanID[16];

            write(sockfd,"Enter LoanID to assign: ",strlen("Enter LoanID to assign: "));
            read(sockfd,loanID,sizeof(loanID));
            printf("%s",loanID);

            write(sockfd,"Enter Employee ID to assign: ",strlen("Enter Employee ID to assign: "));
            read(sockfd,&employeeID,sizeof(int));

            if(userIdExist(employeeID)!=1){
                write(sockfd,"Employee Doesn't Exist\n",strlen("Employee Doesn't Exist\n"));
                continue;
            }

            sem_wait(s);
            int assginedStatus = assignLoanToEmployee(employeeID,loanID);
            if(assginedStatus==1){
                write(sockfd,"Successfully Assigned!\n",strlen("Successfully Assigned!\n"));
            }else if(assginedStatus==0){
                write(sockfd,"We don't have any loan Application from Customer\n",strlen("We don't have any loan Application from Customer\n"));
            }else if(assginedStatus==-1){
                write(sockfd,"No Such LoanID Exist\n",strlen("No Such LoanID Exist\n"));
            }
            else{
                write(sockfd,"Error Occured!\n",strlen("Error Occured!\n"));
            }
            sem_post(s);
        }
        else if(choice==4){
            _Bool permission = 1;
            write(sockfd,&permission,sizeof(_Bool));
        }else if(choice==5){
            //Update Manager Password
            char username[MAX_LEN],currPwd[MAX_LEN],newPwd[MAX_LEN];

            write(sockfd,"Enter Username: ",strlen("Enter Username: "));
            bytes = read(sockfd,username,MAX_LEN);
            username[bytes]='\0';

            write(sockfd,"Enter Current Password: ",strlen("Enter Current Password: "));
            bytes = read(sockfd,currPwd,MAX_LEN);
            currPwd[bytes]='\0';

            //verify account
            int userExistStatus = login(username,currPwd);
            write(sockfd,&userExistStatus,sizeof(int));
            //Sync code
            read(sockfd,buff,BUFF_SIZE-1);
            if(userExistStatus==0){
                write(sockfd,"Account is Deactive\n",strlen("Account is Deactive\n"));
                continue;
            }
            else if(userExistStatus!=currUserID){
                write(sockfd,"Wrong Credentials!\n",strlen("Wrong Credentials!\n"));
                continue;
            }
            

            //Get new password
            write(sockfd,"Enter New Password: ",strlen("Enter New Password: "));
            bytes = read(sockfd,newPwd,MAX_LEN);
            newPwd[bytes]='\0';

            //password change status
            sem_wait(s);
            int updateStatus = change_password(username,currPwd,newPwd);
            write(sockfd,&updateStatus,sizeof(int));
            //Sync code
            read(sockfd,buff,BUFF_SIZE-1);
            if(updateStatus!=currUserID){
                write(sockfd,"Failed to Update password\n",strlen("Failed to Update password\n"));
            }
            else
                write(sockfd,"Password Updated successfully\n",strlen("Password Updated successfully\n"));
            sem_post(s);
        }else if (choice==6){
            _Bool permission = 1;
            write(sockfd,&permission,sizeof(_Bool));
        }
        else if(choice==7){break;}   
    }
    return 1;
}

int administratorZone(int sockfd,int currUserID,sem_t *s){
    ssize_t bytes;
    char buff[BUFF_SIZE];
    int choice;

    while (1)
    {
        //Getting Menu  choice
        read(sockfd,&choice,sizeof(int));

        if(choice==1 || choice==2){
            int uid,change,whichDetail,checkUpdateStatus;
            char name[MAX_LEN];
            char email[MAX_LEN];
            char phone[MAX_LEN];

            if(choice==1)
                write(sockfd,"Enter Emplyoee ID: ",strlen("Enter Emplyoee ID: "));
            else
                write(sockfd,"Enter Customer ID: ",strlen("Enter Customer ID: "));
            bytes = read(sockfd,&uid,sizeof(uid));

            write(sockfd,"Which detail to change?\n1. Name\n2. Email\n3. Phone\n",strlen("Which detail to change?\n1. Name\n2. Email\n3. Phone\n"));
            bytes = read(sockfd,&change,sizeof(int));

            write(sockfd,"sync",strlen("sync"));
            sem_wait(s);
            if(change==1){
                //Get new Name
                bytes = read(sockfd,name,MAX_LEN-1);
                name[bytes]='\0';

                checkUpdateStatus = modifyUserDetail(uid,change,name);
            }else if(change==2){
                //Get new Email
                bytes = read(sockfd,email,MAX_LEN-1);
                name[bytes]='\0';

                checkUpdateStatus = modifyUserDetail(uid,change,email);
            }else if(change==3){
                //Get new phone
                bytes = read(sockfd,phone,MAX_LEN-1);
                name[bytes]='\0';

                checkUpdateStatus = modifyUserDetail(uid,change,phone);                
            }
            sem_post(s);
            if(checkUpdateStatus!=1) write(sockfd,"Failed to update\n",strlen("Failed to update\n"));
            else write(sockfd,"Successfully Updated\n",strlen("Successfully Updated\n"));
        }
        else if(choice==3){
            //Register new Employee
            int UID;
            char username[MAX_LEN];
            char pwd[MAX_LEN];
            char name[MAX_LEN];
            char email[MAX_LEN];
            char phone[MAX_LEN];

            write(sockfd,"Enter Employee ID: ",strlen("Enter Employee ID: "));
            bytes = read(sockfd,&UID,sizeof(int));

            int crossverify = userIdExist(UID);
            write(sockfd,&crossverify,sizeof(int));
            //sync wait
            read(sockfd,buff,BUFF_SIZE-1);
            if(crossverify==1){
                write(sockfd,"This ID is not available\n",strlen("This ID is not available\n"));
                continue;
            }

            write(sockfd,"Enter Username: ",strlen("Enter Username: "));
            bytes = read(sockfd,username,MAX_LEN-1);
            username[bytes]='\0';

            write(sockfd,"Enter Password: ",strlen("Enter Password: "));
            bytes = read(sockfd,pwd,MAX_LEN-1);
            pwd[bytes]='\0';

            write(sockfd,"Enter Name: ",strlen("Enter Name: "));
            bytes = read(sockfd,name,MAX_LEN-1);
            name[bytes]='\0';

            write(sockfd,"Enter Email: ",strlen("Enter Email: "));
            bytes = read(sockfd,email,MAX_LEN-1);
            email[bytes]='\0';

            write(sockfd,"Enter phone: ",strlen("Enter phone: "));
            bytes = read(sockfd,phone,MAX_LEN-1);
            phone[bytes]='\0';

            sem_wait(s);
            int registerExitStatus = register_user(UID,username,pwd,name,email,phone);
            if(registerExitStatus!=1)
                write(sockfd,"Failed to Register\n",strlen("Failed to Register\n"));
            else
                write(sockfd,"Registraion Success\n",strlen("Registraion Success\n"));
            sem_post(s);
        }
        else if(choice==4){
            //Manage User roles
            int oldID,newID,valid;

            write(sockfd,"Whose role you wants to change?\n",strlen("Whose role you wants to change?\n"));
            read(sockfd,&oldID,sizeof(int));

            valid = userIdExist(oldID);
            write(sockfd,&valid,sizeof(int));
            //sync wait
            read(sockfd,buff,MAX_LEN-1);
            if(valid==0){
                write(sockfd,"User doesn't Exist\n",strlen("User doesn't Exist\n"));
                continue;
            }

            //When User exist
            if(oldID>60){
                write(sockfd,"Assign Role:\n1. Manager\n2. Employee\n",strlen("Assign Role:\n1. Manager\n2. Employee\n"));
                read(sockfd,&newID,sizeof(int));
            }
            else if(oldID>30 && oldID<=60){
                write(sockfd,"Assign Role:\n1. Manager\n2. Customer\n",strlen("Assign Role:\n1. Manager\n2. Customer\n"));
                read(sockfd,&newID,sizeof(int));
            }
            else if(oldID>10 && oldID<=30){
                write(sockfd,"Assign Role:\n1. Customer\n2. Employee\n",strlen("Assign Role:\n1. Customer\n2. Employee\n"));
                read(sockfd,&newID,sizeof(int));
            }

            valid = userIdExist(newID);
            write(sockfd,&valid,sizeof(int));
            //sync wait
            read(sockfd,buff,MAX_LEN-1);
            if(valid==1){
                write(sockfd,"User already Exist\n",strlen("User already Exist\n"));
                continue;
            }

            sem_wait(s);
            int roleChangeStatus = managerUserRoles(oldID,newID);
            write(sockfd,&roleChangeStatus,sizeof(int));
            //sync wait
            read(sockfd,buff,MAX_LEN-1);
            if(roleChangeStatus!=1){
                write(sockfd,"Failed to Update role\n",strlen("Failed to Update role\n"));
            }
            else{
                write(sockfd,"Role updated successfully\n",strlen("Role updated successfully\n"));
            }
            sem_post(s);
        }
        else if(choice==5){
            char username[MAX_LEN],currPwd[MAX_LEN],newPwd[MAX_LEN];

            write(sockfd,"Enter Username: ",strlen("Enter Username: "));
            bytes = read(sockfd,username,MAX_LEN);
            username[bytes]='\0';

            write(sockfd,"Enter Current Password: ",strlen("Enter Current Password: "));
            bytes = read(sockfd,currPwd,MAX_LEN);
            currPwd[bytes]='\0';

            //verify account
            int userExistStatus = login(username,currPwd);
            write(sockfd,&userExistStatus,sizeof(int));
            //Sync code
            read(sockfd,buff,BUFF_SIZE-1);
            if(userExistStatus==0){
                write(sockfd,"Account is Deactive\n",strlen("Account is Deactive\n"));
                continue;
            }
            else if(userExistStatus!=currUserID){
                write(sockfd,"Wrong Credentials!\n",strlen("Wrong Credentials!\n"));
                continue;
            }

            //Get new password
            write(sockfd,"Enter New Password: ",strlen("Enter New Password: "));
            bytes = read(sockfd,newPwd,MAX_LEN);
            newPwd[bytes]='\0';

            //password change status
            sem_wait(s);
            int updateStatus = change_password(username,currPwd,newPwd);
            write(sockfd,&updateStatus,sizeof(int));
            //Sync code
            read(sockfd,buff,BUFF_SIZE-1);
            if(updateStatus!=currUserID){
                write(sockfd,"Failed to Update password\n",strlen("Failed to Update password\n"));
            }
            else
                write(sockfd,"Password Updated successfully\n",strlen("Password Updated successfully\n"));
            sem_post(s);
        }
        else if(choice==6)break;
    }
    return 1;
}

int customerZone(int sockfd,int currUserID,sem_t *s){
    char username[32],userpwd[32],usermail[32];
    ssize_t bytes;
    char buff[BUFF_SIZE];
    int mychoice;

    while(1){
        read(sockfd,&mychoice,sizeof(int));

        if(mychoice==1){
            long int userBal = checkUserBalance(currUserID);
            write(sockfd,&userBal,sizeof(long int));
            //Sync wait
            read(sockfd,buff,BUFF_SIZE-1);
        }
        else if(mychoice==2){
            long int amount;
            write(sockfd,"Enter deposit amount: ",strlen("Enter deposit amount: "));
            read(sockfd,&amount,sizeof(long int));
            
            sem_wait(s);
            if(depositFund(currUserID,currUserID,amount)!=1){
                fprintf(stderr,"Couldn't credit money\n");
            }
            sem_post(s);
        }
        else if(mychoice==3){
            long int amount;
            read(sockfd,&amount,sizeof(long int));

            sem_wait(s);
            int debitStatus = withdrawFund(currUserID,currUserID,amount);
            sem_post(s);

            if(debitStatus==0)
                write(sockfd,"Not enough Balance\n",strlen("Not enough Balance\n"));
            else if(debitStatus==1)
                write(sockfd,"Success\n",strlen("Success\n"));
            else
                write(sockfd,"Failed to withdraw\n",strlen("Failed to withdraw\n"));
        }
        else if(mychoice==4){
            int towhom,validUser;
            long int amount;

            write(sockfd,"Send to UserID: ",strlen("Send to UserID: "));
            read(sockfd,&towhom,sizeof(int));

            validUser = userIdExist(towhom);
            write(sockfd,&validUser,sizeof(int));
            read(sockfd,buff,BUFF_SIZE-1);
            if(validUser!=1){
                write(sockfd,"No such User Exist!\n",strlen("No such User Exist!\n"));
                continue;
            }

            write(sockfd,"Enter Amount: ",strlen("Enter Amount: "));
            //sync wait
            read(sockfd,&amount,sizeof(long int));
            
            sem_wait(s);
            if(withdrawFund(currUserID,towhom,amount)!=1){
                write(sockfd,"Not enough Balance\n",strlen("Not enough Balance\n"));
            }
            
            if(depositFund(towhom,currUserID,amount)!=1){
                write(sockfd,"Failed to send money\n",strlen("Failed to send money\n"));
            }
            sem_post(s);

            write(sockfd,"Transfered Successfully \n",strlen("Transfered Successfully \n"));
        }
        else if(mychoice==5){
            //Apply for Loan
            long int amount;
            read(sockfd,&amount,sizeof(long int));
            
            sem_wait(s);
            int status = applytoLoan(currUserID,amount);
            sem_post(s);

            if(status!=1) write(sockfd,"Error occure. Please try again!",strlen("Error occure. Please try again!"));
            else write(sockfd,"Loan Application Submitted Successfully\n",strlen("Loan Application Submitted Successfully\n"));
        }else if(mychoice==6){

        }else if(mychoice==7){
            //Take feedback
            int stars;
            char feedback[256];

            write(sockfd,"Love our service? please rate us(1-10)\n",strlen("Love our service? please rate us(1-10)\n"));
            read(sockfd,&stars,sizeof(int));

            write(sockfd,"Please write Feedback: ",strlen("Please write Feedback: "));
            bytes = read(sockfd,feedback,sizeof(feedback));

            sem_wait(s);
            int status = addFeedback(stars,feedback);
            sem_post(s);

            if(status!=1) write(sockfd,"Failed to add Feedback\n",strlen("Failed to add Feedback\n"));
            else write(sockfd,"Feedback added successfully\n",strlen("Feedback added successfully\n"));

        }else if(mychoice==8){
            //Change password
            char username[MAX_LEN],currPass[MAX_LEN],newPass[MAX_LEN];
            
            write(sockfd,"Enter Username: ",strlen("Enter Username: "));
            read(sockfd,username,MAX_LEN-1);

            write(sockfd,"Enter Current Password: ",strlen("Enter Current Password: "));
            read(sockfd,currPass,MAX_LEN-1);

            write(sockfd,"Enter New Password: ",strlen("Enter New Password: "));
            read(sockfd,newPass,MAX_LEN-1);

            sem_wait(s);
            int uid = change_password(username,currPass,newPass);
            if(uid<=0) write(sockfd,"Error changin password\n",strlen("Error changin password\n"));
            else write(sockfd,"Password changed Successfully\n",strlen("Password changed Successfully\n"));
            sem_post(s);
        }
        else if(mychoice==10){break;}
    }
    return 1;
}