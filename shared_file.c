#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char const *argv[])
{

	int shmfd = 0;
	if ((shmfd = shm_open("shared", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		perror ("SHM_OPEN");

//	ftruncate(shmfd, sizeof(int) * 1 );

	int *mem;
	if( (mem = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0)) == -1)
		perror ("MMAP");
	printf("Map addr is %6.6X\n", (unsigned int) mem);

	pid_t b = getpid();

	if (fwrite ((int) b, sizeof(int), 1, mem) < 0)
		perror("Write");
	printf("Buffer value: %d\n", b);

	sleep(6);

	if (read (*mem, (void *) b, sizeof(int)) < 0)
		perror("Read");
	printf("Buffer value: %d\n", b);

	shm_unlink("/shared_file");
	return 0;
}