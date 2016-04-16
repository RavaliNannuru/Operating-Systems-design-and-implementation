#include <sys/cdefs.h>
#include <lib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int inodewalker();
int zonewalker();
int zinfo(struct stat path_stat);
int print_menu(void);
void list_path(char *file);

int main(void){

	while(TRUE){
		if(print_menu()==1) {
			break;
		}
	}
	return 0;
}

int print_menu(void){
	int result = 0;
	int choice;
	char file_path[128];
	printf("\nSelect an option\n");
	printf("1. Inode walker\n");
	printf("2. Zone walker\n");
	printf("3. Directory walker\n");
	printf("0. Break\n");
	scanf("%d", &choice);
	getchar();
	switch(choice){
	case 1:
		printf("\nInode Walker\n");
		inodewalker();
		break;
	case 2:
		printf("\nZone Walker\n");
		zonewalker();
		break;
	case 3:
		printf("\nDirectory Walker\n");
		printf("Enter File Path: ");
		scanf("%s", file_path);
		list_path(file_path);
		break;
	case 0:
		result = 1;
		break;
	}
	return result;
}

void list_path(char *file) {
	DIR *dp;
	dp = opendir (file);
	struct dirent *ep;
	struct inode *rip;
	if (dp != NULL)
	{
		ep = readdir (dp);
		while (ep) {
			struct stat path_stat;
			stat(ep->d_name, &path_stat);
			if (ep->d_name[0] != '.') {
				if(S_ISDIR(path_stat.st_mode) == 1){
					puts (ep->d_name);
					list_path(ep->d_name);
					zinfo(path_stat);
				}
				else {
					puts (ep->d_name);
					zinfo(path_stat);
				}
			} else {
				puts (ep->d_name);
			}
			ep = readdir (dp);
		}
	}
	else{
		puts("File does not exist or open directory fails");
	}
	(void) closedir (dp);
}

int inodewalker()
{
	message m;
	memset(&m, 0, sizeof(m));
	int ret = (_syscall(VFS_PROC_NR, VFS_INODEWALKER, &m));
	if (ret < 0)
		return m.m1_i1; //return error code
	return ret;
}

int zonewalker()
{
	message m;
	memset(&m, 0, sizeof(m));
	int ret = (_syscall(VFS_PROC_NR, VFS_ZONEWALKER, &m));
	if (ret < 0)
		return m.m1_i1; //return error code
	return ret;
}

int zinfo(struct stat path_stat)
{
	message m;
	memset(&m, 0, sizeof(m));
	m.m_fs_vfs_lookup.device = path_stat.st_dev;
	m.m_fs_vfs_lookup.inode = path_stat.st_ino;
	int ret = (_syscall(VFS_PROC_NR, VFS_ZINFO, &m));
	return ret;
}
