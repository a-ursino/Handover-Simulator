#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "record.h"
#include "simtime.h"
#include <math.h>
//code event
#define CALL 1
#define HANDOVER 2
#define END_CALL 3
//global constant
#define NUM_CELL 4
#define NUM_CH	3
#define MAX_H 8
#define MAX_TIME_CALL 180;
#define NUM_ADJ 2
//global variables
long seme1 = 14123451,
     seme2 =57645805;
	 
Time current_time;
Event *event_list = NULL;
FILE *file;
double lambdaLoad,stayMeanTime;
Record *calls[NUM_CELL][NUM_CH];
int channells[NUM_CH];
int adj_cells[NUM_CELL][NUM_ADJ]
int histo[MAX_H];


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


/*return the first free channell for this cell*/
int find_free_ch(int cell){
	int i=0;
	for(i=0;i<NUM_CH;i++){
		if(calls[cell][i]==NULL){
			return i;
		}
	}
	
}

int remove_event(Event **last,int type_event,int cell,int ch){
	//loop on events list
	Event *evt=NULL,*evt_temp=NULL;
	if(*last==NULL){
		//empty list	
		return;
	}
	
}

int call(int cell, int ch){
tot_call++;
delta=poisson(1/lambdaLoad,&seme1);
schedule(CALL,current_time+delta,cell,0);
Record *rec;
double handover_time,end_call_time;

int ch=0;
if(channells[cell]==NUM_CH){
	//drop the call
	drop_calls++;
}else{
	ch=find_free_ch(cell);
	rec=new_record();
	rec->cell=cell;
	rec->ch=ch;
	rec->num_hand=0;
	channells[cell]++;
	calls[cell][ch]=rec;
	
	handover_time=poisson(stayMeanTime,&seme2);
	end_call_time=poisson(MAX_TIME_CALL,&seme2);
	
	if((current_time+handover_time)<(current_time+end_call_time)){
		//schedule a handover
		schedule(HANDOVER,current_time+handover_time,cell,ch);
	}
	schedule(END_CALL,current_time+end_call_time,cell,ch);
}

return 0;
}


int handover(int cell_src,int ch_src){
	double handover_time;
	int cell_dst;
	Record *rec;
	rec=calls[cell_src][ch_src];
	calls[cell_src][ch_src]=NULL;
	channells[cell_src]--;
	rec->num_hand++;
	//find a new cell among the adj cells
	cell_dst=adj_cells[cell_src][(int)floor(uniform01(&seme)*NUM_ADJ)]	
	if(channells[cell_dst]==NUM_CH){
		//drop_the call on handover
		drop_call_on_hand++;
		//remove the end_call event for this call 
		remove_event(&event_list,END_CALL,cell_src,ch_src);
		release_record(rec);
		return;
	}
	channells[cell_dst]++;
	calls[cell_dst][ch_dst]=rec;
	handover_time=poisson(stayMeanTime,&seme2);
	if(current_time+handover_time<rec->end_call){
		cell_src=cell_dst;
		ch_src=ch_dst;
		schedule(HANDOVER,current_time+handover_time,cell_src,ch_src)
	}

}

int end_call(int cell,int ch){
	Record *rec;
	channells[cell]--;
	rec=calls[cell][ch];
	calls[cell][ch]=NULL;
	tot_call_ok++;
	rec->num_hand
	histo[rec->num_hand<MAX_H:rec->num_hand?MAX_H-1]++;
	release_record(rec);
}


void results(void){
	
	printf("Total calls:%d\n",tot_calls);
	printf("Total blocked calls:%d\n",drop_calls);
	printf("Blocked calls probability:%f\n",drop_calls*1.0/tot_calls);
	printf("Total interrupted calls:%d\n",drop_calls_on_hand);
	printf("Interrupted calls probability:%f\n",drop_calls_on_hand*1.0/tot_calls);
	
	printf("Histogram\n");
	for(i=0;i<MAX_H;i++){
		printf("handover %d call %d	\n",i,histo[i]);
	}
	exit(0);
}

int main(){
int i,j;
Event *ev;
Time maximum,last_event_time;
double delta=0;
printf("Insert the lambda value for the traffic load\n");
get_input("%lf",&lambdaLoad);
printf("Insert the mean time value for cell's stay\n");
get_input("%lf",&stayMeanTime);
printf("Insert the simulation's max time \n");
get_input("%lf",&maximum);
printf("The lambda value for the traffic load is %f\n",lambdaLoad);
printf("The mean time value for cell's stay is %f\n",stayMeanTime);
printf("The simulation's max time is %f\n",maximum);

file = fopen("simulationData.csv","w");
//fprintf(file,"# %s","Time,\tArrivo_Pkt,\tEnd_Tx,\tArrivo_token,\tDropped_Pkt\n");

//initialize all the variables and the events
for(i=0;i<NUM_CELL;i++){
	channells[i]=0;
	for(j=0;j<NUM_CH;j++){
		calls[i][j]=NULL;
	}
	
}
drop_calls=0;//drop the call on start call
drop_calls_on_hand=0;//drop the call while trying to make an handover
tot_calls=0;
tot_calls_ok=0;
for(i=0;i<NUM_CELL;i++){
	delta= negexp(1/lambdaLoad,&seme1)
	schedule(CALL,current_time+delta,i,0); 
}
//initialize the matrix with the adjancent cells
//TO DO

//end initialize all the variables and the events

while (current_time<maximum){
	//remove the event from the event_list
    ev = get_event(&event_list);
    last_event_time = current_time;
    current_time = ev->time;
    //"Time,Arrivo_Pkt,End_Tx,Arrivo_token"
    switch (ev->type){
       case CALL:
              //fprintf(file,"%f\t 1 \t\t NaN \t NaN \t NaN \t NaN\n",ev->time);
              call(ev->cell,0);
		      break;
       case HANDOVER:
              //fprintf(file,"%f\t NaN \t NaN \t 0.5 \t NaN \t NaN\n",ev->time);
              handover(ev->cell,0);
		      break;
		case END_CALL:
            //fprintf(file,"%f\t NaN \t 3 \t\t NaN \t NaN \t NaN\n",ev->time);
		    end_call(ev->cell,e->ch);
			break;
       default:       printf("Orrore!!!\n");
		      exit(1);
     }
	 //destroy the event removed from the event_list
    release_event(ev);
  } //end event loop
   results();
   fclose(file);
return 0;
}