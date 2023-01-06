#include "bptree.h"
#include <bits/stdc++.h>
#include <vector>
#include <sys/time.h>

#define DATA_LENGTH 100
#define NUM_OPERATIONS 16
#define NUM_THREADS 6
#define TIME_LIMIT 3
#define USLEEP_MAX 10

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
	int i, j, k[NUM_OPERATIONS];
	bool unique;
	TRANSACTION *transaction;
	std::random_device rnd;
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
run_transaction(TRANSACTION *transaction)
{
	int i, j;
	int a;
	bool lock;
	DATA *target;
	for (i = 0; i < NUM_OPERATIONS; i++) {
		target = find_data(Root, transaction->operations[i]->key);
		if (transaction->operations[i]->rw) {
			lock = r_trylock(target);
		} else {
			lock = w_trylock(target);
		}
		if (!lock) {
			for (j = 0; i < j; i++) {
				target = find_data(Root, transaction->operations[j]->key);
				if (transaction->operations[j]->rw) {
					r_unlock(target);
				} else {
					w_unlock(target);
				}
			}
			// printf("*");
			a = nowait_abort.load();
			nowait_abort.compare_exchange_strong(a, a + 1);
			return false;
		}
	}
	for (i = 0; i < NUM_OPERATIONS; i++) {
		target = find_data(Root, transaction->operations[i]->key);
		if (transaction->operations[i]->rw) {
			
		} else {
			target->val++;
		}
	}
	for (i = 0; i < NUM_OPERATIONS; i++) {
		target = find_data(Root, transaction->operations[i]->key);
		if (transaction->operations[i]->rw) {
			r_unlock(target);
		} else {
			w_unlock(target);
		}
	}
	// printf("-");
	a = nowait_commit.load();
	nowait_commit.compare_exchange_strong(a, a + 1);
	return true;
}

void
*nowait(void *arg) {
	// long tid = (long)arg;
	TRANSACTION *t = t_rw.front();
	t_rw.pop();
	for (;;) {
		run_transaction(t);
	}
	// printf("Thread %ld: Complete!\n", tid);
	return NULL;
}

void
*waitrnd(void *arg) {
	// long tid = (long)arg;
	std::random_device rnd;
	TRANSACTION *t = t_rw.front();
	t_rw.pop();
	for (;;) {
		if (!run_transaction(t)) usleep(rnd() % USLEEP_MAX + 1);
	}
	// printf("Thread %ld: Complete!\n", tid);
	return NULL;
}

void
*time_measure(void *arg)
{
	struct timeval begin, end;
	begin = cur_time();
	for (;;) {
		end = cur_time();
		if (end.tv_sec - begin.tv_sec >= TIME_LIMIT) {
			// printf("\nTIME UP!\n");
			printf("Number of Commit: %d\n", nowait_commit.load());
			printf("Number of Abort: %d\n", nowait_abort.load());
			exit(0);
		}
	}
	return NULL;
}

int
main(int argc, char *argv[])
{
	int i;
	// int e, g;
	std::vector<DATA *> origin;
	pthread_t thread[NUM_THREADS];
	struct timeval begin, end;

	init_root();

	// printf("\n==== Insert ===============\n");
	for (i = 1; i <= DATA_LENGTH; i++) {
		origin.push_back(new DATA);
		origin[i-1]->key = i;
		origin[i-1]->val = 0;
		origin[i-1]->next = NULL;
		origin[i-1]->lock.store(0);
		insert(i, origin[i-1]);
		// if(i % (DATA_LENGTH / 10) == 0) printf("Inserted: %d\n", i);
	}

	// read only
	for (i = 0; i < NUM_THREADS; i++) {
		TRANSACTION *transaction = generate_transaction(true, false);
		t_r.push(transaction);
	}

	// read and write
	for (i = 0; i < NUM_THREADS; i++) {
		TRANSACTION *transaction = generate_transaction(true, true);
		t_rw.push(transaction);
	}


	// printf("\n==== Search and Update ====\n");
	begin = cur_time();
    pthread_create(&thread[0], NULL, waitrnd, (void *)0);
    pthread_create(&thread[1], NULL, waitrnd, (void *)1);
    pthread_create(&thread[2], NULL, waitrnd, (void *)2);
    pthread_create(&thread[3], NULL, waitrnd, (void *)3);
    pthread_create(&thread[4], NULL, waitrnd, (void *)4);
    pthread_create(&thread[5], NULL, time_measure, NULL);
	// e = 6;

	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread[i], NULL);
	}
	end = cur_time();

	// printf("\n==== Check =================\n");
	// printf("Expected: val = %d\n", e);
	// for (i = 1; i <= DATA_LENGTH; i++) {
	// 	g = read_data(find_data(Root, i));
	// 	if (g != e) {
	// 		printf("Update failure: key = %d, val = %d\n", i, g);
	// 		exit(1);
	// 	}
		// if (i % (DATA_LENGTH / 10) == 0) {
		// 	printf("Checked: key %d \t(val = %d)\n", i, g);
		// }
	// }
	// printf("Check Passed!\n\n");
    // printf("Time: %f s\n\n", end.tv_sec - begin.tv_sec +  (float)(end.tv_usec - begin.tv_usec) / 1000000);

	return 0;
}
