#include <pthread.h>
#include <stdio.h>
#include <vector>
#include <string.h>

void* printAttrs(void* idx_);

int main(int argc, char** argv)
{
	int number_of_thr = 2;
	std::vector<pthread_t> thr(number_of_thr);
	std::vector<int> index(number_of_thr);
	memset(index.data(), 0, sizeof(index));
	for (int i = 0; i < number_of_thr; i++)
	{
		index[i] = i;
		pthread_create(thr.data() + i, NULL, printAttrs, index.data() + i);
	}
	for (int i = 0; i < number_of_thr; i++)
		pthread_join(thr[i], NULL);
	return 0;
}

void* printAttrs(void* idx_)
{
    pthread_attr_t tattr;
	int scope = {};
	size_t stack_size = {};
	int policy = {};
	int inheritsched = {};
	int detachstate = {};
	void* stackaddr = {};

	int* idx = (int*) idx_;
	printf("\nhello, world! I'm pthread with index #%d\n", *idx);
    printf("My attrs:\n");
	if (pthread_attr_init(&tattr) != 0)
		perror("Init");

	pthread_attr_getscope(&tattr, &scope);
	printf("Scope = %d\n", scope);
	printf("Possible: Process == %d, system == %d\n", \
		PTHREAD_SCOPE_PROCESS, PTHREAD_SCOPE_SYSTEM);
	printf("=--\n");

	pthread_attr_getdetachstate(&tattr, &detachstate);
	printf("Detachstate = %d\n", detachstate);
	printf("Possible: DETACHED == %d, JOINABLE == %d\n", \
		 PTHREAD_CREATE_DETACHED,  PTHREAD_CREATE_JOINABLE);
	printf("=--\n");

	pthread_attr_getstack(&tattr, &stackaddr, &stack_size);
	printf("Stack_size = %lu bytes\n", stack_size);
	printf("Stack_addr = %p\n", stackaddr);
	printf("=--\n");

	pthread_attr_getinheritsched(&tattr, &inheritsched);
	printf("Inheritsched = %d\n", inheritsched);
	printf("Possible: INHERIT_SCHED == %d, EXPLICIT_SCHED == %d\n", \
		 PTHREAD_INHERIT_SCHED,  PTHREAD_EXPLICIT_SCHED);
	printf("=--\n");

	pthread_attr_getschedpolicy(&tattr, &policy);
	printf("Policy = %d\n", policy);
	printf("Possible: FIFO == %d, RR == %d, OTHER == %d\n", \
		 SCHED_FIFO,  SCHED_RR, SCHED_OTHER);
	printf("=--\n");
    fflush(0);

	return NULL;
}
