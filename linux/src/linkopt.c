 

/*
文件操作链表
从QT TCP 客户端那边移植过来

（1）c语言获取时间
#include <time.h>
time_t time( time_t * ) ;

（2）随机数
int rand(void);
int rand_r(unsigned int *seedp);
void srand(unsigned int seed);
*/

#include "linkopt.h"
#include "mylib.h"


//##########################################################辅助函数
//生成唯一码
#define CODE_SELETE_NUM 36
const char u_code_selete[CODE_SELETE_NUM]={
    '0','1','2','3','4','5','6','7','8','9',
    'a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t',
    'u','v','w','x','y','z'
};
int generateUniqueCode(char *code)
{
    static int cnt = 0;
    cnt++;
    //char code[UNIQUE_CODE_SIZE + 1] = {0};//最后一个字符存放'\0'
    unsigned short n;
    int i;
    time_t t, t_tmp;
    for(i = 0; i < UNIQUE_CODE_SIZE / 2; i++)
    {
        t = time(&t_tmp);
        srand(t + cnt);        
        n = rand() % 65536;    //产生16位数的随机数
        code[i * 2 ] = u_code_selete[(n >> 8) % 36];
        code[i * 2 + 1] = u_code_selete[(n & 0xff) % 36];
        cnt++;
    }
    code[UNIQUE_CODE_SIZE] = '\0';
    return 0;
}



//由链表头获取对应的索引号
int linkOptManage_getIndex(OptLink *head)
{
    int i;
    for(i = 0; i < gloale_opt_link_manage_head.num; i++)
    {
        if(gloale_opt_link_manage_head.head[i] == head)
        {
            return i;
        }     
    }
    return -1;
}


//###############################################################链表buf文件及其接口
//将链表存于散列表中
/*
 *用于同步的散列表文件存于store文件夹中（临时的则放置于 optlink 文件夹），例如：para.link  和 data.link
 参数散列表约定：
 （1）有效数据起始于起始码 0x66 0x77 0x88 0x99（在文件中找到第一个此标志）
 （2）数据头：结构体的个数，大小为4个字节（肯定够用了）
 （3）数据有多个连续的结构体组成，结构包含必要参数：opt、obj、unique（以后可以拓展，加一个MD5校验码）
 数据散列表约定：
 （1）有效数据起始于起始码 0x66 0x77 0x88 0x99
 （2）数据头：结构块的个数，大小为4个字节（肯定够用了）
 （3）连续的数据块：数据块长度（4字节）+ 时间戳(4字节) + 唯一标识码（16字节）+ 有效数据
 */
 //创建数据表
int dataTable_create(char *name)
{  
    //创建文件
    if(0 != fullPath_bufCreate(name))
        {
        printf("dataTable_create: file create faild\n");
        return -1;
    }
    
    //填充头部
    char tmp[32] = {0};//注意：以后更改代码时要看大小是否超过范围
    int num;
    tmp[0] = START_CODE_0;
    tmp[1] = START_CODE_1;
    tmp[2] = START_CODE_2;
    tmp[3] = START_CODE_3;
    tmp[4] = (0 & 0xff000000) >> 24;   
    tmp[5] = (0 & 0xff0000) >> 16;  
    tmp[6] = (0 & 0xff00) >> 8;  
    tmp[7] = (0 & 0xff);  
    fullPath_bufAppend(name, tmp, 8);

    return 0;
}

//数据表操作接口（增、删、改、查）
int dataTable_contentOptInt(char *data_table, OptLink *head, OptLink *node, int opt, void *data, unsigned int start, unsigned int len)
{
    //参数检测
        if(start < 0 || len < 0)
        {
            printf("dataTable_contentOptInt: para error\n");
            return -1;
        }
                     
        //分析此文件中的内容   
        //--1 取得文件长度
        int num;
        int totle_len = fullPath_bufGetLen(data_table);
        if(totle_len < 8)
            {
            printf("dataTable_contentOptInt: file is too short\n");
            return 0;
        }    
        //--2 判断起始码 取得个数
        char tmp[32 + UNIQUE_CODE_SIZE +1] = {0};
        fullPath_bufGetBlock(data_table, tmp, 0, 8);
        //printf("=======================>C   tmp[0]=0x%x tmp[1]=0x%x tmp[2]=0x%x tmp[3]=0x%x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
        if(START_CODE_0 != (unsigned char)tmp[0] || 
            START_CODE_1 != (unsigned char)tmp[1] ||
            START_CODE_2 != (unsigned char)tmp[2] ||
            START_CODE_3 != (unsigned char)tmp[3])
            {
            printf("dataTable_contentOptInt: file format is error\n");
            return -1;
        }
        num = (unsigned char)(tmp[4]) << 24 | (unsigned char)(tmp[5]) << 16 | (unsigned char)(tmp[6]) << 8 | (unsigned char)(tmp[7]);            
        if(num  < 0)
            {
            printf("dataTable_contentOptInt: num is error\n");
            return -1;
        }
        //--3 取得节点对应的起始位置
        unsigned int seek = 8;
        int node_len;
        int i;  
        int index_mark;
        index_mark = 0;
        //获得索引
        int index;  
        index = linkOptGetIndex(head, node);
        if(index < 0  )
           {
           printf("dataTable_contentOptInt:get index error, index=%d\n", index);
           return -1;
        }
        //printf("=======================>B  num=%d totle_len=%d  index=%d\n", num, totle_len, index);
        for(i = 0; i < num; i++)
        {
            fullPath_bufGetBlock(data_table, tmp, seek, 8);
            node_len = (unsigned char)(tmp[0]) << 24 | (unsigned char)(tmp[1]) << 16 | (unsigned char)(tmp[2]) << 8 | (unsigned char)(tmp[3]);
            if(node_len < 0 )
            {
             printf("dataTable_contentOptInt: get node_len error\n");
             return -1;
             }
            
                 
            if(i == index)//找到了相应的索引, index_mark 作为标记
                {
                index_mark = 1;
                break;
            }   
            seek+= node_len;
    
        } 
       
        //按操作码动作
        if(opt == contentOpt_new)//新增节点元素，直接从后面添加
        {      
            //加1
            tmp[0] = ((num + 1) & 0xff000000) >> 24;   
            tmp[1] = ((num + 1)& 0xff0000) >> 16;  
            tmp[2] = ((num + 1)& 0xff00) >> 8;  
            tmp[3] = ((num + 1) & 0xff);  
            fullPath_bufSetBlock(data_table, tmp, 4, 4);
           // printf("=======================>A  num=%d  seek=%d  idnex=%d len=%d\n", num, seek, index, len);
            if(data != NULL)
                {
                len = 0;
            }
            tmp[0] = ((9 + UNIQUE_CODE_SIZE + len) & 0xff000000) >> 24;   
            tmp[1] = ((9 + UNIQUE_CODE_SIZE + len) & 0xff0000) >> 16;  
            tmp[2] = ((9 + UNIQUE_CODE_SIZE + len) & 0xff00) >> 8;  
            tmp[3] = ((9 + UNIQUE_CODE_SIZE + len) & 0xff);
            tmp[4] = (node->time & 0xff000000) >> 24;   
            tmp[5] = (node->time & 0xff0000) >> 16;  
            tmp[6] = (node->time & 0xff00) >> 8;  
            tmp[7] = (node->time & 0xff);   
            for(i = 0; i < UNIQUE_CODE_SIZE + 1; i++)
                {
                tmp[8 + i] = node->unique[i];
            }
            fullPath_bufInsertBlock(data_table, tmp, seek, 9 + UNIQUE_CODE_SIZE);
            if(data != NULL)
                {
                 fullPath_bufInsertBlock(data_table, data, seek + 9 + UNIQUE_CODE_SIZE, len); 
            }
                          
        }
        else if(opt == contentOpt_append)//追加内容
            {
    
             if(data == NULL || (index_mark == 0 ))//逻辑比较绕
                {
                return 0;
            }
            tmp[0] = ((node_len + len) & 0xff000000) >> 24;   
            tmp[1] = ((node_len + len) & 0xff0000) >> 16;  
            tmp[2] = ((node_len + len) & 0xff00) >> 8;  
            tmp[3] = ((node_len + len) & 0xff);
            fullPath_bufSetBlock(data_table, tmp, seek, 4);
            //printf("===========================>A seek=%d \n",seek);
            if(data != NULL)
                {
                fullPath_bufInsertBlock(data_table, data, seek + 9 + UNIQUE_CODE_SIZE + start, len);
            }       
        }
        else if(opt == contentOpt_del)
            {
            if(num < 1 || index_mark == 0)//不足一个节点，没有删除的对象；或者没有匹配到对象
                {
                return 0;
            }
            //printf("=======================>A  num=%d seek =%d node_len=%d\n", num, seek, node_len);
            //减1
            tmp[0] = ((num - 1) & 0xff000000) >> 24;   
            tmp[1] = ((num - 1)& 0xff0000) >> 16;  
            tmp[2] = ((num - 1)& 0xff00) >> 8;  
            tmp[3] = ((num - 1) & 0xff);  
            fullPath_bufSetBlock(data_table, tmp, 4, 4);
            
            tmp[0] = (0 & 0xff000000) >> 24;   
            tmp[1] = (0& 0xff0000) >> 16;  
            tmp[2] = (0 & 0xff00) >> 8;  
            tmp[3] = (0 & 0xff);
            tmp[4] = (index & 0xff000000) >> 24;   
            tmp[5] = (index & 0xff0000) >> 16;  
            tmp[6] = (index & 0xff00) >> 8;  
            tmp[7] = (index & 0xff);              
            fullPath_bufDelBlock(data_table, seek , node_len);         
        }
        else if(opt == contentOpt_get)
        {
              //printf("=======================>A  index=%d seek =%d\n", index, seek);
            fullPath_bufGetBlock(data_table, data, seek + 9 + UNIQUE_CODE_SIZE + start, len);
        }
        else if(opt == contentOpt_getInfo)//获得起始位置 start（4这字节）和 长度len（4字节）
        {
            if(data == NULL)
            {
                return -1;
            }
            int ret_start = seek + 9 + UNIQUE_CODE_SIZE; 
              int ret_len = node_len - 9 - UNIQUE_CODE_SIZE; 
              if(ret_len < 0)
              {
                    return -1;
              }
            ((char *)data)[0] = (ret_start & 0xff000000) >> 24;   
            ((char *)data)[1] = (ret_start & 0xff0000) >> 16;  
            ((char *)data)[2] = (ret_start & 0xff00) >> 8;  
            ((char *)data)[3] = (ret_start & 0xff);
            ((char *)data)[4] = (ret_len & 0xff000000) >> 24;   
            ((char *)data)[5] = (ret_len & 0xff0000) >> 16;  
            ((char *)data)[6] = (ret_len & 0xff00) >> 8;  
            ((char *)data)[7] = (ret_len & 0xff);
            
        }
    
        return 0;
}




//对数据表的操作（停止使用）
int dataTable_contentOpt(OptLink *head, OptLink *node, int opt, void *data, unsigned int start, unsigned int len)
{
//    //参数检测
//    if(head == NULL || node == NULL || start < 0 || len < 0)
//    {
//        printf("dataTable_contentOpt: para error\n");
//        return -1;
//    }
//    
//    //取得文件名字
//    int link_index;
//    link_index = linkOptManage_getIndex(head);
//    if(link_index < 0)
//        {
//        printf("dataTable_contentOpt: idnex is error\n");
//        return -1;
//    }
//    if(strlen(gloale_opt_link_manage_head.buf_name[link_index]) <= 0 )
//        {
//        printf("dataTable_contentOpt: buf_name is error\n");
//        return -1;
//    }
//
//    char buf_name[FILE_NAME_MAX_SIZE] = {0};
//    strcpy(buf_name, gloale_opt_link_manage_head.buf_name[link_index]);
//
//    
//    return dataTable_contentOptInt(buf_name, head, node, opt, data, start, len);
    return 0;
    
}


//################################################################链表基本功能
//初始化
void linkOptInit(void)
{
    floderCreate(WORK_SPACE_PATH);
    floderCreate(OPT_LIN_BUF_PATH);
    memset((void *)(&gloale_opt_link_manage_head), 0 , sizeof(OptLinkManage));//初始化链表管理结构体
    gloale_opt_link_manage_head.num = 0;
}
 

//方法
OptLink *linkOptCreate(void)//链表创建
{
    OptLink *head = (OptLink *)malloc(sizeof(OptLink));
    head->next = NULL;
    
    sprintf(gloale_opt_link_manage_head.buf_name[ gloale_opt_link_manage_head.num],
            "%s/%s-%d.buf", OPT_LIN_BUF_PATH, OPT_LINK_BUF_NAME, gloale_opt_link_manage_head.num);
    //dataTable_create(gloale_opt_link_manage_head.buf_name[ gloale_opt_link_manage_head.num]);//开辟buf 
    gloale_opt_link_manage_head.head[gloale_opt_link_manage_head.num] = head;   
    gloale_opt_link_manage_head.num++;
    
     
    return head;
}

//OptLink *linkOptCreate_assignBuf(char *buf_name)//链表创建，指定buf文件
//{
//    OptLink *head = (OptLink *)malloc(sizeof(OptLink));
//    head->next = NULL;
//    
//    sprintf(gloale_opt_link_manage_head.buf_name[ gloale_opt_link_manage_head.num], buf_name);
//    gloale_opt_link_manage_head.head[gloale_opt_link_manage_head.num] = head;   
//    gloale_opt_link_manage_head.num++;
//     
//    return head;
//}


OptLink *linkOptAddNode(OptLink *head)//增加节点
{
    if(head == NULL)
    {
        return NULL;
    }
    OptLink *node = (OptLink *)malloc(sizeof(OptLink));
    OptLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }
    prob->next = node;
    node->next = NULL;
    //生成唯一码，唯一码重复的概率还是很低的，所以只进行一次检测
    generateUniqueCode(node->unique);
    int mark = 1;//一次检测标志
    prob = head->next;
    while(prob != NULL)
    {
        if(strcmp(prob->unique, node->unique) == 0 && prob != node)
        {
            if(mark == 1)
            {
                generateUniqueCode(node->unique);
                prob = head->next;
                mark = 0;
                continue;
            }
            printf("linkAddNode: unique is repetition\n");
            return NULL;
        }
        prob = prob->next;
    }
    //时间戳
    time_t t_tmp;
    node->time = time(&t_tmp);
    //在缓存文件中插入空内容
    dataTable_contentOpt(head, node, contentOpt_new, NULL, 0, 0);

    
    return node;
}


OptLink *linkOptInsertNode(OptLink *head, int index)//插入节点
{
    if(head == NULL)
    {
        return NULL;
    }
    int  i;
    OptLink *node = (OptLink *)malloc(sizeof(OptLink));
    OptLink *prob = head;
    i = 0;
    while(prob != NULL)
    {

        if(i == index)
        {
            node->next = prob->next;
            prob->next = node;
            break;
        }
        i++;
        prob = prob->next;
    }

    //生成唯一码，唯一码重复的概率还是很低的，所以只进行一次检测
    generateUniqueCode(node->unique);
    int mark = 1;//一次检测标志
    prob = head->next;
    while(prob != NULL)
    {
        if(strcmp(prob->unique, node->unique) == 0 && prob != node)
        {
            if(mark == 1)
            {
                generateUniqueCode(node->unique);
                mark = 0;
                continue;
            }
            printf("linkAddNode: unique is repetition\n");
            return NULL;
        }
        prob = prob->next;
    }
    //时间戳
    time_t t_tmp;
    node->time = time(&t_tmp);
    //在缓存文件中插入空内容
    dataTable_contentOpt(head, node, contentOpt_new, NULL, 0, 0);

    return node;
}


void linkOptDelNode(OptLink *head)//删除节点
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob->next != NULL)
    {
        last = prob;
        prob = prob->next;

    }
    //删除buf文件中相关内容
     dataTable_contentOpt(head, prob, contentOpt_del, NULL, 0, 0);

     
    last->next = NULL;    
    free(prob);        
}


void linkOptDelNode_byIndex(OptLink *head, int index)//根据索引删除节点  注：索引为节点的一个属性，以0为起始自增
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    int i = 0;
    while(prob != NULL)
    {
        if(i == index)
        {
            //删除buf文件中相关内容
            dataTable_contentOpt(head, prob, contentOpt_del, NULL, 0, 0);
            
            last->next = prob->next;
            free(prob);
            return;
        }
        i++;
        last = prob;
        prob = prob->next;
    }
}

//通过节点本身删除节点
int linkOptDelNode_byNode(OptLink *head, OptLink *node)
{
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob == node)
        {
            //删除buf文件中相关内容
            dataTable_contentOpt(head, prob, contentOpt_del, NULL, 0, 0);
            
            last->next = node->next;
            free(node);
            return 0;
        }
        last = prob;
        prob = prob->next;
    }

    return -1;
}

//删除链表的所有节点，但是并不删除链表头
void linkOptDelNodeAll(OptLink *head)
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {       
            //删除buf文件中相关内容
            dataTable_contentOpt(head, prob, contentOpt_del, NULL, 0, 0);
            
            free(prob);
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }
    head->next == NULL;
}

void linkOptlinkDestory(OptLink *head)//摧毁链表
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {
            //删除buf文件中相关内容
            dataTable_contentOpt(head, prob, contentOpt_del, NULL, 0, 0);
            
            free(prob);
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }

    gloale_opt_link_manage_head.num--;
    free(head);
}

//获取索引
int linkOptGetIndex(OptLink *head, OptLink *node)
{
    if(head == NULL)
   {
       return -1;
   }

    OptLink *prob = head->next;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        if(prob == node)
        {
            return cnt;
        }
        cnt++;
        prob = prob->next;
    }
    return -1;
}
//获取节点个数
int linkOptGetNum(OptLink *head)
{
    if(head == NULL)
    {
        return -1;
    }
    OptLink *prob = head->next;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        cnt++;
        prob = prob->next;
    }

    return cnt;
}

//链表追加
int linkOptlinkAppend(OptLink *head, OptLink *add_head)
{
    if(head == NULL || add_head == NULL)
    {
        printf("linkAppend: link head is NULL");
        return -1;
    }
    OptLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }

    prob->next = add_head->next;

    return 0;
}
//链表追加 （确保追加的节点不与之前的重复，通过唯一码和时间戳来判断）
int linkOptlinkAppend_noRepeat(OptLink *head, OptLink *add_head)
{
    if(head == NULL || add_head == NULL)
    {
        printf("linkOptlinkAppend_noRepeat: link head is NULL");
        return -1;
    }
   
	int size = sizeof(OptLink);
   OptLink *prob = add_head->next;
   OptLink *node;
   while(prob != NULL)
   {
		if(linkOptLink_getNodeByUnique(head, prob->unique, prob->time) == NULL)
		{
			node = linkOptAddNode(head);
			memcpy(node, prob, size);
			prob->next = NULL;
		}
	   prob = prob->next;
   }

    return 0;
}

//查找唯一码和时间戳相等的节点
OptLink *linkOptLink_getNodeByUnique(OptLink *head, char *unique, int t)
{
	if(head == NULL )
    {
        printf("linkOptLink_getNodeByUnique: link head is NULL");
        return NULL;
    }
	OptLink * prob = head->next;
	while(prob != NULL)
	{
		if(strcmp(prob->unique, unique) == 0 && prob->time == t)
		{
			return prob;
		}
		prob = prob->next;
	}
	return NULL;	
}



//测试
void OptLinkShow(OptLink *head)
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    printf("totle num: %d\n", linkOptGetNum(head));
    char block[20] = {0};
    while(prob != NULL)
    {
        
        printf(" opt=%d  obj=%s   unique=%s  time=%d \n", prob->opt, prob->obj, prob->unique, prob->time);
        prob = prob->next;
    }

}

void OptLinkTest(void)
{

    OptLink *head = linkOptCreate();
    OptLink *node1 = linkOptAddNode(head);
    OptLink *node2 = linkOptAddNode(head);
    OptLink *node3 = linkOptAddNode(head);
    OptLink *node4 = linkOptAddNode(head);

    strcpy(node1->obj, "node1");
    strcpy(node2->obj, "node2");
    strcpy(node3->obj, "node3");
    strcpy(node4->obj, "node4");

    dataTable_contentOpt(head, node1, contentOpt_append, "kkkkkkkk", 0, 8);
    dataTable_contentOpt(head, node2, contentOpt_append, "jjjjjjjj", 0, 8);
    dataTable_contentOpt(head, node3, contentOpt_append, "hhhhhhhh", 0, 8);
    dataTable_contentOpt(head, node4, contentOpt_append, "iiiiiiii", 0, 8);

    //插入测试
     //OptLink *node5 = linkOptInsertNode(head, 2);
     //strcpy(node5->obj, "node5");

     //删除测试
     //linkOptDelNode(head);
     //linkOptDelNode_byIndex(head,  2);
    
    printf("================>链表测试1\n");
    OptLinkShow(head);

    //获取内容测试
//    char tmp[32] = {0};
//    dataTable_contentOpt(head, node1, contentOpt_get, tmp, 0, 8 );
//    printf("获取节点node1内容： %s\n", tmp);

   
}







//int main(void)
//{
//    
//    linkOptInit();
//    OptLinkTest();
//    
//    
//    return 0;
//   
//}
//



























