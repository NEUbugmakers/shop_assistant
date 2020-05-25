#ifndef B_BACKUP_H
#define B_BACKUP_H

void CreateFolder();
int B_backup();
int B_resume();
void CreateBackup(char* filename);
int CopyFile(char* source_path,char *destination_path);
#endif // B_BACKUP_H


