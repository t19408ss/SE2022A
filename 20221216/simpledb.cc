#include "bptree.h"
#include <iostream>
#include <vector>
#include <sys/time.h>

#define DATA_LENGTH (1000*1000)
#define NUM_OPERATIONS 16

typedef struct _OPERATION {
	int key;
	bool rw; // True: read, False: write
	int offset;
} OPERATION;

typedef struct _TRANSACTION {
	OPERATION *operations[NUM_OPERATIONS];
} TRANSACTION;

std::queue<TRANSACTION *> t_r, t_rw;

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

int 
interactive()
{
	int key;

	std::cout << "Key: ";
	std::cin >> key;

	return key;
}

OPERATION *
alloc_operation(int key, bool rw, int offset)
{
	OPERATION *operation;
	if (!(operation = (OPERATION *)calloc(1, sizeof(OPERATION)))) ERR;
	operation->key = key;
	operation->rw = rw;
	operation->offset = offset;

	return operation;
}

TRANSACTION *
generate_transaction(bool read, bool write)
{
	if (!read && !write) return NULL;
	int i;
	TRANSACTION *transaction;
	std::random_device rnd;
	if (!(transaction = (TRANSACTION *)calloc(1, sizeof(TRANSACTION)))) ERR;
	for (i = 0; i < NUM_OPERATIONS; i++) {
		transaction->operations[i] = alloc_operation((rnd() % NUM_OPERATIONS) + 1, 
			(read ? (write ? (rnd() % 2 == 1 ? true : false) : true) : false) , 1);
	}
	return transaction;
}

void
search_and_read(int key, long tid, bool print)
{
	int r;
	DATA *target;
	target = find_data(Root, key);
	r = read_data(target);
	if (print) printf("Thread %ld: key %d \tread(%d)\n", tid, key, r);
}

void
search_and_write(int key, int newval, long tid, bool print)
{
	DATA *target;
	target = find_data(Root, key);
	write_data(target, newval);
	if (print) printf("Thread %ld: key %d \t write(%d)\n", tid, key, newval);
}

void
search_and_update(int key, int offset, long tid, bool print)
{
	int r;
	DATA *target;
	target = find_data(Root, key);
	r = update_data(target, offset);
	if (print) printf("Thread %ld: key %d \tupdate(%d => %d)\n", tid, key, r, r + offset);
}

void *
worker_r(void *id)
{
	int i;
	long tid = (long)id;
	TRANSACTION *t = t_r.front();
	t_r.pop();
	for (i = 0; i < NUM_OPERATIONS; i++) {
		if(t->operations[i]->rw) {
			search_and_read(t->operations[i]->key, tid, false);
		} else {
			search_and_write(t->operations[i]->key, t->operations[i]->offset, tid, false);
		}
	}
	printf(".");
	return NULL;
}

void *
worker_rw(void *id)
{
	int i;
	long tid = (long)id;
	TRANSACTION *t = t_rw.front();
	t_rw.pop();
	for (i = 0; i < NUM_OPERATIONS; i++) {
		if(t->operations[i]->rw) {
			search_and_read(t->operations[i]->key, tid, false);
		} else {
			search_and_write(t->operations[i]->key, t->operations[i]->offset, tid, false);
		}
	}
	printf(".");
	return NULL;
}

int
main(int argc, char *argv[])
{
	int i;
	int data_len, num_threads;
	std::vector<DATA *> origin;
	std::vector<pthread_t> thread;
	struct timeval begin, end;

	if (argc == 3) {
		data_len = atoi(argv[1]);
		num_threads = atoi(argv[2]);

	} else {
		data_len = 1000000;
		num_threads = 10;
	}
	printf("\nDATA LENGTH: %d\n", data_len);
	printf("NUMBER OF THREADS: %d\n", num_threads);

	init_root();
	printf("\nDatabase init");
	for (i = 1; i <= data_len; i++) {
		origin.push_back(alloc_data(i, 0));
		insert(i, origin[i-1]);
		// if(i % (data_len / 10) == 0) printf("Inserted: (%d / %d)\n", i, data_len);
	}
	printf(" ==> Inserted %d data!\n", data_len);

	// read only
	for (i = 0; i < num_threads; i++) {
		TRANSACTION *transaction = generate_transaction(true, false);
		t_r.push(transaction);
	}

	// read and write
	for (i = 0; i < num_threads; i++) {
		TRANSACTION *transaction = generate_transaction(true, true);
		t_rw.push(transaction);
	}

	printf("\nRunning Transactions <read only> ");

	begin = cur_time();
	for (i = 0; i < num_threads; i++) {
		thread.push_back(NULL);
		pthread_create(&thread[i], NULL, worker_r, (void *)(long)i);
	}
	for (i = 0; i < num_threads; i++) {
		pthread_join(thread[i], NULL);
	}
	end = cur_time();

	printf(" Finished!\n");
    printf("\tTime: %f s\n", end.tv_sec - begin.tv_sec +  (float)(end.tv_usec - begin.tv_usec) / 1000000);

	printf("\nRunning Transactions <read and write> ");

	begin = cur_time();
	for (i = 0; i < num_threads; i++) {
		thread.push_back(NULL);
		pthread_create(&thread[i], NULL, worker_rw, (void *)(long)i);
	}
	for (i = 0; i < num_threads; i++) {
		pthread_join(thread[i], NULL);
	}
	end = cur_time();

	printf(" Finished!\n");
    printf("\tTime: %f s\n\n", end.tv_sec - begin.tv_sec +  (float)(end.tv_usec - begin.tv_usec) / 1000000);


	return 0;
}
