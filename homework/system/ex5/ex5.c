#include <stdio.h>
#include <sys/queue.h>

#define W 1
#define R 2
#define F 3

typedef struct {
    char pname[10];
    float in_time;
    float finish_time;
    float need_time;
    float alloc_time;
    float cycle_time;
    float weight_cycle_time;
    int pstatus;
} pcb;

//带尾指针的双向队列
TAILQ_HEAD(p_head, p_entry) proc_head;
struct p_entry {
    pcb *p;
    TAILQ_ENTRY(p_entry) entries;
} p_link[5];

pcb pcbs[5];

int init_process()
{
    TAILQ_INIT(&proc_head);
    float need_time[5] = {4, 3, 5, 2, 4};
    int i = 0;
    for (i = 0; i < 5; i++) {
        pcbs[i].in_time = i;
        pcbs[i].need_time = need_time[i];
        pcbs[i].alloc_time = 0;
        pcbs[i].pstatus = W;
        p_link[i].p = &pcbs[i];
        TAILQ_INSERT_TAIL(&proc_head, &p_link[i], entries);
    }
    return 0;
}


int run_process(pcb *p, float *time_now, float piece_time)
{
    if (piece_time) {       //有时间片
        if (p->need_time - p->alloc_time > piece_time) { //剩余所需时间大于时间片
            *time_now += piece_time;                    //更新当前时间
            p->alloc_time += piece_time;                //增加已使用CPU时间
            p->pstatus = R;                             //更新进程状态为运行
            return 0;
        }
        else {              //剩余所需时间小于等于时间片
            *time_now += p->need_time - p->alloc_time;  //当前时间增加所需时间减去已分配时间
        }
    }
    else {                  //无时间片
        *time_now += p->need_time;              //当前时间增加所需时间
    }
    p->alloc_time = p->need_time;               //已分配时间为所需时间
    p->finish_time = *time_now;                 //完成时间为当前时间
    p->cycle_time = p->finish_time - p->in_time; //周转时间为完成时间减加入时间
    p->weight_cycle_time = p->cycle_time / p->need_time; //带权周转时间为周转时间除服务时间
    p->pstatus = F;                             //状态为已完成
    return 0;
}

//先来先服务
int fcfs()
{
    pcb *p;
    float time = 0.0;

    init_process();
    while(proc_head.tqh_first) {
        p = proc_head.tqh_first->p;
        run_process(p, &time, 0);
        TAILQ_REMOVE(&proc_head, proc_head.tqh_first, entries);
    }
    return 0;
}

//短作业优先
int spn()
{
    struct p_entry *pi, *pcb_node;
    float time = 0.0;

    init_process();
    pcb_node = proc_head.tqh_first;
    while(proc_head.tqh_first) {
        TAILQ_FOREACH(pi, &proc_head, entries) {
            if (pi->p->in_time <= time) {
                if (pi->p->need_time >= pcb_node->p->need_time)
                    continue;
                pcb_node = pi; 
            }
        }
        run_process(pcb_node->p, &time, 0);
        TAILQ_REMOVE(&proc_head, pcb_node, entries);
        pcb_node = proc_head.tqh_first;
    }
    return 0;
}

//时间片轮转
int rr(float piece_time)
{
    pcb *p;
    struct p_entry *pi;
    float time = 0.0;

    init_process();
    while(proc_head.tqh_first) {
        p = proc_head.tqh_first->p;
        run_process(p, &time, piece_time);
        pi = proc_head.tqh_first;
        TAILQ_REMOVE(&proc_head, proc_head.tqh_first, entries);
        if (p->pstatus != F) {
            TAILQ_INSERT_TAIL(&proc_head, pi, entries);
        }
    }
    return 0;
}

//打印
int prnt(const char *alg)
{
    int i = 0;
    float ave_cyc_time = 0,  ave_wei_cyc_time = 0;
    for (i = 0; i < 5; i++) {
        ave_cyc_time += pcbs[i].cycle_time;
        ave_wei_cyc_time += pcbs[i].weight_cycle_time;
    }
    ave_cyc_time /= 5;
    ave_wei_cyc_time /= 5;
    printf("%-10s%-20s%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f%-10s\n", alg, "完成时间", pcbs[0].finish_time, pcbs[1].finish_time, pcbs[2].finish_time, pcbs[3].finish_time, pcbs[4].finish_time, "");
    printf("%-10s%-20s%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f\n", "", "周转时间", pcbs[0].cycle_time, pcbs[1].cycle_time, pcbs[2].cycle_time, pcbs[3].cycle_time, pcbs[4].cycle_time, ave_cyc_time);
    printf("%-10s%-20s%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f\n", "", "带权周转", pcbs[0].weight_cycle_time, pcbs[1].weight_cycle_time, pcbs[2].weight_cycle_time, pcbs[3].weight_cycle_time, pcbs[4].weight_cycle_time, ave_wei_cyc_time);
    return 0;
}

int main()
{
    float piece_time = 2.0;
    float ave_cyc_time, ave_wei_cyc_time;
    printf("%-10s%-20s%-10s%-10s%-10s%-10s%-10s%-10s\n", "", "进程名称", "A", "B", "C", "D", "E", "平均");
    printf("%-10s%-20s%-10d%-10d%-10d%-10d%-10d%-10s\n", "", "到达时间", 0, 1, 2, 3, 4, "");
    printf("%-10s%-20s%-10d%-10d%-10d%-10d%-10d%-10s\n", "", "服务时间", 4, 3, 5, 2, 4, "");
    fcfs();
    prnt("FCFS");
    spn();
    prnt("SPN");
    rr(piece_time);
    prnt("RR");
    return 0;
}
