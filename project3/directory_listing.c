#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void list_path(char *);
int main (int argc, char *argv[])
{
	if(argc < 2) {
		printf("Number of arguments for the directory_listing is less...\n");
		printf("Format:./directory_listing <file_path>...\n");
		return -1;
	}
	char* file = argv[1];
	list_path(file);
	return 0;
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
					printf("\tInode:%ju\n",path_stat.st_ino);
					list_path(ep->d_name);
				}
				else {
					puts (ep->d_name);
				}
			} else {
				puts (ep->d_name);
			}
			ep = readdir (dp);
		}
		(void) closedir (dp);
	}
	else{
		puts("File does not exist or open directory fails");
	}
}
