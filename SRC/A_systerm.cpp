

#include "A_systerm.h"
#include "WP_basic.h"

int A_signUp(A_account* type,A_account * systerm) {

    char password[8];
    memset(password, '\0', sizeof(password));
    strcpy(password, type->A_password);
    if(!strcmp(password,systerm->A_password)){
        return 1;
    } else{
        return 0;
    }

    return 0;
}



A_account * A_readFile(){
    FILE * A_p=NULL;
    char com[1000];
    char password [8];
    memset(com, '\0', sizeof(com));
    memset(password, '\0', sizeof(password));
    if ((A_p = fopen("A_systerm.txt", "rb+")) == NULL) {
        printf("Error!open the file A_systerm.txt failed\n");
        for(int i=0;i<6;i++){
            strcpy(password,"000000");
        }
        A_account * w = new A_account;
        w->A_right =0;
        strcpy(w->A_password,password);
        w->next =NULL;
        return w;
    } else{
        fscanf(A_p,"%s",com);
        if (strcmp(com,"******")){//如果没有标识符，
            qDebug()<<"systerm file is probably destroyed!";
            for(int i=0;i<6;i++){
                strcpy(password,"000000");
            }
            A_account * w = new A_account;
            w->A_right =0;
            strcpy(w->A_password,password);
            w->next =NULL;

            return w;

        }else{
            memset(com, '\0', sizeof(com));
            fscanf(A_p,"%s",com);
            for(int i=0;i<6;i++){
                password[i]=com[account(i)];
            }
            qDebug()<<"密码："<<password;
            A_account * w = new A_account;
            w->A_right =0;
            strcpy(w->A_password,password);
            w->next =NULL;
            fclose(A_p);
            return w;
        }
    }

}



int account(int  i){
    return i+6;
}
