#include <sys/cdefs.h>
#include <lib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int inodewalker();
int zonewalker();
int zinfo();

int print_menu(void);

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
			zinfo();
			break;
		case 0:
			result = 1;
			break;
	}
	return result;
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

int zinfo()
{
  message m;
  memset(&m, 0, sizeof(m));
  int ret = (_syscall(VFS_PROC_NR, VFS_ZINFO, &m));
  return ret;
}
