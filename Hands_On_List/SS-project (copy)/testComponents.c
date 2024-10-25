#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>   
#include <unistd.h>  
#include <sys/stat.h>

#define MAX_LEN 64
#define PATH_LEN 64

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

int modifyUserDetail(int userID,int whichEntry){
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

    int c;
    while ((c = getchar()) != '\n' && c != EOF);    

    if(whichEntry==1){
        printf("Enter New Name: ");
        fgets(udetail.name, sizeof(udetail.name), stdin);
        udetail.name[strcspn(udetail.name, "\n")] = 0;
    }else if(whichEntry==1){
        printf("Enter New Email: ");
        fgets(udetail.email, sizeof(udetail.email), stdin);
        udetail.email[strcspn(udetail.email, "\n")] = 0;
    }else if(whichEntry==1){
        printf("Enter New Phone: ");
        fgets(udetail.phone, sizeof(udetail.phone), stdin);
        udetail.phone[strcspn(udetail.phone, "\n")] = 0;
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

void active_deactiveUser(int userID){
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
        return;
    }

    //User Found
    user.active = !(user.active);
    lseek(fd,offset,SEEK_SET);
    if(write(fd,&user,sizeof(struct credential))<0){
        perror("Couldn't Activate/deactivate");
        close(fd);
        return;
    }
    close(fd);
    return;
}

void transactionEntry(int userID,int toUserID,char *type,long int bal){
    struct transHistory tranHist;
    char path[PATH_LEN];

    tranHist.fromID = userID;
    tranHist.toID = toUserID;
    snprintf(tranHist.type,MAX_LEN,"%s",type);
    tranHist.balance = bal;

    snprintf(path,PATH_LEN,"%d/trans.dat",userID);
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

int depositFund(int userID,int touserID,long int addAmount){
    struct acbalance bal;
    char path[PATH_LEN];

    snprintf(path,PATH_LEN,"%d/balance.dat",userID);
    int fd = open(path,O_RDWR);
    if(fd<0) perror("Error opening balance file");
    
    read(fd,&bal,sizeof(struct acbalance));
    bal.balance = bal.balance + addAmount;
    lseek(fd,0,SEEK_SET);

    write(fd,&bal,sizeof(struct acbalance));
    close(fd);

    transactionEntry(userID,touserID,"credit",addAmount);
    return 1;
}

int withdrawFund(int userID,int touserID,long int debitAmount){
    struct acbalance bal;
    char path[PATH_LEN];

    snprintf(path,PATH_LEN,"%d/balance.dat",userID);
    int fd = open(path,O_RDWR);
    if(fd<0) perror("Error opening balance file");
    
    read(fd,&bal,sizeof(struct acbalance));
    bal.balance = bal.balance - debitAmount;
    lseek(fd,0,SEEK_SET);

    write(fd,&bal,sizeof(struct acbalance));
    close(fd);

    transactionEntry(userID,touserID,"debit",debitAmount);
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

void deleterecord(const char* uname, const char* pwd) {
    struct credential nextRecord;
    int fd1 = open("docu.dat", O_RDONLY);
    if (fd1 < 0) {
        perror("Error opening docu.dat");
        return;
    }

    int fd2 = open("temp.dat", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd2 < 0) {
        perror("Error opening temp.dat");
        close(fd1);
        return;
    }

    while (read(fd1, &nextRecord, sizeof(struct credential)) > 0) {
        if (strcmp(nextRecord.username, uname) == 0 &&
            strcmp(nextRecord.password, pwd) == 0) {
            continue;
        }
        write(fd2, &nextRecord, sizeof(struct credential));
    }

    close(fd1);
    close(fd2);

    remove("docu.dat");
    rename("temp.dat", "docu.dat");
}

// Function to register a user by saving credentials to a file
int register_user() {
    struct credential user;
    struct userDetails udetail;
    struct acbalance ubal;
    struct transHistory utrans;
    struct feedback ufeed;
    char path[PATH_LEN];

    //Input credentials
    printf("Register\n");
    printf("Enter UID: ");
    scanf("%d",&user.UID);
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);
    getchar();

    //Input User details
    printf("Enter Name: ");
    fgets(udetail.name, sizeof(udetail.name), stdin);
    udetail.name[strcspn(udetail.name, "\n")] = 0;

    printf("Enter Email: ");
    fgets(udetail.email, sizeof(udetail.email), stdin);
    udetail.email[strcspn(udetail.email, "\n")] = 0;

    printf("Enter phone: ");
    fgets(udetail.phone, sizeof(udetail.phone), stdin);
    udetail.phone[strcspn(udetail.phone, "\n")] = 0;

    udetail.UID = user.UID;
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

    user.active =1;
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
    snprintf(path,PATH_LEN,"%d/trans.dat",user.UID);

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

void printUserdetail(int userID){
    char path[PATH_LEN];
    struct userDetails userstat;

    snprintf(path,PATH_LEN,"%d/userdetail.dat",userID);
    int fd = open(path,O_RDONLY);
    read(fd,&userstat,sizeof(struct userDetails));
    close(fd);
    printf("User Information-----------------------------\n");
    printf("|Desgination: %s\n|Your ID: %d\n|Name: %s\n|Email: %s\n|Phone: %s",userstat.designation,userstat.UID,userstat.name,userstat.email,userstat.phone);
}

void printCredFile(){
    char path[]="docu.dat";
    struct credential user;

    int fd = open(path,O_RDONLY);
    printf("Fetched Data--------------------------\n");
    while(read(fd,&user,sizeof(struct credential))>0){
        printf("%d\t%d\t%s\t%s\n",user.UID,user.active,user.username,user.password);
    }
    close(fd);
    return;
}

int managerUserRoles(int oldID,int newID){
    struct userDetails currDetail;
    struct credential currCred;
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
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/balance.dat",oldID);

        fd = open(path,O_WRONLY|O_CREAT,0766);
        if(fd<0) fprintf(stderr,"Error creating <%s> files",path);
        close(fd);

        //Creating User Transaction History file
        bzero(path,PATH_LEN);
        snprintf(path,PATH_LEN,"%d/trans.dat",oldID);

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
        snprintf(path,PATH_LEN,"%d/trans.dat",oldID);
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
        snprintf(path,PATH_LEN,"%d/trans.dat",oldID);
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
void viewCustLoan(int userID){
    struct loanApplication loanEntry;
    char path[MAX_LEN];

    snprintf(path,MAX_LEN,"%d/userloandetail.dat",userID);
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("Error opening the file");
        return;
    }

    printf("LoanID\t#Application\tAmount(\u20B9)\tStatus\n");
    printf("----------------------------------------------\n");
    while (read(fd,&loanEntry,sizeof(struct loanApplication))>0){
        printf("%s\t%d\t\t\u20B9%ld\t%s\n",loanEntry.loanID,loanEntry.applicationNo,loanEntry.amount,loanEntry.status);
    }
    close(fd);
}
void viewAssignedLoan(int userID) {
    struct assignedLoan loan;
    char path[PATH_LEN];

    if(userID>30 && userID<=60)
        snprintf(path,PATH_LEN,"%d/assignedloan.dat",userID);
    else if(userID>10 && userID<=30)
        snprintf(path,PATH_LEN,"managerDB/loanApplication.dat");
    int fd = open(path, O_RDONLY);
    
    if (fd < 0) {
        perror("Error opening the file");
        return;
    }
    printf("LoanID\tUserID\tUsername\tAmount(\u20B9)\tStatus\n");
    printf("------------------------------------------------------\n");
    while (read(fd, &loan, sizeof(struct assignedLoan)) > 0) {
        printf("%s\t%d\t%-10s\t\u20B9%-10ld\t%-10s\n", loan.loanID, loan.uid, loan.username, loan.amount, loan.status);
    }
    close(fd);
}
int sendLoanAppToManager(struct userDetails user,struct loanApplication loanForm){
    struct assignedLoan recievedForm;

    recievedForm.amount=loanForm.amount;
    snprintf(recievedForm.loanID,MAX_LEN,"%s",loanForm.loanID);
    snprintf(recievedForm.status,MAX_LEN,"%s",loanForm.status);

    recievedForm.uid=user.UID;
    snprintf(recievedForm.username,MAX_LEN,"%s",user.name);

    int fd = open("managerDB/loanApplication.dat",O_WRONLY|O_APPEND);
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

void addFeedback(int starRating, const char *feedbackMessage) {
    struct feedback fbEntry;
    int fd;

    fbEntry.star = starRating;
    snprintf(fbEntry.fb, sizeof(fbEntry.fb), "%s", feedbackMessage);

    fd = open("managerDB/feedback.dat", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Error opening feedback.dat");
        return;
    }

    if (write(fd, &fbEntry, sizeof(struct feedback)) < 0) {
        perror("Error writing feedback");
    }
    close(fd);
}
void viewAllFeedback() {
    struct feedback fbEntry;
    
    int fd = open("managerDB/feedback.dat", O_RDONLY);
    if (fd < 0) {
        perror("Error opening feedback.dat");
        return;
    }

    printf("All Feedbacks\n");
    while (read(fd, &fbEntry, sizeof(struct feedback)) > 0) {
        printf("Rating: %d stars\nFeedback: %s\n",fbEntry.star,fbEntry.fb);
        printf("********\n");
    }
    close(fd);
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
    
    viewAssignedLoan(80);
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
int main() {
    int choice, auth;
    
    while (1) {
        printf("\n1. Register\n2. Login\n3. change password\n4. Exit\n5. Get user detail\n6. Fetch Credential Data\n7. Delete User\n8. Activate/Deactivate User\n9. Update User Entry\n10. Modify User role\n11. Apply to loan\n12. Add Feedback\n13. Assigned loan to Employee\n14. View Employee/User Assigned Loan\n15. Process Loan\n16. Assign/Reject Loan\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                char s1[MAX_LEN],s2[MAX_LEN];
                printf("Enter Username: ");
                scanf("%s",s1);
                printf("Enter Password: ");
                scanf("%s",s2);
                auth = login(s1,s2);
                if (auth) {
                    printf("Login successful!\n");
                } else {
                    printf("Login failed. Invalid credentials.\n");
                }
                break;
            case 3:
                char inp1[MAX_LEN],inp2[MAX_LEN],inp3[MAX_LEN];
                printf("Enter your username: ");
                scanf("%s", inp1);
                
                printf("Enter your current password: ");
                scanf("%s",inp2);
                
                printf("Enter new password: ");
                scanf("%s",inp3);
                change_password(inp1,inp2,inp3);
                break;
            case 4:
                printf("Exiting**\n");
                exit(0);
            case 5:
                int id;
                printf("Enter User ID: ");
                scanf("%d",&id);
                printUserdetail(id);
                break;
            case 6:
                printCredFile();
                break;
            case 7:
                char u1[MAX_LEN],u2[MAX_LEN];
                printf("Enter Username: ");
                scanf("%s",u1);
                printf("Enter Password: ");
                scanf("%s",u2);
                deleterecord(u1,u2);
                break;     
            case 8:
                int userid;
                printf("Enter User Id: ");
                scanf("%d",&userid);
                active_deactiveUser(userid);
                break;
            case 9:
                int id1,entry;
                printf("Enter UserID: ");
                scanf("%d",&id1);;
                printf("Which Entry to change?: ");
                scanf("%d",&entry);
                modifyUserDetail(id1,entry);
                break;
            case 10:
                int oldid,newid;
                printf("Enter old ID: ");
                scanf("%d",&oldid);
                printf("Enter new ID: ");
                scanf("%d",&newid);
                managerUserRoles(oldid,newid);
                break;
            case 11:
                int uid1;
                long int amount;
                printf("Enter UID: ");
                scanf("%d",&uid1);
                printf("Enter amount: ");
                scanf("%ld",&amount);
                applytoLoan(uid1,amount);
                viewCustLoan(uid1);
                break;;
            case 12:
                int stars;
                char fb[256];
                printf("Give Stars: ");
                scanf("%d",&stars);
                getchar();
                fgets(fb,sizeof(fb),stdin);
                fb[strcspn(fb, "\n")] = 0;
                addFeedback(stars,fb);
                viewAllFeedback();
                break;
            case 13:
                int uid2;
                char loanID[20];
                viewAssignedLoan(80);
                printf("Enter EmployeeID: ");
                scanf("%d",&uid2);
                printf("Enter loanID: ");
                scanf("%s",loanID);
                assignLoanToEmployee(uid2,loanID);
                break;
            case 14:
                int uid3,uid5;
                printf("Enter EmployeeID: ");
                scanf("%d",&uid3);
                printf("Enter CustomerID: ");
                scanf("%d",&uid5);
                printf("All Loan Application*****\n");
                viewAssignedLoan(80);
                printf("Assigned loan to %d*****\n",uid3);
                viewAssignedLoan(uid3);
                printf("Customer loan %d*****\n",uid5);
                viewCustLoan(uid5);
                break;
            case 15:
                int uid4;char loanID2[20];
                printf("Enter EmployeeID: ");
                scanf("%d",&uid4);
                viewAssignedLoan(uid4);
                printf("Enter loanID: ");
                scanf("%s",loanID2);
                processLoan(uid4,loanID2);
                break;
            case 16:
                int uid6,act1;char loanID3[20];
                printf("Enter EmployeeID: ");
                scanf("%d",&uid6);
                viewAssignedLoan(uid6);
                printf("Enter loanID: ");
                scanf("%s",loanID3);
                printf("1. Accept\n2. Reject\nEnter: ");
                scanf("%d",&act1);
                accept_rejectLoanApp(uid6,loanID3,act1);
                break;
            default:
                printf("Invalid choice.\n");
        }

        sleep(1);  // Pause for a moment
    }

    return 0;
}