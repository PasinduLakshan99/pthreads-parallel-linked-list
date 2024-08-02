#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"

#define INSERT 0
#define DELETE 1
#define MEMBER 2

#define MAX_VALUE 65535 // 2^16 - 1

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

int n = 10000, m = 100000;
float mMember = 0.99, mInsert = 0.005, mDelete = 0.005;
int thread_count;

void PrintList(struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;
    while (curr_p != NULL) {
        printf("%d -> ", curr_p->data);
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

void *serial_thread_func(void *args) {
    struct list_node_s *head = (struct list_node_s *)args;
    int value;

    int local_member = m* mMember;
    int local_insert = m* mInsert;
    int local_delete = m* mDelete;

    printf("    => local_member: %d, local_insert: %d, local_delete: %d\n", local_member, local_insert, local_delete);

    while (local_member > 0 || local_insert > 0 || local_delete > 0) {
        float op = rand() % 3;
        value = rand() % MAX_VALUE;
      
        if (op == MEMBER && local_member > 0) {
            Member(value, head);
            local_member--;
        } else if (op == INSERT && local_insert > 0) {
            Insert(value, &head);
            local_insert--;
        } else if (op == DELETE && local_delete > 0) {
            Delete(value, &head);
            local_delete--;
        }
    }

    return NULL;
}

void *mutex_thread_func(void *args) {
    struct list_node_s *head = (struct list_node_s *)args;
	
	
	int ops_per_thread = m/thread_count;

	int local_member = ops_per_thread* mMember;
	int local_insert = ops_per_thread* mInsert;
	int local_delete = ops_per_thread* mDelete;
    int value;

    printf("    => local_member: %d, local_insert: %d, local_delete: %d\n", local_member, local_insert, local_delete);

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

    printf("    => local_member: %d, local_insert: %d, local_delete: %d\n", local_member, local_insert, local_delete);

	while (local_member > 0 || local_insert > 0 || local_delete > 0) {
		float op = rand() % 3;
		value = rand() % MAX_VALUE;
	  
		if (op == MEMBER && local_member > 0) {
			// pthread_rwlock_rdlock(&rwlock);
			Member(value, head);
			// pthread_rwlock_unlock(&rwlock);
			local_member--;
		} else if (op == INSERT && local_insert > 0) {
			// pthread_rwlock_wrlock(&rwlock);
            Insert(value, &head);
			// pthread_rwlock_unlock(&rwlock);
			local_insert--;
		} else if (op == DELETE && local_delete > 0) {
			// pthread_rwlock_wrlock(&rwlock);
			Delete(value, &head);
			// pthread_rwlock_unlock(&rwlock);
			local_delete--;
		}
	}

    return NULL;
}

void perform_operations_serial(struct list_node_s *head) { 
    pthread_t thread_handle = (pthread_t)malloc(sizeof(pthread_t));
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_create(&thread_handle, NULL, serial_thread_func, (void*) head);
    pthread_join(thread_handle, NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("    Elapsed time with serial: %.10f seconds\n", elapsed);
} /* perform_operations serial */

void perform_operations_mutex(struct list_node_s *head) {

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
    
} /* perform_operations_mutex */

void perform_operations_rwlock(struct list_node_s *head) {

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
  
} /* perform_operations_rwlock */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
        exit(1);
    }
    srand(time(NULL)); // Seed random number generator

    struct list_node_s *head = NULL;

    thread_count = atoi(argv[1]);

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

    struct list_node_s *list_mutex = CopyList(head);
    struct list_node_s *list_rwlock = CopyList(head);

    pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);

    printf("\n=====================================================================\n");
    printf("|                   Performing serial operations                    |\n");
    printf("=====================================================================\n");
    perform_operations_serial(head);
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
    FreeList(head);

    return 0;
}