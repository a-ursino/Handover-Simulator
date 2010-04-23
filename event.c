/*
**  File:   Event.c
**  Author: Maurizio Munafo'
**  Description: Funzioni per la gestione della lista degli eventi
*/

#include <stdio.h>
#include <stdlib.h>
#include "event.h"

Event *free_events = NULL;  /* Puntatore alla mia Free List di eventi */

/*
**  Funzione:  void insert_event(Event **last, Event *elem)
**  Parametri: Event **last  - Puntatore di riferimento alla lista (l'ultimo
**                             elemento)
**             Event  *elem  - Record di tipo Event da inserire
**  Ritorno:   nessuno
**  Descrizione: Inserisce 'elem' nella Event List referenziata da **last
*/
void insert_event(Event **last, Event *elem)
{
  Event *p;
  if (elem==NULL) return;       /* Non c'e' nulla da inserire */

  /* L'Event List e' vuota: elem diventa l'Event List */
  if ((*last)==NULL)
    {
       (*last) = elem;
       (*last)->next = elem;
       (*last)->prev = elem;
       return;
    }

  /*
  ** elem e' schedulato dopo l'ultimo evento presente in lista:
  ** viene inserito in fondo e diventa il nuovo last
  */
  if (elem->time >= (*last)->time)
    {
       elem->prev = (*last);
       elem->next = (*last)->next;
       ((*last)->next)->prev = elem;
       (*last)->next = elem;
       (*last) = elem;
    }
  else
  /*
  ** cerco la posizione di elem, procedendo dal fondo della lista
  ** ovvero seguendo il puntatore prev
  */
    {
       p = (*last);
       while (elem->time < p->time && p->prev!=(*last))
         p = p->prev;
	   if (elem->time < p->time)
         {
		   /* p e' il primo elemento schedulato dopo elem:
		      inserisco elem subito prima */
           elem->prev = p->prev;
           elem->next = p;
           (p->prev)->next = elem;
           p->prev = elem;
         }
       else
         {
		   /* elem deve essere il primo evento della lista:
		      inserisco elem dopo p (che e' *last) */
           elem->prev = p;
           elem->next = p->next;
           (p->next)->prev = elem;
           p->next = elem;
         }
    }
  return;
}

/*
**  Funzione:  Event *get_event(Event **last)
**  Parametri: Event **last  - Puntatore di riferimento alla lista (l'ultimo
**                             elemento)
**  Ritorno:   Puntatore all'evento estratto
**  Descrizione: Ritorna il prossimo evento da schedulare, estraendolo dalla
**               cima della lista. Torna NULL se l'Event List era vuota
*/
Event *get_event(Event **last)
{
  Event *p;
  if ((*last)==NULL) return NULL;   /* L'Event List e' vuota */
  if ((*last)->next==(*last))
    {
	  /* Sto estraendo l'unico elemento della Event List */
      p = (*last);
      (*last) = NULL;
    }
  else
    {
	  /* Estraggo il primo elemento (successore di last) */
      p = (*last)->next;
      (p->next)->prev = (*last);
      (*last)->next = p->next;
    }
  return p;
}

/*
**  Funzione:  Event *new_event(void)
**  Parametri: Nessuno
**  Ritorno:   Puntatore ad un nuovo evento
**  Descrizione: Ritorna un nuovo evento.
se c'è almeno un'elemento nella free-list(lista di riciclo)
lo preleva da li, altrimenti lo crea da zero
**  Effetto collaterale: Inserisce o rimuove elementi dalla Free List globale
*/
Event *new_event(void)
{
 extern Event *free_events;
 Event *p;
 int i;
 if (free_events==NULL)
  {
    if ((p=(Event *)malloc(sizeof(Event)))==NULL)
      { printf("Error  : Memory allocation error\n");
        exit(1);
      }
    p->time = (Time)0;
    insert_event(&free_events,p);
  }
 return(get_event(&free_events));  /* Ritorno il primo elemento della
                                      Free List */
}

/*
**  Funzione:  void release_event(Event *elem)
**  Parametri: Evento da rilasciare
**  Ritorno:   Nessuno
**  Descrizione: Rilascia la struttura dati dell'evento, inserendola nella
**               Free List(per il riciclo della memoria)
*/
void release_event(Event *elem)
{
 extern Event *free_events;
 elem->time = (Time)MAX_TIME;    /* Cosi' il record va subito in fondo alla
                                    Free List */
 insert_event(&free_events,elem);
}
