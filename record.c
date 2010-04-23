/* 
**  File:   record.c
**  Author: Maurizio Munafo'
**  Description: Funzioni per la gestione della lista FIFO
*/

#include <stdio.h>
#include <stdlib.h>
#include "record.h"

Record *free_records = NULL;

void in_list(Record **handle, Record *element)
{
 if (*handle==NULL)
  {
    *handle = element;
    element->next = element;
  }
 else
  {
    element->next = (*handle)->next;
    (*handle)->next = element;
    *handle = element;
  }
}

Record *out_list(Record **handle)
{
  Record *p;
  p = *handle;
  if (p==NULL) return(NULL);
  if (p->next==p)
    *handle = NULL;
  else
   {
    p = (*handle)->next;
    (*handle)->next = p->next;
   }
  return(p);
}

Record *new_record(void)
{
 extern Record *free_records;
 Record *p;
 int i;
 if (free_records==NULL)
  {
    if ((p=(Record *)malloc(sizeof(Record)))==NULL)
         { printf("Error  : Memory allocation error\n");
           exit(1);
         }
    in_list(&free_records,p);
  }
 return(out_list(&free_records));
}

void release_record(Record *element)
{
 extern Record *free_records;
 in_list(&free_records,element);
}
