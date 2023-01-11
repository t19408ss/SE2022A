#include "bptree.h"
#include <bits/stdc++.h>
#include <vector>
#include <sys/time.h>

#define DATA_LENGTH 1000*1000
#define DATA_VALUE_MAX 10000
#define NUM_OPERATIONS 16
#define NUM_THREADS 6
#define TIME_LIMIT 3
#define USLEEP_MAX 10

std::random_device rnd;

typedef struct _OPERATION {
	int key;
	bool rw; // True: read, False: write
	int offset;
} OPERATION;

typedef struct _TRANSACTION {
	OPERATION *operations[NUM_OPERATIONS];
} TRANSACTION;

std::vector<TRANSACTION *> tq;

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

float
time_p(struct timeval begin, struct timeval end) {
	return end.tv_sec - begin.tv_sec +  (float)(end.tv_usec - begin.tv_usec) / 1000000;
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
	int i, j, k[NUM_OPERATIONS];
	bool unique;
	TRANSACTION *transaction;
	if (!(transaction = (TRANSACTION *)calloc(1, sizeof(TRANSACTION)))) ERR;
	do {
		unique = true;
		for (i = 0; i < NUM_OPERATIONS; i++) {
			k[i] = (rnd() % DATA_LENGTH) + 1;
			transaction->operations[i] = alloc_operation(k[i], 
				(read ? (write ? (rnd() % 2 == 1 ? true : false) : true) : false) , 1);
		}
		for (i = 0; i < NUM_OPERATIONS - 1; i++) {
			for (j = i + 1; j < NUM_OPERATIONS; j++) {
				if (k[i] == k[j]) unique = false;
			}
		}
	} while (!unique);
	return transaction;
}

bool
run_transaction(TRANSACTION *transaction, bool use_index)
{
	int i, j;
	int a;
	bool lock;
	DATA *target;
	for (i = 0; i < NUM_OPERATIONS; i++) {
		if (use_index) {
			target = find_data(Root, transaction->operations[i]->key);
		} else {
			target = find_data_without_index(Head, transaction->operations[i]->key);
		}
		if (transaction->operations[i]->rw) {
			lock = r_trylock(target);
		} else {
			lock = w_trylock(target);
		}
		if (!lock) {
			for (j = 0; i < j; i++) {
				if (use_index) {
					target = find_data(Root, transaction->operations[j]->key);
				} else {
					target = find_data_without_index(Head, transaction->operations[j]->key);
				}
				if (transaction->operations[j]->rw) {
					r_unlock(target);
				} else {
					w_unlock(target);
				}
			}
			// printf("*");
			a = num_abort.load();
			num_abort.compare_exchange_strong(a, a + 1);
			return false;
		}
	}
	for (i = 0; i < NUM_OPERATIONS; i++) {
		if (use_index) {
			target = find_data(Root, transaction->operations[i]->key);
		} else {
			target = find_data_without_index(Head, transaction->operations[i]->key);
		}
		if (transaction->operations[i]->rw) {
			
		} else {
			target->val++;
		}
	}
	for (i = 0; i < NUM_OPERATIONS; i++) {
		if (use_index) {
			target = find_data(Root, transaction->operations[i]->key);
		} else {
			target = find_data_without_index(Head, transaction->operations[i]->key);
		}
		if (transaction->operations[i]->rw) {
			r_unlock(target);
		} else {
			w_unlock(target);
		}
	}
	// printf("-");
	a = num_commit.load();
	num_commit.compare_exchange_strong(a, a + 1);
	return true;
}

void
*run_with_index(void *arg) {
	long tid = (long)arg;
	TRANSACTION *t = tq[(int)tid - 1];
	for (;;) {
		run_transaction(t, true);
	}
	// printf("Thread %ld: Commited!\n", tid);
	return NULL;
}

void
*run_without_index(void *arg) {
	long tid = (long)arg;
	TRANSACTION *t = tq[(int)tid - 1];
	for (;;) {
		run_transaction(t, false);
	}
	// printf("Thread %ld: Commited!\n", tid);
	return NULL;
}

void
*time_measure(void *arg)
{
	struct timeval begin, end;
	int nc, na, ns;
	begin = cur_time();
	for (;;) {
		end = cur_time();
		if (end.tv_sec - begin.tv_sec >= TIME_LIMIT) {
			// printf("\nTIME UP!\n");
			nc = num_commit.load();
			na = num_abort.load();
			ns = nc + na;
			printf("Number of Commit:\t%10d\n", nc);
			printf("Number of Abort:\t%10d\n", na);
			printf("Total:\t\t\t%10d\n", ns);
			break;
		}
	}
	pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
	int i;
	DATA *dp;
	pthread_t t1[NUM_THREADS], t2[NUM_THREADS];

	init_root();
	num_abort.store(0);
	num_commit.store(0);

	printf("\n==== Insert data ==========================\n");
	for (i = 1; i <= DATA_LENGTH; i++) {
		DATA *d;
		d = alloc_data(i, rnd() % DATA_VALUE_MAX + 1, NULL);
		if (i == 1) {
			Head = d;
		} else {
			dp->next = d;
		}
		dp = d;
		insert(i, d);
		if(i % (DATA_LENGTH / 10) == 0) printf("Inserted key%8d:%8d\n", d->key, d->val);
	}

	// Generate update transaction
	for (i = 0; i < NUM_THREADS - 1; i++) {
		TRANSACTION *transaction = generate_transaction(true, true);
		tq.push_back(transaction);
	}

	printf("\n== Transacton throughput : with index =====\n");
    pthread_create(&t1[0], NULL, time_measure, (void *)0);
    pthread_create(&t1[1], NULL, run_with_index, (void *)1);
    pthread_create(&t1[2], NULL, run_with_index, (void *)2);
    pthread_create(&t1[3], NULL, run_with_index, (void *)3);
    pthread_create(&t1[4], NULL, run_with_index, (void *)4);
    pthread_create(&t1[5], NULL, run_with_index, (void *)5);

	pthread_join(t1[0], NULL);
	pthread_cancel(t1[1]);
	pthread_cancel(t1[2]);
	pthread_cancel(t1[3]);
	pthread_cancel(t1[4]);
	pthread_cancel(t1[5]);


	num_abort.store(0);
	num_commit.store(0);

	printf("\n== Transacton throughput : without index ==\n");
    pthread_create(&t2[0], NULL, time_measure, (void *)0);
    pthread_create(&t2[1], NULL, run_without_index, (void *)1);
    pthread_create(&t2[2], NULL, run_without_index, (void *)2);
    pthread_create(&t2[3], NULL, run_without_index, (void *)3);
    pthread_create(&t2[4], NULL, run_without_index, (void *)4);
    pthread_create(&t2[5], NULL, run_without_index, (void *)5);

	pthread_join(t2[0], NULL);
	pthread_cancel(t2[1]);
	pthread_cancel(t2[2]);
	pthread_cancel(t2[3]);
	pthread_cancel(t2[4]);
	pthread_cancel(t2[5]);

	exit(0);
}
