#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linked_list.h"

#define INSERT 0
#define DELETE 1
#define MEMBER 2

#define MAX_VALUE 65535 // 2^16 - 1

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

int n = 1000, m = 10000;
float mMember, mInsert , mDelete ;
int thread_count;

void PrintList(struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;
    struct list_node_s *prev_p = NULL;

    while (curr_p != NULL) {
        if (prev_p != NULL) {
            printf("(%d)(%p) [diff: %ld] -> ", curr_p->data, (void *)curr_p, (char *)curr_p - (char *)prev_p);
        } else {
            printf("(%d)(%p) -> ", curr_p->data, (void *)curr_p);
        }
        prev_p = curr_p;
        curr_p = curr_p->next;
    }
    printf("NULL\n");
} /* PrintList */

void populate_list(int n, struct list_node_s **head) {
    int value;
    for (int i = 0; i < n; i++) {
        do {
            value = rand() % (MAX_VALUE + 1);
        } while (!Insert(value, head)); // Insert returns 0 if value is already in the list
    }
} /* populate_list */

void *mutex_thread_func(void *args) {
    struct list_node_s *head = (struct list_node_s *)args;

	int ops_per_thread = m/thread_count;

	int local_member = ops_per_thread* mMember;
	int local_insert = ops_per_thread* mInsert;
	int local_delete = ops_per_thread* mDelete;
    int value;

	while (local_member > 0 || local_insert > 0 || local_delete > 0) {
		float op = rand() % 3;
		value = rand() % MAX_VALUE;

		if (op == MEMBER && local_member > 0) {
			pthread_mutex_lock(&mutex);
			Member(value, head);
			pthread_mutex_unlock(&mutex);
			local_member--;
		} else if (op == INSERT && local_insert > 0) {
			pthread_mutex_lock(&mutex);
            Insert(value, &head);
			pthread_mutex_unlock(&mutex);
			local_insert--;
		} else if (op == DELETE && local_delete > 0) {
			pthread_mutex_lock(&mutex);
			Delete(value, &head);
			pthread_mutex_unlock(&mutex);
			local_delete--;
		}
	}

    return NULL;
}

void *rwlock_thread_func(void *args) {
    struct list_node_s *head = (struct list_node_s *)args;

	int ops_per_thread = m/thread_count;

	int local_member = ops_per_thread* mMember;
	int local_insert = ops_per_thread* mInsert;
	int local_delete = ops_per_thread* mDelete;
    int value;

	while (local_member > 0 || local_insert > 0 || local_delete > 0) {
		float op = rand() % 3;
		value = rand() % MAX_VALUE;
	  
		if (op == MEMBER && local_member > 0) {
			pthread_rwlock_rdlock(&rwlock);
			Member(value, head);
			pthread_rwlock_unlock(&rwlock);
			local_member--;
		} else if (op == INSERT && local_insert > 0) {
			pthread_rwlock_wrlock(&rwlock);
            Insert(value, &head);
			pthread_rwlock_unlock(&rwlock);
			local_insert--;
		} else if (op == DELETE && local_delete > 0) {
			pthread_rwlock_wrlock(&rwlock);
			Delete(value, &head);
			pthread_rwlock_unlock(&rwlock);
			local_delete--;
		}
	}

    return NULL;
}

double perform_operations_serial(struct list_node_s *head) {

    int value;

    int local_member = m* mMember;
	int local_insert = m* mInsert;
	int local_delete = m* mDelete;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < local_member; i++) {
		value = rand() % MAX_VALUE;
		Member(value, head);
    }

    for (int i = 0; i < local_insert; i++) {
        value = rand() % MAX_VALUE;
        Insert(value, &head);
    }

    for (int i = 0; i < local_delete; i++) {
        value = rand() % MAX_VALUE;
        Delete(value, &head);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("    Elapsed time with serial: %.10f seconds\n", elapsed);

	return elapsed;
} /* perform_operations serial */

double perform_operations_mutex(struct list_node_s *head) {

    pthread_t *thread_handles = malloc(thread_count*sizeof(pthread_t));
    printf("    => Thread count: %d\n", thread_count);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < thread_count; i++)
		pthread_create(&thread_handles[i], NULL, mutex_thread_func, (void*) head);

    for (int i = 0; i < thread_count; i++)
        pthread_join(thread_handles[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("    Elapsed time with mutex: %.10f seconds\n", elapsed);
    
    free(thread_handles);

	return elapsed;
} /* perform_operations_mutex */

double perform_operations_rwlock(struct list_node_s *head) {

    pthread_t *thread_handles = malloc(thread_count*sizeof(pthread_t));

    printf("    => Thread count: %d\n", thread_count);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < thread_count; i++)
		pthread_create(&thread_handles[i], NULL, rwlock_thread_func, (void*) head);

    for (int i = 0; i < thread_count; i++)
        pthread_join(thread_handles[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("    Elapsed time with rwlock: %.10f seconds\n", elapsed);

	free(thread_handles);

	return elapsed;
} /* perform_operations_rwlock */

void generate_csv(int num_samples) {

	int num_iterations = num_samples;  // Number of times to run the loop
	double serial, mutex, rwlock;

	struct list_node_s *head = NULL;
	struct list_node_s *list_serial;
	struct list_node_s *list_mutex;
	struct list_node_s *list_rwlock;

	FILE *file = fopen("output.csv", "w");
	if (file == NULL) {
		perror("Unable to open file");
		exit(EXIT_FAILURE);
	}
	fprintf(file, "threads,serial,mutex,rwlock\n");

	populate_list(n, &head);

	for (int i = 1; i<=8 ; i*=2) {
		for (int j = 0; j < num_iterations; j++) {
			thread_count = i;
			list_mutex = CopyList(head);
			list_rwlock = CopyList(head);
			if (thread_count==1) {
				list_serial = CopyList(head);
				serial = perform_operations_serial(list_serial);
			} else {
				serial = 0;
			}
			mutex = perform_operations_mutex(list_mutex);
			rwlock = perform_operations_rwlock(list_rwlock);

			fprintf(file, "%d,%.10f,%.10f,%.10f\n", thread_count, serial, mutex, rwlock);
		}
	}
	FreeList(head);
	FreeList(list_serial);
	FreeList(list_mutex);
	FreeList(list_rwlock);

	fclose(file);
	printf("Data written to output.csv\n");
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		fprintf(stderr, "Usage: %s <number of threads> <nMember> <nInsert> | -gen-csv <number of samples> <nMember> <nInsert>\n", argv[0]);
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-gen-csv") == 0) {
		if (argc != 5) {
			fprintf(stderr, "Usage: %s -gen-csv <number of samples> <nMember> <nInsert>\n", argv[0]);
			return EXIT_FAILURE;
		}
		int num_samples = atoi(argv[2]);
        mMember = atof(argv[3]);
        mInsert = atof(argv[4]);
        mDelete = 1 - mMember - mInsert;
		generate_csv(num_samples);
		return 0;
	}

    srand(time(NULL)); // Seed random number generator

    struct list_node_s *head = NULL;

    thread_count = atoi(argv[1]);
    mMember = atof(argv[2]);
    mInsert = atof(argv[3]);
    mDelete = 1 - mMember - mInsert;

    printf("\n=====================================================================\n");
    printf("|                          Test Details                             |\n");
    printf("=====================================================================\n");
    printf("| %-40s : %21d  |\n", "Number of threads", thread_count);
    printf("| %-40s : %21d  |\n", "n(Initial list size)", n);
    printf("| %-40s : %21d  |\n", "m(Operations to perform)", m);
    printf("---------------------------------------------------------------------\n");
    printf("| %-40s : %21.3f  |\n", "Percentage of mMember", mMember);
    printf("| %-40s : %21.3f  |\n", "Percentage of mInsert", mInsert);
    printf("| %-40s : %21.3f  |\n", "Percentage of mDelete", mDelete);
    printf("=====================================================================\n");

    printf("\n--------------------------------------------------------------------\n");
    printf("\nPopulating list with %d random elements\n", n);
    populate_list(n, &head);
    printf("List populated\n");
    printf("\n--------------------------------------------------------------------\n");

    struct list_node_s *list_serial = CopyList(head);
    struct list_node_s *list_mutex = CopyList(head);
    struct list_node_s *list_rwlock = CopyList(head);

    pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);

    printf("\n=====================================================================\n");
    printf("|                   Performing serial operations                    |\n");
    printf("=====================================================================\n");
    perform_operations_serial(list_serial);
    printf("=====================================================================\n\n");

    printf("\n=====================================================================\n");
    printf("|                Performing operations with mutex                   |\n");
    printf("=====================================================================\n");
    perform_operations_mutex(list_mutex);
    printf("=====================================================================\n\n");

    printf("\n=====================================================================\n");
    printf("|              Performing operations with rwlock                    |\n");
    printf("=====================================================================\n");
    perform_operations_rwlock(list_rwlock);
    printf("=====================================================================\n");

    pthread_rwlock_destroy(&rwlock);
    pthread_mutex_destroy(&mutex);

    FreeList(list_mutex);
    FreeList(list_rwlock);
	FreeList(list_serial);
    FreeList(head);

    return 0;
}