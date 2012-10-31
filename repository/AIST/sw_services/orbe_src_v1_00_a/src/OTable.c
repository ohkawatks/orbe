#include	<stdio.h>
#include	<stdlib.h>
#include	"Giop.h"
#include	"Orbe.h"
#include	"CORBA_Object.h"
#include	"OTable.h"

struct Olist {
	unsigned int	pos;	/* pointing next room */
	CORBA_Object	table[MAX_OBJECTS];
};

static struct Olist repository;

int OTable_add(CORBA_Object o)
{
	if (repository.pos <= MAX_OBJECTS - 1) {
		repository.table[repository.pos++] = o;
		return 0;
	}
	DEBUG("OTableadd : table has no slots.");
	return -1;
}

int OTable_remove(CORBA_Object o)
{
	int i = 0;
	while (i < repository.pos) {
		if (o == repository.table[i])  {
			int j = 0;
			for (j = i; j < repository.pos; j++) {
				repository.table[j] = repository.table[j + 1];
			}
			repository.table[j] = NULL;
			repository.pos--;
			DEBUG("OTable_remove : removed an object.");
			return 0;
		}
	}
	return 0;
}
	
CORBA_Object OTable_current()
{
	if (repository.pos > 0) {
		return repository.table[repository.pos - 1];
	}
	DEBUG("OTable_current : table has no objects.");
	return NULL;
}

int OTable_has(CORBA_Object o)
{
	int i = 0;
	while (i < repository.pos) {
		if (o == repository.table[i++])  {
			return 1;
		}
	}
	return 0;
}
