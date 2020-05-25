#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <QtDebug>
#include <string.h>
// 创建文件夹
void CreateFolder()
{
    //文件夹名称
    char folderName[] = "BackupData";
    // 文件夹不存在则创建文件夹
    if (_access(folderName, 0) == -1) _mkdir(folderName);
}
//然后在main函数中调用CreateFolder函数即可
int CopyFile(char* source_path,char *destination_path){//复制文件
    char buffer[1024];
    FILE *fcopy,*fpaste;//定义两个文件流，分别用于文件的读取和写入int len;
    if(strstr(source_path,".txt")==NULL){
        if((fcopy=fopen(source_path,"rb"))==NULL){//打开源文件的文件流
            qDebug()<<"源文件丢失";
            return 0;
        }
        if((fpaste=fopen(destination_path,"wb"))==NULL){//打开目标文件的文件流
            fclose(fcopy);
            qDebug()<<"目标文件创建失败";
            return 0;
        }
    }
    else{
        if((fcopy=fopen(source_path,"r"))==NULL){//打开源文件的文件流
            qDebug()<<"源文件丢失";
            return 0;
        }
        if((fpaste=fopen(destination_path,"w"))==NULL){//打开目标文件的文件流
            fclose(fcopy);
            qDebug()<<"目标文件创建失败";
            return 0;
        }
    }
    int len;//len为fread读到的字节长
    while((len=fread(buffer,1,1024,fcopy))>0){//从源文件中读取数据并放到缓冲区中，第二个参数1也可以写成sizeof(char)
        fwrite(buffer,1,len,fpaste);//将缓冲区的数据写到目标文件中
    }
    fclose(fcopy);
    fclose(fpaste);
    return 1;
}
void CreateBackup(char* filename){
    FILE* fp;
    fp=fopen(filename,"w");
    fprintf(fp,"member.txt\ndiscount.txt\nback_list.txt\nroot\ngoods\nSR_sales\nP_list.txt\nW_list.txt\nA_systerm.txt");
    fclose(fp);
}
int B_backup()
{
    CreateFolder();
    FILE* fp;
    int back=1;
    char filecopy[30],filepaste[40],prefix[]="BackupData/",suffix[]="_back";
    fp = fopen("back_list.txt", "r");
    if (fp == NULL) {
        qDebug() << "打开文件失败!";
        CreateBackup("back_list.txt");
        fp = fopen("back_list.txt", "r");
        back=0;
    }else  qDebug() << "打开文件成功!";
    while (!feof(fp)) {
        fscanf(fp,"%s",filecopy);
        strcpy(filepaste,prefix);
        strcat(filepaste,filecopy);
        strcat(filepaste,suffix);
        qDebug()<<filecopy;
        qDebug()<<filepaste;
        CopyFile(filecopy,filepaste);
    }
    fclose(fp);
    return back;
}
int B_resume()
{
    FILE* fp;
    int back=1;
    char filecopy[30],filepaste[40],prefix[]="BackupData/",suffix[]="_back";
    fp = fopen("BackupData/back_list.txt_back", "r");
    if (fp == NULL) {
        qDebug() << "打开文件失败!";
        CreateBackup("BackupData/back_list.txt_back");
        fp = fopen("BackupData/back_list.txt_back", "r");
        back=0;
    }else  qDebug() << "打开文件成功!";
    while (!feof(fp)) {
        fscanf(fp,"%s",filecopy);
        strcpy(filepaste,prefix);
        strcat(filepaste,filecopy);
        strcat(filepaste,suffix);
        qDebug()<<filecopy;
        qDebug()<<filepaste;
        CopyFile(filepaste,filecopy);
    }
    fclose(fp);
    return back;
}
