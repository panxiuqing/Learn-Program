#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

#define TRUE 1
#define FALSE 0
#define INVALID -1

#define clear_period 50

#define NUM_INSTRUCTION 320
#define NUM_VIR_PAGE 32

typedef struct {
    int id, vp_id, offset;
} instruction_t;
instruction_t ins[NUM_INSTRUCTION];

typedef struct {
    int pn, pfn, counter, time, is_out;
} page_t;
page_t vir_page[NUM_VIR_PAGE];

TAILQ_HEAD(pf_head, pf_entry) free_head;
struct pf_entry {
    int vp_id, pf_id;
    TAILQ_ENTRY(pf_entry) entries;
};
struct pf_head busy_head;

struct pf_entry pfs[NUM_VIR_PAGE];

int disaffect;

void initialize();
void FIFO();
void LRU();
void OPT();
void LFU();
void CLOCK();

int main()
{
    int S, i;
    srand(10 * getpid());
    S = ((int)rand()) % 320;
    for (i = 0; i < NUM_INSTRUCTION; i += 4) {
        ins[i].id = S;
        ins[i+1].id = ins[i].id + 1;
        ins[i+2].id = ((int)rand()) % ins[i].id;
        ins[i+3].id = ins[i+2].id + 1;
        S = ((int)(rand())) % (318 - ins[i+3].id) + ins[i+3].id + 1;
    }

    for (i = 0; i < NUM_INSTRUCTION; i++) {
        ins[i].vp_id = ins[i].id / 10;
        ins[i].offset = ins[i].id % 10;
    }

    for (i = 4; i <= 4; i++) {
        printf("%2d page frames\t", i);
        FIFO(i);
        LRU(i);
        OPT(i);
        LFU(i);
        CLOCK(i);
        printf("\n");
    }
    return 0;
}

void initialize(int num_pf)
{
    int i;
    disaffect = 0;
    TAILQ_INIT(&free_head);
    TAILQ_INIT(&busy_head);

    for(i = 0; i < NUM_VIR_PAGE; i++) {
        vir_page[i].pn = i;
        vir_page[i].pfn = INVALID;
        vir_page[i].is_out = TRUE;
        vir_page[i].counter = 0;
        vir_page[i].time = -1;
    }

    /*TAILQ_INSERT_HEAD(&free_head, pfs[0], entries);*/
    /*pfs[0].pf_id = 0;*/
    for(i = 0; i < num_pf; i++) {
        TAILQ_INSERT_HEAD(&free_head, &pfs[i], entries);
        pfs[i].pf_id = i;
    }
}

void FIFO(int num_pf)
{
    int i = 0;
    int pf_id = 0;
    initialize(num_pf);
    for (i = 0; i < NUM_INSTRUCTION; i++) {
        printf("%d ", ins[i].vp_id);
        if (vir_page[ ins[i].vp_id ].is_out) {
            printf("\t %d\n", ins[i].vp_id);
            disaffect++;
            if (free_head.tqh_first == NULL) {
                pf_id = (*busy_head.tqh_last)->pf_id;
                vir_page[ pfs[pf_id].vp_id ].is_out = TRUE;
                TAILQ_REMOVE(&busy_head, *busy_head.tqh_last, entries);
                TAILQ_INSERT_HEAD(&free_head, &pfs[pf_id], entries);
            }
            vir_page[ ins[i].vp_id ].is_out = FALSE;
            pf_id = free_head.tqh_first->pf_id;
            TAILQ_REMOVE(&free_head, free_head.tqh_first, entries);
            /*if (busy_head.tqh_first == NULL)*/
                TAILQ_INSERT_HEAD(&busy_head, &pfs[pf_id], entries);
            /*else*/
                /*TAILQ_INSERT_TAIL(&busy_head, &pfs[pf_id], entries);*/
        }
    }
    printf("FIFO:%6.4f ", 1 - (float)disaffect/320);
}

void LRU(int total_pf)
{
}

void OPT(int total_pf)
{
}

void LFU(int total_pf)
{
}

void CLOCK(int total_pf)
{
}
