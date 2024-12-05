#define _GNU_SOURCE
#include <pthread.h>
#include <assert.h>
#include <stdatomic.h>

#include "my_mutex.h"

void mutex_init(mutex_t *m) {
	// it's not atomic operation
	// but if library provide it why not?
	atomic_init(&m->lock, 0);
	// atomic_init(&m->owner_tid, 0);
}

static int futex(atomic_int *uaddr, int futex_op, int val,
		const struct timespec *timeout, int *uaddr2, int val3) {
	
	return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr, val3);
}

// static int atomic_futex(atomic_int *a_uaddr, int futex_op, int val,
// 		const struct timespec *timeout, int *uaddr2, int val3) {
	
// 	int uadd= atomic_load(a_uaddr);
// 	ret syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr, val3);
// }

void mutex_lock(mutex_t *m) {
	int err;
	int my_tid = gettid();
	while (1) {
		int zero = 0;
		
		// do NUMBER_PREFERABLE_TRYES times to use profit of cas-operations before futex_wait
		// сделать хозяина
		for (int i = 0; i < NUMBER_PREFERABLE_TRYES; ++i) {
			zero = 0;
			// пытаемся изменить значение с 1 на 0
			// &m->lock: адрес переменной блокировки.
			// &zero: предполагаемое текущее значение. 
			// Если реальное значение не равно 0 (= изначальное значение zero), то в zero вернется то, что лежит в lock
			// 0: значение, на которое будет заменено текущее значение, если успешный захват.
			if (atomic_compare_exchange_strong(&m->lock, &zero, my_tid)) {
				return;
			}
		}
		
		int cur_owner_tid = zero;
		// &m->lock: адрес переменной блокировки.
		// FUTEX_WAIT: операция ожидания. Функция объединяет ядро и пользовательское пространство, позволяя потокам ждать, когда значение мьютекса изменится.
		// 0: значение, по которому мы ждем (в данном случае, ожидаем, что мьютекс станет свободным).
		err = futex(&m->lock, FUTEX_WAIT, cur_owner_tid, NULL, NULL, 0); //ждем освобождения мьютекса
		if (err == -1 && errno != EAGAIN) {
			printf("futex FUTEX_WAIT failed: %s\n", strerror(errno));
			abort();
		}
	}
}

/**
 * if success or in case it was not busy, return 0
 * if there was a try to unlock mutex not by owner, mutex won't be unlocked and function returns 1
 * if smth else hapenned, return 2
 */
int mutex_unlock(mutex_t *m) {
	int my_cur_tid = gettid();

	// checking, if it's locked
	// if not, return 0
	if (atomic_load(&m->lock) == 0) {
		return 0;
	}

	// if we are here, it's locked
	
	// пытаемся изменить значение с my_cur_tid на 0
	// если получилось - идем внутрь
	if (atomic_compare_exchange_strong(&m->lock, &my_cur_tid, 0) ) {		
		int err;
		err = futex(&m->lock, FUTEX_WAKE, 1, NULL, NULL, 0); // пробудит 1 поток, который захватился на FUTEX_WAIT
		if (err == -1 ) {
			printf("futex FUTEX_WAKE failed: %s\n", strerror(errno));
			abort();
		}
		return 0;
	}
	//if we're here ...
	return 0;

}

