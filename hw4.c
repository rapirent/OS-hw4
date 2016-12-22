#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#define MAX_VP 10000
#define MAX_PF 100
struct page{
    int PFN;
    int in_use;
    int present;
    int lru;
    int lfu;
};
struct page VPN[MAX_VP];
struct node{
    int virtual_number;
    struct node* next;
};
//PFN[PFN][In-Use][Present]
//present: 1 for physical memory
//PFN:page-out to disk K(when present = 0) page-in the physical memory
//in_use: is it in virtual memory
int main(void)
{
    srand(time(NULL));
    char tmp[1000] = {'\0'};
    char policy_str[20] = {'\0'};
    int policy = 0;
    //1: FIFO, 2: LRU, 3: Random
    int vp_number;
    int pf_number;
    int PF[MAX_PF] = {0};
    int disk[MAX_VP] = {0};
    fgets(tmp,1000,stdin);
    sscanf(tmp,"Policy: %s",policy_str);
    memset(tmp, 0, sizeof(tmp));
    if(strcmp(policy_str,"FIFO")==0)
    {
        policy = 1;
    }
    else if(strcmp(policy_str,"LRU")==0)
    {
        policy = 2;
    }
    else if(strcmp(policy_str, "Random")==0)
    {
        policy = 3;
    }
    else if(strcmp(policy_str, "LFU")==0)
    {
    	policy = 4;
    }
    struct node* queue_top = NULL;
    struct node* queue_last = NULL;
    //queue:queue_top->3->2->queue_last
    fgets(tmp,1000,stdin);
    sscanf(tmp,"Number of Virtual Page: %d\n",&vp_number);
    memset(tmp, 0, sizeof(tmp));
    fgets(tmp,1000,stdin);
    sscanf(tmp,"Number of Physical Frame: %d\n",&pf_number);
    memset(tmp, 0, sizeof(tmp));
    int clean;
    for(clean=0;clean<vp_number;clean++)
    {
        VPN[clean].PFN = -1;
        VPN[clean].in_use = 0;
        VPN[clean].present = 0;
        VPN[clean].lru = 0;
        VPN[clean].lfu = 0;
    }
    fgets(tmp,1000,stdin);
    memset(tmp, 0, sizeof(tmp));
    int reference;
    float pf_count=0;
    int reference_count = 0;
    int most_use = 0;
    int source, destination, evicted_vpn;
    while(fgets(tmp,1000,stdin)!=NULL)
    {
        sscanf(tmp,"Reference %d",&reference);
        memset(tmp, 0, sizeof(tmp));
        VPN[reference].in_use = 1;
        reference_count = reference_count + 1;
        VPN[reference].lru = reference_count;
        VPN[reference].lfu = VPN[reference].lfu + 1;
        if(VPN[reference].present==1)
        {
            //hit
            printf("Hit, %d=>%d\n",reference,VPN[reference].PFN);
        }
        else
        {
            pf_count = pf_count + 1;
            //miss
            int find_blank;//check if there is a space to space the page
            int is_empty=0;
            for(find_blank = 0; find_blank<pf_number;find_blank++)
            {
                if(PF[find_blank]==0)//there is a empty space
                {
                    is_empty=1;
                    break;
                }
            }
            if(is_empty==0)//no empty sapce
            {
                // printf("type1\n");
                int check_disk = 0, disk_empty = 0;
                for(;check_disk<MAX_VP;check_disk++)
                {
                    if(disk[check_disk] == 0)
                    {
                        disk_empty = 1;
                        break;
                    }
                }
                if(disk_empty == 1)
                {
                    //if disk has empty space
                    destination = check_disk;
                }
                else
                {
                    //if there is no fatal error
                    destination = VPN[reference].PFN;
                }
                ////////////////
                if(policy==1)//FIFO
                {
                    if(queue_top != NULL)
                    {
                        evicted_vpn = queue_top->virtual_number;//the number be page-out
                    }
                    else
                    {
                        fprintf(stderr,"FIFO queue ERROR!!!!\n");
                    }
                    source = VPN[reference].PFN;
                    VPN[reference].PFN = VPN[evicted_vpn].PFN;
                    VPN[reference].present = 1;
                    VPN[evicted_vpn].present = 0;
                    VPN[evicted_vpn].lru = 0;//XXX
                    VPN[evicted_vpn].lfu = 0;//XXX
                    VPN[evicted_vpn].PFN = destination;
                    struct node* new_node = malloc(sizeof(struct node));
                    new_node->virtual_number = reference;
                    new_node->next = NULL;
                    queue_last->next = new_node;
                    queue_last = new_node;
                    struct node* tmp_node = queue_top;
                    queue_top = queue_top->next;//delete from top
                    free(tmp_node);
                }
                else if(policy==2)
                {
                    int compare_number = reference_count, less_number = 0;
                    int traversal = 0;
                    for(;traversal<vp_number;traversal++)
                    {
                        if(VPN[traversal].lru<compare_number&&VPN[traversal].lru!=0)
                        {
                            compare_number = VPN[traversal].lru;
                            less_number = traversal;
                        }
                    }
                    evicted_vpn = less_number;
                    source = VPN[reference].PFN;
                    VPN[reference].PFN = VPN[evicted_vpn].PFN;
                    VPN[reference].present = 1;//TODO
                    VPN[evicted_vpn].PFN = destination;
                    VPN[evicted_vpn].present = 0;
                    VPN[evicted_vpn].lru = 0;
                    VPN[evicted_vpn].lfu = 0;
                }
                else if(policy==3)
                {
                	evicted_vpn=(rand()%vp_number)+1;
                	while(VPN[evicted_vpn].in_use!=1||VPN[evicted_vpn].present==0)
                	{
                		evicted_vpn=(rand()%vp_number)+1;
                	}
                	// while((evited_vpn=(rand())%vp_number)+1)&&VPN[reference]
                    // evicted_vpn=(rand()%vp_number)+1;
                    source = VPN[reference].PFN;
                    VPN[reference].PFN = VPN[evicted_vpn].PFN;
                    VPN[reference].present = 1;
                    VPN[evicted_vpn].PFN = destination;
                    VPN[evicted_vpn].present = 0;
                    VPN[evicted_vpn].lru = 0;
                    VPN[evicted_vpn].lfu = 0;
                }
                else if(policy==4)
                {
                    int compare_number = reference_count, less_number = 0;
                    int traversal = 0;
                    for(;traversal<vp_number;traversal++)
                    {
                        if(VPN[traversal].lfu<compare_number&&VPN[traversal].lfu!=0)
                        {
                            compare_number = VPN[traversal].lfu;
                            less_number = traversal;
                        }
                    }
                    evicted_vpn = less_number;
                    source = VPN[reference].PFN;
                    VPN[reference].PFN = VPN[evicted_vpn].PFN;
                    VPN[reference].present = 1;//TODO
                    VPN[evicted_vpn].PFN = destination;
                    VPN[evicted_vpn].present = 0;
                    VPN[evicted_vpn].lru = 0;
                    VPN[evicted_vpn].lfu = 0;
                }
                disk[source] = 0;
                disk[destination] = 1;
            }
            else
            {
                if(policy==1)//FIFO
                {
                    struct node* new_node = malloc(sizeof(struct node));
                    new_node->virtual_number = reference;
                    new_node->next = NULL;//add from last
                    if(queue_last!=NULL)
                    {
                        queue_last->next = new_node;
                    }
                    queue_last = new_node;
                    if(queue_top==NULL)
                    {
                        queue_top = queue_last;
                    }
                }
                source = VPN[reference].PFN;
                destination = -1;
                evicted_vpn = -1;
                VPN[reference].PFN = find_blank;
                VPN[reference].present = 1;
                PF[find_blank] = 1;
                disk[source] = 0;
                // printf("type2\n");
            }
            printf("Miss, %d, %d>>%d, %d<<%d\n",VPN[reference].PFN, evicted_vpn, destination, reference, source);
        }
    }
    printf("Page Fault Rate: %.3f\n",pf_count/reference_count);
    return 0;
}
