#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

static sem_t* semOutEmpty1;
static sem_t* semOutEmpty2;
static sem_t* semInFull1;
static sem_t* semInFull2;
static int* buffer1;
static int* buffer2;

void write_buf (int* buffer, sem_t* empty, sem_t* full, int c)
{
	sem_wait(empty);
	*buffer = c;
//	printf("Buffer write!\n\r");
	sem_post(full);
}

int read_buf (int* buffer, sem_t* full, sem_t* empty)
{
	sem_wait(full);
	int a = *buffer;
//	printf("Buffer read!\n\r");
	sem_post(empty);
	return a;
}

static void* write_start (void* p_data)
{
	int* buffer_out = NULL;
	int character;
	sem_t* OutEmpty;
	sem_t* InFull;
	if (!strcmp(p_data, "0"))
	{
		OutEmpty = semOutEmpty1;
		InFull = semInFull2;
		buffer_out = buffer1;
	}
	else
	{
		OutEmpty = semOutEmpty2;
		InFull = semInFull1;
		buffer_out = buffer2;
	}

	system ("/bin/stty raw");
	while((character = getchar()) != 27)
	{
		if (character == 13)
		{
			printf("\n\r");
			write_buf(buffer_out, OutEmpty, InFull, (int) '\n');
			write_buf(buffer_out, OutEmpty, InFull, (int) '\r');
		}
		else
			write_buf(buffer_out, OutEmpty, InFull, character);
	}
	system ("/bin/stty cooked");
}

static void* read_start (void* p_data)
{
	int* buffer_in = NULL;
	sem_t* OutEmpty;
	sem_t* InFull;
	if (!strcmp(p_data, "0"))
	{
		OutEmpty = semOutEmpty2;
		InFull = semInFull1;
		buffer_in = buffer2;
	}
	else
	{
		OutEmpty = semOutEmpty1;
		InFull = semInFull2;
		buffer_in = buffer1;
	}
	printf("Thread read\n");

	while (1)
	{
		printf("%c", read_buf(buffer_in, InFull, OutEmpty));
		fflush(stdout);
	}
}

int main(int argc, char const *argv[])
{
	printf("%d\n", getpid());

	if ((argc != 2) | (strcmp(argv[1], "0") & strcmp(argv[1], "1")))
	{
		printf("Bad Arguments!\n\t./project 0\n\t./project 1\n");
		return 1;
	}

	int fd1, fd2;
	if ((fd1 = shm_open("/memoire1", O_CREAT | O_RDWR, 0600)) < 0)
		perror("Open /memoire1");
	if ((fd2 = shm_open("/memoire2", O_CREAT | O_RDWR, 0600)) < 0)
		perror("Open /memoire2");
	ftruncate(fd1, sizeof(int));
	ftruncate(fd2, sizeof(int));
	if((buffer1 = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0)) == -1)
		perror("/memoire1");
	if((buffer2 = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0)) == -1)
		perror("/memoire2");
	shm_unlink("Ulink /memoire1");
	shm_unlink("Ulink /memoire2");

	if ((semOutEmpty1 = sem_open("/semOutEmpty1", O_CREAT | O_RDWR, 0600, 1)) == -1)
		perror("Open /semOutEmpty1");
	if ((semOutEmpty2 = sem_open("/semOutEmpty2", O_CREAT | O_RDWR, 0600, 1)) == -1)
		perror("Open /semOutEmpty2");
	if ((semInFull1 = sem_open("/semInFull1", O_CREAT | O_RDWR, 0600, 0)) == -1)
		perror("Open /semInFull1");
	if ((semInFull2 = sem_open("/semInFull2", O_CREAT | O_RDWR, 0600, 0)) == -1)
		perror("Open /semInFull2");
	shm_unlink("Ulink /semOutEmpty1");
	shm_unlink("Ulink /semOutEmpty2");
	shm_unlink("Ulink /semInFull1");
	shm_unlink("Ulink /semInFull2");


	pthread_t write_thread, read_thread;
	if (pthread_create (& write_thread, NULL, write_start, argv[1]) != 0)
		perror("THREAD");
	if (pthread_create (& read_thread, NULL, read_start, argv[1]) != 0)
		perror("THREAD");
	printf("Thread write : %X | Thread read : %X\n", write_thread, read_thread);
	pthread_join (write_thread, NULL);
	//pthread_join (read_thread, NULL);
	
	return 0;
}