#define _GNU_SOURCE
#include <pthread.h>
#include <assert.h>
#include <fcntl.h>
#include <stdatomic.h>

#include "my_spinlock.h"




void spinlock_init(spinlock_t *s) {
	s->lock = 1;
}

void spinlock_lock(spinlock_t *s) {
	while (1) {
		int one = 1;

		// пытаемся изменить значение с 1 на 0
		// &m->lock: адрес переменной блокировки.
		// &one: предполагаемое текущее значение.
		// 0: значение, на которое будет заменено текущее значение, если успешный захват.
		if (atomic_compare_exchange_strong(&s->lock, &one, 0)) {
			break;
		}
	}
}

void spinlock_unlock(spinlock_t *s) {
	const int zero = 0;
	// если получилось - мы молодцы
	// если не получилось - значит, он и так был свободен
	atomic_compare_exchange_strong(&s->lock, &zero, 1);
}
