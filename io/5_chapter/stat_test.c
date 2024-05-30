#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

/**
  stat函数测试，它可以获取一个文件的属性，主要通过一个struct类型变量获取
  传入一个stat类型指针，返回一个stat相关信息
  其中一个st_mode可以区分，这个文件是linux系统中7中文件类型的哪一种

#include <sys/stat.h>
int stat(const char *restrict path, struct stat *restrict buf);

 **/
int main(void)
{
	struct stat st;
	int ret;

	ret = stat("./test.txt",&st);
	if(ret == -1)
	{
		perror("stat error");
		return 1;
	}

	printf("inode: %ld\n",st.st_ino);
	printf("size: %ld\n",st.st_size);

	switch (st.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}
	

	return 0;
}
