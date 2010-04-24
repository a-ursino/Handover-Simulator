/* 
**  File:   record.h
**  Author: Maurizio Munafo'
**  Description: Definizioni, strutture dati e prototipi per la gestione 
**               di una lista FIFO di record 
*/

#include "simtime.h"

typedef struct record_unit Record;

struct record_unit
  { 
    Record *next;
    Time  arrival;
	Time  end_call;
	int cell;
	int ch;
	int num_hand;
  };

/* Prototipi */
void in_list(Record **, Record *);	/* Inserisce un'evento nella FEL */
Record *out_list(Record **);		/* Preleva il primo evento dalla FEL */
Record *new_record(void );		/* Ritorna un nuovo evento */
void release_record(Record *);		/* Rilascia un evento */

