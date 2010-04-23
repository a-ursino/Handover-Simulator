/*	  
**  Module  : random.c
**  Version : 1.2
**  Date    : 25/03/97
*/	  

#include <stdio.h>
#include <math.h>

#define MODULE  2147483647
#define A       16807
#define LASTXN  127773
#define UPTOMOD -2836
#define RATIO   0.46566128e-9            /* 1/MODULE */

/*	  
**  Function: long rnd32(long seed) 
**  Remarks : Congruential generator of pseudorandom sequences of numbers
**            uniformly distributed between 1 and 2147483646, using the con-
**            gruential relation: Xn+1 = 16807 * Xn  mod  2147483647 . 
**            The input to the routine is the integer Xn, while the returned 
**            integer is the number Xn+1.
*/
long rnd32(long seed)
{
 long times, rest, prod1, prod2;

 times = seed / LASTXN;
 rest  = seed - times * LASTXN;
 prod1 = times * UPTOMOD;
 prod2 = rest * A;
 seed  = prod1 + prod2;
 if (seed < 0) seed = seed + MODULE;
 return (seed);
}

/*
**  Function: double uniform(double a, double b, long *seed)
**  Return  : a value uniformly distributed between 'a' and 'b'
**  Remarks : The value of '*seed' is changed
*/	  
double uniform(double a, double b, long *seed)
{
 double u;
 *seed = rnd32 (*seed);
 u = (*seed) * RATIO;
 u = a + u * (b-a);
 return (u);
}

/*
**  Function: double uniform01(long *seed)
**  Return  : a value uniformly distributed between 0.0 and 1.0
**  Remarks : The value of '*seed' is changed
*/	  
double uniform01(long *seed)
{
 double u;
 *seed = rnd32 (*seed);
 u = (*seed) * RATIO;
 return (u);
}

/*	  
**  Function: double negexp(double mean, long *seed)
**  Return  : a value exponentially distributed with mean 'mean'
**  Remarks : The value of '*seed' is changed
*/	  
double negexp (double mean, long *seed)
{
 double u;
 *seed = rnd32 (*seed);
 u = (*seed) * RATIO;
 return ( - mean*log(u) );
}

/*
**  Function: double pareto(double alpha , double kappa, long *seed)
**  Return  : a value pareto distributed with paremeters k and alpha
**  Remarks : The value of '*seed' is changed
*/
    /*  -MMM- Qui forse occorre passare come parametri alpha e il valor 
        medio... 
    */
double pareto(double alpha , double kappa, long *seed)
{
 double u;
 *seed = rnd32 (*seed);
 u = (*seed) * RATIO;
 return ( kappa/pow(u,(1./alpha)) );
}


/*
**  Function: double pareto_m(double alpha , double kappa, long *seed)
**  Return  : a value pareto distributed with paremeter alpha and mean 'mean'
**  Remarks : The value of '*seed' is changed. Valid only if alpha > 1.0
*/
double pareto_m(double alpha , double mean, long *seed)
{
 double u;
 double kappa;
 if (alpha <= 1.0)
   { printf("Error pareto_m(): alpha must be greater than 1.0\n");
     return(mean);
   }
 *seed = rnd32 (*seed);
 u = (*seed) * RATIO;
 kappa = (alpha-1.0)*mean/alpha;
 return ( kappa/pow(u,(1./alpha)) );
}

/*	  
**  Function: int poisson(double alpha,long *seed)
**  Return  : The number of users arrived, according to a poisson process 
**            with rate 'alpha' user/slot, in a slot
**  Remarks : The value of '*seed' is changed
*/	  
int poisson(double alpha,long *seed)
{
 int n = 0;
 double pn,lim;
 double prob;

 lim = pn = exp(-alpha);
 prob = uniform01(seed);
 while(prob>lim)
  { n++;
    pn *= alpha/n;
    lim += pn;
  }
 return(n);
}

/*	  
**  Function: int geometric0(double mean,long *seed)
**  Return  : a random value distributed geometrically with average 'mean',
**            starting from 0. (0 w.p. 1-p, 1 w.p. p(1-p), etc.) 
**  Remarks : The value of '*seed' is changed
*/	  
int geometric0(double mean,long *seed)
{
 int n;
 double prob,status;

 status = mean/(1.0+mean);	    /* E[X] = p/(1-p) -> p = E[X]/(1+E[X])  */
 prob = uniform01(seed);	    /* 1-p = prob. di avere n = 0 	    */
 n = (int)floor(log(1-prob)/log(status));
 return(n);
}

/*	  
**  Function: int geometric1(double mean,long *seed)
**  Return  : a random value distributed geometrically with average 'mean',
**            starting from 1. (1 w.p. 1-p, 2 w.p. p(1-p), etc.) 
**  Remarks : The value of '*seed' is changed
*/	  
int geometric1(double mean,long *seed)
{
 int n;
 double prob,status;

 status = (mean-1)/mean;	    /* E[X] = 1/(1-p) -> p = (E[X]-1)/E[X]  */
 prob = uniform01(seed);	    /* 1-p = prob. di avere n = 1 	    */
 n = 1 + (int)floor(log(1-prob)/log(status));
 return(n);
}

/*	  
**  Function: int geometric_trunc1(double mean,int max_len,long *seed)
**  Return  : a random value distributed geometrically with average 'mean',
**            starting from 1.
**            The distribution is truncated at the value 'max_len'.
**  Remarks : The value of '*seed' is changed
*/	  
int geometric_trunc1(double mean,int max_len,long *seed)
{
  /* These function returns a number distributed quasi-geometrically with   */
  /* average mean and maximum value 'max_len'.				    */
  /* There are some problems with the calculation. Here we explain the way  */
  /* the numbers are calculated.					    */
  /* The mean value of the random variable is:				    */
  /*                                                                        */
  /*			 Sum(i*p^(i-1),i=1..N)				    */
  /*		  E[x] = --------------------- = m			    */
  /*			  Sum(p^(i-1),i=1..N)				    */
  /* i.e.								    */
  /*		         p^N ( Np - N - 1) + 1				    */
  /*		     m = ---------------------		(1)		    */
  /*			     (1-p)(1-p^N)				    */
  /*    								    */
  /* where p is the transition probability in the Markov chain of the model.*/
  /*                                                                        */
  /* We need the value of p as a function of m and N. The only solution is  */
  /* to solve the equation (1) in the variable p using the Newton method,   */
  /* i.e.                                                                   */
  /*           p' = p - f(p)/f'(p)                                          */
  /* being p' the value of p at the step i+1, p the value at the step i,    */
  /* f(p) is (1) and f'(p) is df(p)/dp.                                     */
  /* In our calculations, we use:                                           */
  /*                                                                        */
  /*    f(p)  = p^N * ((m-N)p + N - m + 1) - mp + m -1                      */
  /*    f'(p) = (m-N) p^N + N p^(N-1)((m-N)p + N - m + 1) - m               */
  /*                                                                        */
  /* and the value  p = (m-1)/m  as starting point. This is the value of    */
  /* p when N tends to infinity.					    */
  /*                                                                        */
  /* This value of p is used to find the number n to be returned.  A random */
  /* variable q uniformly distributed in (0,1) is extracted, so if	    */
  /*                                                                        */
  /*               sum(p^(i-1),i=1..n)    1 - p^n                           */
  /*          q = --------------------- = -------                           */
  /*               sum(p^(i-1),i=1..N)    1 - p^N                           */
  /*                                                                        */
  /* we found that                                                          */
  /*                                                                        */
  /*		  |~  log(p^N * q - q - 1) ~|				    */
  /*	      n = |   --------------------  |				    */
  /*		  |         log(p)	    |				    */
  /*                                                                        */
  /* In order to avoid large computations, the previous values of 'mean'    */
  /* and 'max_len' are recorded, so if the function is called twice or more */
  /* times consecutively with the same parameters, the previously computed  */
  /* value of p can be used.						    */
  /*                                                                        */
  /* In the code, there is the corrispondence:                              */
  /*		p     -> status						    */
  /*		m     -> mean						    */
  /*		N     -> max_len					    */
  /*		q     -> prob						    */
  /*		f(p)  -> f_p						    */
  /*		f'(p) -> df_p						    */
  /* between the symbols used in this comment and the variables names.      */

 int n;
 double prob,f_p,df_p,
        temp_status,temp_res,len;
 static double status   = 0.0,
               old_mean = 0.0,
               status_N = 0.0;
 static int    old_max  = 0;
 
 if (mean>=(double)max_len)
   { printf("Error Calling Geometric_Trunc1()\n");
     return(1);
   }
 if ( fabs(old_mean-mean)>1e-5 || old_max!=max_len)
  {
    len = (double)max_len;
    temp_status = (mean-1)/mean;
    do
     {
      status = temp_status;
      status_N = pow(status,len);
      temp_res = (mean-len) * status + len-mean + 1;
      f_p = status_N * temp_res - mean * status + mean - 1;
      df_p = (mean-len) * status_N + len * status_N * temp_res / status - mean;
      temp_status = status - f_p / df_p;
     }
    while (fabs(temp_status-status)>1e-9);
    status = temp_status;
    status_N = pow(status,len);
    old_mean = mean;
    old_max = max_len;
  } 

 prob = uniform01(seed);
 n = 1 + (int)floor(log(1 - prob + prob * status_N)/log(status));
 return(n);
}

/*	  
**  Function: int trunc_exp(double mean,long length,long *seed)
**  Return  : mean and length are in bytes;
**  Remarks : The value of '*seed' is changed
*/	  
int trunc_exp(double mean,long length,long *seed)
{
 double len,prob;

 *seed = rnd32 (*seed);
 prob = (*seed) * RATIO;
 /* len =  - 8*mean*(log(*seed)-21.4875626); */
 len = -8*mean*log(prob);
 len = (len>length*8.) ? length : len/8.;
 return( (int)len == 0 ? 1 : (int)len );
}

