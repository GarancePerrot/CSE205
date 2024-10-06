#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "cachelab.h"


/*Garance Perrot , garance.perrot@polytechnique.fr*/

/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0;         /* set index bits */
int b = 0;         /* block offset bits */
int E = 0;         /* associativity = nb of lines per set */
char* trace_file = NULL;


int S;// num of sets = 2^s
int B;// block size = 2^b

/* we use 'Least Recently Used' LRU method for evictions: each line_t has an attribute time 
that gets updated to the current time if we hit or miss , and we take the minimum time of all
lines of the current set to chose which one to evict */
int current_time;

/*results variables*/
int hits;
int misses;
int evictions;

/*data structures for cache*/
typedef struct
{
    int valid_bit;
    int tag;
    int time; 
}line_t;

typedef line_t* set_t;

typedef set_t* cache_t;

/*declare the cache */
cache_t cache;

/*initialize cache*/
void initialize_cache();
/* parse each operation in target file */
void parse_input(FILE* openTrace);
/* update cache */
void update_cache(unsigned long address);
/* free cache */
void free_cache();
/* update time */
void update_time();
/*helper function*/
void printUsage(char* argv[]); 




/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 * main - Main routine 
 */
int main(int argc, char* argv[])
{
  char c;
  
  while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
    switch(c){
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      trace_file = optarg;
      break;
    case 'v':
      verbosity = 1;
      break;
    case 'h':
      printUsage(argv);
      exit(0);
    default:
      printUsage(argv);
      exit(1);
    }
  }

  /* Make sure that all required command line args were specified */
  if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
    printf("%s: Missing required command line argument\n", argv[0]);
    printUsage(argv);
    exit(1);
  }
  if(trace_file==NULL) //check that we get a correct pointer for the file
  {
    printf("File Error\n");
    return -1;
  }

  /*initialize variables*/
  current_time=0;
  hits = 0;
  misses = 0;
  evictions = 0;
  S = 1<<s; //S = 2^s
  B = 1<<b; //B = 2^b

  /* open and read the file*/

  FILE *openTrace;
  openTrace = fopen(trace_file, "r");
	if (openTrace != NULL) {
    initialize_cache();
    parse_input(openTrace);
    free_cache();
  }
  printSummary(hits, misses, evictions);
  return 0;
}



void parse_input(FILE* openTrace)
{
    char op;  //operation
    unsigned long address;  //address
    int size;  
    char comma;
    
    while (fscanf(openTrace, " %c %lx%c%u", &op, &address,&comma, &size)>0) //read a character (op), an unsigned long hexadecimal (address), a character (comma), and an unsigned integer (size)
    {
        switch (op)  //different cases according to the operations 
        {
        case 'L': // load Data
            update_cache(address);
            break;
        case 'M':// modify Data
            update_cache(address);

        case 'S': // store Data
            update_cache(address);
            break;

        default:  //we do not care about 'I'
            break;
        }
        update_time();
    }
    
}


void initialize_cache()
{
    cache = (cache_t)malloc(sizeof(set_t) * S);  //allocate memory for cache

    for(int i=0; i<S; ++i)   //for each set:
    {
        cache[i] = (set_t)malloc(sizeof(line_t) * E); //allocate memory for each line

        for(int j=0; j<E; ++j) //for each line:
        {
            cache[i][j].tag = -1;  //initialize the tag to -1
            cache[i][j].time = -1; //initialize time to -1
            cache[i][j].valid_bit = 0;// initialize valid_bit to invalid
        }
    }
}

void free_cache()
{
    for(int i=0; i<S; ++i) //for each set:
        free(cache[i]); //free each line
    free(cache);  //free the cache structure
}

void update_time()
{
    current_time ++;
}

void update_cache(unsigned long address)
{
    long current_tag = (address>>(b+s));
    long current_set = (address>>b) & ((0xFFFFFFFF)>>(64-s)); //mask to extract the set bits
    
    for(int i=0; i<E; ++i)// search in the current set:
    {   
        if((cache[current_set][i].tag == current_tag) && cache[current_set][i].valid_bit==1) // if we hit (matched tag and valid bit)
        {
            hits ++;   // increment hit variable
            cache[current_set][i].time = current_time; //update the line's time
            return;
        }
    }
//if we do not hit: 
    for(int i=0; i<E; ++i)//iterate over the lines of the set 
    {
        if(cache[current_set][i].valid_bit == 0)//  if the bit is invalid, we have a miss on an empty line
        {
          /* update the line's tag, time and set the bit to valid*/
            cache[current_set][i].tag = current_tag;  
            cache[current_set][i].time = current_time;
            cache[current_set][i].valid_bit = 1;
            misses ++; //increment miss variable
            return;
        }
    }

    // otherwise we need eviction: we use LRU to chose which line to evict:
    evictions ++; //increment eviction variable
    misses ++;   //eviction implies a miss

    int min_time = 10000;
    int min_line;  //the line which will be evicted
    for(int i=0; i<E; ++i) //iterate over all lines
    {   
        if(cache[current_set][i].time<min_time) //search only for lines with small time attributes 
        {
            min_time = cache[current_set][i].time;
            min_line = i;
        }
    }
    /* Now min_line is the line of the set with the minimum time attribute, 
    meaning it is the least recently used, we modify the tag and time attributes*/
    cache[current_set][min_line].tag = current_tag;  
    cache[current_set][min_line].time = current_time;
} 