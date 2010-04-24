#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "record.h"
#include "simtime.h"
#include <math.h>
//code event

//global constant


long seme1 = 14123451,
     seme2 =57645805;
	 
Time current_time;


 void schedule(int type, Time time,int station,int cell, int channell){
   Event *ev;
   ev=new_event();
   ev->type=type;
   ev->time=time;
   ev->cell=cell;
   ev->channell=channell;
   insert_event(&event_list,ev);
   return;
}

void get_input(char *format,void *variable){
    static char linebuffer[255];
    char *pin;

    fgets(linebuffer, 255, stdin);	/*  Gets a data from stdin without  */
    pin = strrchr (linebuffer, '\n');	/*  flushing problems		    */
    if (pin!=NULL) *pin = '\0';

    sscanf(linebuffer,format,variable);	/* Read only the beginning of the   */
					/* line, letting the user to write  */
					/* all the garbage he prefer on the */
					/* input line.			    */
}

int main(){



return 0;
}