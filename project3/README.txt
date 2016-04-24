Project3 CS-551 File System Tools Implementation

Team Members
Balaji A R (A20347964)
Paulo Vitor Becker Ayrosa Monterio de Andrade (A20341532)
Sai Ravali Nannuru (A20354346)

The folder contains the following files:
1.test.c :
	This folder contains files to test system calls implemented for inode, zone and directory walker.
	-> execute ./test and choose an option
	-> option 1 for inode walker.
	-> option 2 for zone walker.
	-> option 3 for directory walker(this option prints each file and its corresponding zones)
	-> option 4 for directory walker (this option prints each file and its corresponding inode)
2. Folder repair:
	This folder contains file system tools to check and repair inode bitmap, zone bitmap, a directory file is corrupted, inode of a directory file is completely damaged.
	-> execute ./fsck for options to choose
	-> Select an option
	-> 1. repair Inode, Zone map (This option checks and repair both inode and zone bitmap)
	-> 2. repair Inode of a file (This option asks user for a file and checks its inode for its damage recovery)
	-> 3. repair directory file 
	-> 0. Break 
For detailed information about the implementation, Please refer to the Design Document. 
3. Folder usr
|usr ->
|include:
	|minix:caller.h,fsdriver.h,vfsif.h : header files which lets the minix to map system call to its number
|src
	|minix
		|fs |mfs this folder contains system call for inode, zone and directory walker
		|lib this folder contains code which maps a request from virtual file system to minix file system
		|servers this folder contains code for the virtual file system calls. once these system call is called, this request is mapped to corresponding system calls in minix file system

Building:
copy corresponding files as similar to the folder structure provided and goto folder /usr/src/releasetools and execute makehdboot. this command will install new minix and new system calls are deployed.
		


	
