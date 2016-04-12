#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

struct inode {
  u16_t i_mode;		/* file type, protection, etc. */
  u16_t i_nlinks;		/* how many links to this file */
  u16_t i_uid;			/* user id of the file's owner */
  u16_t i_gid;			/* group number */
  i32_t i_size;			/* current file size in bytes */
  u32_t i_atime;		/* time of last access (V2 only) */
  u32_t i_mtime;		/* when was file data last changed */
  u32_t i_ctime;		/* when was inode itself changed (V2 only)*/
  u32_t i_zone[V2_NR_TZONES]; /* zone numbers for direct, ind, and dbl ind */

  /* The following items are not present on the disk. */
  dev_t i_dev;			/* which device is the inode on */
  ino_t i_num;			/* inode number on its (minor) device */
  int i_count;			/* # times inode used; 0 means slot is free */
  unsigned int i_ndzones;	/* # direct zones (Vx_NR_DZONES) */
  unsigned int i_nindirs;	/* # indirect zones per indirect block */
  struct super_block *i_sp;	/* pointer to super block for inode's device */
  char i_dirt;			/* CLEAN or DIRTY */
  zone_t i_zsearch;		/* where to start search for new zones */
  off_t i_last_dpos;		/* where to start dentry search */

  char i_mountpoint;		/* true if mounted on */

  char i_seek;			/* set on LSEEK, cleared on READ/WRITE */
  char i_update;		/* the ATIME, CTIME, and MTIME bits are here */

  LIST_ENTRY(inode) i_hash;     /* hash list */
  TAILQ_ENTRY(inode) i_unused;  /* free and unused list */

};
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
					/* Find the inode referred */
					if ((rip = get_inode(path_stat.st_dev, path_stat.st_ino)) == NULL)
					{
						puts("Inode is not found\n");
					}
					else {
						printf("\tUID:%d\n",rip->i_uid);
					}
					printf("\tInode:%ju\n",path_stat.st_ino);
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
