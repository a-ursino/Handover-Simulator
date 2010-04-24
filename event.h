/*
**  File:   event.h
**  Author: Maurizio Munafo'
**  Description: Definizioni, strutture dati e prototipi per la gestione
**               della lista degli eventi
*/

#include "simtime.h"

typedef struct event_unit Event;

/*
  L'Event List e' una lista circolare bilinkata, referenziata dal puntatore
  all'ultimo elemento
*/
struct event_unit
      {
	Event *next;	/* Evento successivo */
	Event *prev;    /* Evento precedente */
	Time  time;     /* Istante in cui deve avvenire la schedulazione
			   dell'evento */
	int   type;     /* Tipo di evento, da definire volta per volta */

    int cell;		/*the id of che cell*/
	int channel;	/*the id of the channnell for this cell*/
      };

/* Prototipi */
void insert_event(Event **, Event *);	/* Inserisce un'evento nel FES */
Event *get_event(Event **);		/* Preleva il primo evento dal FES */
Event *new_event(void );		/* Ritorna un nuovo evento */
void release_event(Event *);		/* Rilascia un evento */
