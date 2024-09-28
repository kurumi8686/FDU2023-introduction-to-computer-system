#pragma once

typedef unsigned int un;
typedef struct {
    int valid_bits; //有效位
    un target; //标记位
    int time; //时间戳
}cache_line;
cache_line** cache = NULL;

// cache parameters
int numSet;  //s
int associativity;  //E
int blockSize; //b
char filePath[100];
int verbose = 0;
int allSet;  //S = 2^s

// final results
int hits = 0;
int misses = 0;
int evictions = 0;

un set_index = 0;  //组索引
un target_index = 0;  //目标值索引

// will be set in getopt() function
extern char *optarg;

// you can define functions here
void usage();
void parseline(int argc, char **argv);
//构造和析构
void init();
void dele();
//更新高速缓存
void update_cache(un address);
void exist_time_increase();
