#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csim.h"
#include <getopt.h>


void init() {
    //��ʼ��cache[S][E]  S--set������ E--ÿ��set�е�cache line��
    cache = (cache_line**)malloc(sizeof(cache_line*) * allSet);
    for (int i = 0; i < allSet; i++)
    {
        cache[i] = (cache_line*)malloc(sizeof(cache_line) * associativity);
    }
    for (int i = 0; i < allSet; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            cache[i][j].valid_bits = 0;
            cache[i][j].target = 0; //�޵�ַ
            cache[i][j].time = -1;
        }
    }
}
void dele()
{
    for (int i = 0; i < allSet; i++)
    {
        free(cache[i]);
    }
    free(cache);
}
// print usage info
void usage()
{
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(1);
}
// parse command line and get the parameters
void parseline(int argc, char **argv)
{
    int opt;
    int num = 0;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            usage();
            break;
        case 'v':
            verbose = 1;  
            break;
        case 's':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            numSet = num; 
            allSet = numSet;
            break;
        case 'E':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            associativity = num;
            break;
        case 'b':
            num = atoi(optarg);
            if (num == 0 && optarg[0] != '0')
            {
                printf("./csim: Missing required command line argument\n");
                usage();
            }
            blockSize = num; 
            break;
        case 't':
            strcpy(filePath, optarg);
            break;
        case ':':
            printf("./csim: Missing required command line argument\n");
            usage();
            break;
        case '?':
            usage();
            break;
        default:
            printf("getopt error");
            exit(1);
            break;
        }
    }
}

//LUR���Ը��¸��ٻ���
void update_cache(un address)
{
    set_index = (address / blockSize) % numSet;
    target_index = address / (blockSize * numSet);

    for (int i = 0; i < associativity; i++)
    {
        if (cache[set_index][i].target == target_index
            && cache[set_index][i].valid_bits == 1)
        {
            //���У���ʵ����target���϶��Ѽ���
            cache[set_index][i].time = 0;
            hits++;
            if (verbose) printf(" hit");
            return;
        }
    }
    //û������
    misses++;
    if (verbose) printf(" miss");
    for (int i = 0; i < associativity; i++)
    {
        if (cache[set_index][i].valid_bits == 0)
        {
            cache[set_index][i].target = target_index;
            cache[set_index][i].valid_bits = 1;
            cache[set_index][i].time = 0;
            return;
        }
    }
    //���ٻ�����������û��hit����Ҫ��̭һ�����û�õġ�
    //�ҳ����ʱ�䣬Ҳ���Ǵ������û���ã�LRU
    int memotime = -1; int memoi = 0;
    for (int i = 0; i < associativity; i++)
    {
        if (cache[set_index][i].time > memotime)
        {
            memotime = cache[set_index][i].time;
            memoi = i;
        }
    }
    evictions++;
    if (verbose) printf(" eviction");
    cache[set_index][memoi].target = target_index;
    cache[set_index][memoi].time = 0; //�´���
    cache[set_index][memoi].valid_bits = 1;
}

void exist_time_increase()
{
    for (int j = 0; j < associativity; j++)
    {
        if (cache[set_index][j].valid_bits) cache[set_index][j].time++;
    }
}

int main(int argc, char *argv[])
{
    parseline(argc, argv);
    // Simulate();
    init();
    FILE* file = fopen(filePath, "r");

    char select;
    un address;
    int size;
    while (fscanf(file, " %c %x,%d", &select, &address, &size) > 0)
    {
        address &= 0xffffffff;
        if (verbose) printf("%c %x,%d", select, address, size);
        switch (select)
        {
            case 'M': //�޸����ݲ������൱���ȶ���д������
                update_cache(address);
                exist_time_increase();
                update_cache(address);
                exist_time_increase();
                break;
            case 'L': //��ȡ���ݲ�����һ��
                update_cache(address);
                exist_time_increase();
                break;
            case 'S': //д�����ݲ�����һ��
                update_cache(address);
                exist_time_increase();
                break;
        }
        printf("\n");
    }
    printSummary(hits, misses, evictions);
    fclose(file);
    dele();
    return 0;
}
