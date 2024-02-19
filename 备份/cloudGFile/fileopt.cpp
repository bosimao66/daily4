

#include "fileopt.h"





//#########################################################类外方法
//二进制数据复制
int my_memcpy(void *data, void *out, int start, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		((char *)out)[i] = ((char *)data)[i + start];
	}
	return 0;
}




FileOpt::FileOpt()
{
    init();
}

//#####################################################功能函数
//测试
void FileOpt::test()
{
    static int mark = 0;


    QString tmp1, tmp2;
    tmp1.clear();
	//tmp1.append(store_path);
	//tmp1.append(USER_SYNC_PARA_TABLE);
	tmp1.append(current_para_table_local);
    tmp2.clear();
	//tmp2.append(store_path);
	//tmp2.append(USER_SYNC_DATA_TABLE);
	tmp2.append(current_data_table_local);

	OptLink *node1 = opt_link_int->linkAddNode(opt_link_head);
	OptLink *node2 = opt_link_int->linkAddNode(opt_link_head);
	OptLink *node3 = opt_link_int->linkAddNode(opt_link_head);
	OptLink *node4 = opt_link_int->linkAddNode(opt_link_head);
	node1->opt = 1;
	node1->obj.clear();
	node1->obj.append("name1");
	node1->content.clear();
	node1->content.append("cont1111");
	node2->opt = 1;
	node2->obj.clear();
	node2->obj.append("name2");
	node2->content.clear();
	node2->content.append("cont2222");
	node3->opt = 1;
	node3->obj.clear();
	node3->obj.append("name3");
	node3->content.clear();
	node3->content.append("cont3333");
	node4->opt = 1;
	node4->obj.clear();
	node4->obj.append("name4");
	node4->content.clear();
	node4->content.append("cont4444");
	qDebug()<<"[test]增加节点";
	opt_link_int->show(opt_link_head);
	linkToFile((opt_link_head), tmp1, tmp2);
	exeLinkAction(opt_link_head);

}

//文件操作初始化
void FileOpt::init()
{
    qDebug()<<"fileopt init is comming";
    QDir d;
    wk_path.clear();
    wk_path.append(FILE_OPT_WK_PATH);
    wk_path.append("/");
    back_dir.clear();
    back_dir.append(FILE_OPT_BACK_DIR);
    store_dir.clear();
    store_dir.append(FILE_OPT_STORE_DIR);

    //不存在则创建文件
    if(floderExist(wk_path) == false)
    {
        //qDebug()<<"===>wk_path="<<wk_path;
        if(false == d.mkdir(FILE_OPT_WK_PATH))
        {
            wk_path.clear();
            wk_path.append("./");
        }

    }


    store_path.clear();
    store_path.append(wk_path);
    store_path.append(store_dir);
    store_path.append("/");
    back_path.clear();
    back_path.append(wk_path);
    back_path.append(back_dir);
    back_path.append("/");
    //创建备份路径
    if(floderExist(back_path) == false)
    {

        if(true == d.cd(wk_path))
        {
            d.mkdir(back_dir);
            back_path.clear();
            back_path.append(wk_path);
            back_path.append(back_dir);
            back_path.append("/");
        }
    }

    //创建仓库路径
    if(floderExist(store_path) == false)
    {
        if(true == d.cd(wk_path))
        {
            d.mkdir(store_dir);
            store_path.clear();
            store_path.append(wk_path);
            store_path.append(store_dir);
            store_path.append("/");
        }
    }

    //操作链表接口
    opt_link_head = NULL;


}
//设置操作链表头，操作链表的接口
void FileOpt::setOptLinkHead(OptLink *head, OptLink *interface)
{
    opt_link_head = head;
    opt_link_int = interface;
    return ;
}



//#####################################################功能函数
int FileOpt::userFloderInit(QString user_name)//用户目录初始化化
{
	QString tmp1, tmp2, tmp3, tmp4;
	tmp1.clear();
	tmp1.sprintf("%s%s/%s", wk_path.toLatin1().data(), FILE_OPT_USER_DIR, user_name.toLatin1().data());
	tmp2.clear();
	tmp2.sprintf("%s%s", wk_path.toLatin1().data(), FILE_OPT_USER_DIR);
	tmp3.clear();
	tmp3.sprintf("%s%s/%s/%s", wk_path.toLatin1().data(), FILE_OPT_USER_DIR, user_name.toLatin1().data(), FILE_OPT_USER_TABLE_DIR);
	QDir d;

	QByteArray block;
	block[0] = START_CODE_0;
	block[1] = START_CODE_1;
	block[2] = START_CODE_2;
	block[3] = START_CODE_3;
	block[4] = block[5] = block[6] = block[7] = 0;
	if(floderExist(tmp2) == false)
	{
		qDebug()<<"tmp1: "<<tmp1;
		qDebug()<<"tmp2: "<<tmp2;
		if(true == d.cd(wk_path))
		{
			d.mkdir(FILE_OPT_USER_DIR);
		}
	}
	if(true == d.cd(tmp2))
	{
		d.mkdir(user_name);
		if(true == d.cd (tmp1))
		{
			d.mkdir(FILE_OPT_USER_TABLE_DIR);
			tmp4.clear();
			tmp4.append(tmp3);
			tmp4.append("/");
			tmp4.append(USER_LOCAL_PARA_TABLE);
			QFile f(tmp4);
			f.open(QIODevice::WriteOnly | QIODevice::Truncate);
			f.write(block);
			f.close();

			tmp4.clear();
			tmp4.append(tmp3);
			tmp4.append("/");
			tmp4.append(USER_LOCAL_DATA_TABLE);
			f.setFileName(tmp4);
			f.open(QIODevice::WriteOnly | QIODevice::Truncate);
			f.write(block);
			f.close();

			d.mkdir(FILE_OPT_USER_TABLE_DIR);
			tmp4.clear();
			tmp4.append(tmp3);
			tmp4.append("/");
			tmp4.append(USER_DOWNLOAD_PARA_TABLE);
			f.setFileName(tmp4);
			f.open(QIODevice::WriteOnly | QIODevice::Truncate);
			f.write(block);
			f.close();

			tmp4.clear();
			tmp4.append(tmp3);
			tmp4.append("/");
			tmp4.append(USER_DOWNLOAD_DATA_TABLE);
			f.setFileName(tmp4);
			f.open(QIODevice::WriteOnly | QIODevice::Truncate);
			f.write(block);
			f.close();

			//创建备份目录
			d.mkdir(FILE_OPT_BACK_DIR);
		}
	}


	return 0;
}
int FileOpt::setCurrentUserFloder(QString user_name)//设置当前的用户目录
{
	current_user_path.clear();
	current_user_path.sprintf("%s%s/%s", wk_path.toLatin1().data(), FILE_OPT_USER_DIR, user_name.toLatin1().data());
	current_user_table_path.clear();
	current_user_table_path.sprintf("%s%s/%s/%s", wk_path.toLatin1().data(), FILE_OPT_USER_DIR, user_name.toLatin1().data(), FILE_OPT_USER_TABLE_DIR);

	current_para_table_sync.clear();
	current_data_table_sync.clear();
	current_para_table_local.clear();
	current_data_table_local.clear();
	current_para_table_download.clear();
	current_data_table_download.clear();
	current_back_path.clear();

	current_para_table_sync.sprintf("%s/%s", current_user_table_path.toLatin1().data(), USER_SYNC_PARA_TABLE);
	current_data_table_sync.sprintf("%s/%s", current_user_table_path.toLatin1().data(), USER_SYNC_DATA_TABLE);
	current_para_table_local.sprintf("%s/%s", current_user_table_path.toLatin1().data(), USER_LOCAL_PARA_TABLE);
	current_data_table_local.sprintf("%s/%s", current_user_table_path.toLatin1().data(), USER_LOCAL_DATA_TABLE);
	current_para_table_download.sprintf("%s/%s", current_user_table_path.toLatin1().data(), USER_DOWNLOAD_PARA_TABLE);
	current_data_table_download.sprintf("%s/%s", current_user_table_path.toLatin1().data(), USER_DOWNLOAD_DATA_TABLE);
	current_back_path.sprintf("%s/%s",  current_user_path.toLatin1().data(), FILE_OPT_BACK_DIR);
	//qDebug()<<"=======================>A "<<current_para_table_download;
	return 0;
}

//将链表存于散列表中
/*
 * 散列表文件存于store文件夹中例如：para.link  和 data.link
 参数散列表约定：
 （1）有效数据起始于起始码 0x66 0x77 0x88 0x99（在文件中找到第一个此标志）
 （2）数据头：结构体的个数，大小为4个字节（肯定够用了）
 （3）数据有多个连续的结构体组成，结构包含必要参数：opt、obj、unique（以后可以拓展，加一个MD5校验码）
 数据散列表约定：
 （1）有效数据起始于起始码 0x66 0x77 0x88 0x99
 （2）数据头：结构块的个数，大小为4个字节（肯定够用了）
 （3）连续的数据块：数据块长度（4字节）+ 时间戳(4字节) + 唯一标识码（16字节）+ 有效数据
 */
int FileOpt::linkToFile(OptLink *head, QString para_file, QString data_file)
{
    int ret1, ret2;
    //qDebug()<<"====================>AA "<<para_file<<" "<<data_file;
    ret1 = linkToParaFile(head, para_file);
    ret2 = linkToDataFile(head, data_file);
    if(ret1 != 0 || ret2 != 0)
    {
        return -1;
    }
    return 0;
}

//参数表的写入
int FileOpt::linkToParaFile(OptLink *head, QString file_name)
{
	//qDebug()<<"===========================>B";
	//opt_link_int->show(head);
    //填充内容
    QByteArray t, block;
    t.clear();
    t[0] = START_CODE_0;
    t[1] = START_CODE_1;
    t[2] = START_CODE_2;
    t[3] = START_CODE_3;

    quint32 num = opt_link_int->getNum(head);
    t[4] = (num & 0xff000000) >> 24;
    t[5] = (num & 0xff0000) >> 16;
    t[6] = (num & 0xff00) >> 8;
    t[7] = (num & 0xff);

    STRUCT_PARA_TABLE_ELEMENT ele;
    int size = sizeof(STRUCT_PARA_TABLE_ELEMENT);
    OptLink *prob = head->next;
    int index;
    int i;
    index = 0;
    while(prob != NULL)
    {
        memset((void *)(&ele), 0,size);
        strcpy(ele.file_name, prob->obj.toLatin1().data());
        ele.index = index;
        ele.opt = prob->opt;
        strcpy(ele.unique, prob->unique);
		ele.time = prob->time;
        //结构体数据放入block
        block.clear();
        for(i = 0; i < size; i++)
        {
            block[i] = ((char *)(&ele))[i];
        }
        //qDebug()<<"===================>[block]"<<block;
        t.append(block);
        index++;
        prob = prob->next;
    }

    //打开文件并写入内容
    QFile para(file_name);
    if(para.open(QIODevice::ReadWrite | QIODevice::Truncate) == false)
    {
        qDebug()<<"linkToParaFile: file open faild";
        return -1;
    }
    para.write(t);
    para.close();

    return 0;
}

//数据表的写入
int FileOpt::linkToDataFile(OptLink *head, QString file_name)
{
    int i;
    //填充内容
    QByteArray t, block;
    t.clear();
    t[0] = START_CODE_0;
    t[1] = START_CODE_1;
    t[2] = START_CODE_2;
    t[3] = START_CODE_3;

    quint32 num = opt_link_int->getNum(head);
    t[4] = (num & 0xff000000) >> 24;
    t[5] = (num & 0xff0000) >> 16;
    t[6] = (num & 0xff00) >> 8;
    t[7] = (num & 0xff);

    quint32 data_len;
    quint32 index;
    OptLink *prob = head->next;
    index = 0;
    while(prob != NULL)
    {
        block.clear();
        data_len = 8 + UNIQUE_CODE_SIZE + 1 + prob->content.length();
        block[0] = (data_len & 0xff000000) >> 24;
        block[1] = (data_len & 0xff0000) >> 16;
        block[2] = (data_len & 0xff00) >> 8;
        block[3] = (data_len & 0xff);
		//时间戳 quint64 , 这里截取了低 32 位，应该够用？？
		block[4] = (prob->time & 0xff000000) >> 24;
		block[5] = (prob->time & 0xff0000) >> 16;
		block[6] = (prob->time & 0xff00) >> 8;
		block[7] = (prob->time & 0xff);

        for(i = 0; i < UNIQUE_CODE_SIZE + 1; i++)
        {
            block[8 + i] = prob->unique[i];
        }
        block.append(prob->content.data());
        t.append(block);
        index++;
        prob = prob->next;
    }
    //打开文件并写入内容
    QFile data(file_name);
    if(data.open(QIODevice::ReadWrite | QIODevice::Truncate) == false)
    {
        qDebug()<<"linkToDataFile: file open faild";
        return -1;
    }
    data.write(t);
    data.close();

    return 0;
}


//散列表文件转化为链表
OptLink *FileOpt::fileToLink(QString para_name, QString data_name)
{
	//创建链表
	OptLink *head;
	head = opt_link_int->linkCreate();

    QFile para(para_name);
    QFile data(data_name);
    //qDebug()<<"para_name="<<para_name<<"  data_name"<<data_name;
    if(para.open(QIODevice::ReadOnly) == false ||
            data.open(QIODevice::ReadOnly) == false)
    {
		qDebug()<<"fileToLink: file open faild, para_name="<<para_name<<"  data_name="<<data_name;
		return head;
    }

    QByteArray t1 = para.readAll();
    QByteArray t2 = data.readAll();
    para.close();
    data.close();


    quint32 start_para, start_data;
    start_para = start_data = 0;
    int mark1, mark2;
    mark1 = mark2 = 0;
    int i, j;
    //找到起始码
    //qDebug()<<"t1.length="<<t1.length()<<"  t2.length"<<t2.length();
    for(i = 0; i < t1.length() - 3; i++)
    {
        if((quint8)t1[i] == START_CODE_0 && (quint8)t1[i + 1] == START_CODE_1 &&
                (quint8)t1[i + 2] == START_CODE_2 && (quint8)t1[i + 3] == START_CODE_3)
        {
            start_para = i;
            mark1 = 1;
        }
    }

    for(i = 0; i < t2.length() - 3; i++)
    {
        if((quint8)t2[i] == START_CODE_0 && (quint8)t2[i + 1] == START_CODE_1 &&
                (quint8)t2[i + 2] == START_CODE_2 && (quint8)t2[i + 3] == START_CODE_3)
        {
            start_data = i;
            mark2 = 1;
        }
    }

    if(mark1 != 1 || mark2 != 1)
    {
        qDebug()<<"fileToLink: pre code get error,mark1 "<<mark1<<" mark2"<<mark2;
		return head;
    }

    //取得节点个数
    quint32 num1, num2, num;
    num1 = (quint8)t1[start_para + 4] << 24 | (quint8)t1[start_para + 5] << 16
        | (quint8)t1[start_para + 6] << 8 | (quint8)t1[start_para + 7];
    num2 = (quint8)t2[start_data + 4] << 24 | (quint8)t2[start_data + 5] << 16
        | (quint8)t2[start_data + 6] << 8 | (quint8)t2[start_data + 7];
    if(num1 <= 0 || num2 <= 0)
    {
        qDebug()<<"fileToLink: ele num is empty or error， num1="<<num1<<" num2="<<num2;
		return head;
    }
    num = (num1 < num2 ? num1 : num2);
    qDebug()<<"num="<<num;

    //填充到链表节点
    int index;
    OptLink *node;
	STRUCT_PARA_TABLE_ELEMENT ele;
    int size = sizeof(STRUCT_PARA_TABLE_ELEMENT);
    quint32 seek1, seek2;
    index = 0;
    QByteArray data_block, byte_tmp;
    char data_unique[UNIQUE_CODE_SIZE + 1];
    quint32 data_len;

    seek1 = start_para + 8;
    seek2 = start_data + 8;

    //qDebug()<<"=================="<<t2;
    for(i = 0; i < num; i++)
    {
        node = opt_link_int->linkAddNode(head);
        if(node == NULL)
        {
            qDebug()<<"fileToLink: node is NULL";
            break;
        }
        //取出结构体
        memset((void *)(&ele), 0, size);
        //qDebug()<<"seek1="<<seek1;
        byte_tmp.clear();
        byte_tmp.append(t1.mid(seek1, size));
        memcpy((void *)(&ele), byte_tmp.data(),size);
        //qDebug()<<"ele: opt"<<ele.opt<<" file_name="<<ele.file_name<<" unique="<<ele.unique;
        seek1 += size;
        node->opt = ele.opt;
        node->obj.clear();
        node->obj.append(ele.file_name);
        strcpy(node->unique, ele.unique);
        node->index = index;
		node->time = ele.time;
        //qDebug()<<"node->opt="<<node->opt<<" obj="<<node->obj<<" unique="<<node->unique<<" index"<<node->index;
        //取出数据
        data_len = (quint8)t2[seek2] << 24 | (quint8)t2[seek2 + 1] << 16 |
        (quint8)t2[seek2 + 2] << 8 | (quint8)t2[seek2 + 3];
        //qDebug()<<"seek2="<<seek2<<"  data_len="<<data_len;//seek2 += data_len;
        for(j = 0; j < UNIQUE_CODE_SIZE + 1; j++)
        {
            data_unique[j] = t2[seek2 + 8 + j];
        }

        if(strcmp(ele.unique, data_unique) != 0)
        {
            qDebug()<<"fileToLink: unique code is not equal";
            opt_link_int->linkDelNode(head);
            continue;
        }
        data_block.clear();
        data_block = t2.mid(seek2 + 8 + UNIQUE_CODE_SIZE + 1, data_len - 8 - UNIQUE_CODE_SIZE - 1);
        seek2 += data_len;
        node->content.clear();
        node->content.append(data_block);
        index++;
    }
    return head;
}

//将参数表解析成链表（从服务器端代码移植过来）
int FileOpt::paraTable_toOptLink(QString para_name, OptLink *out_head)
{
	int i;
	//qDebug()<<"====================>A para_name="<<para_name;
	QFile f(para_name);

	//文件长度获取和存在性检测
	if(f.exists() == false)
	{
		qDebug()<<"paraTable_toOptLink: file not exist";
		return -1;
	}


	//起始码校验以及获取节点数量
	QByteArray block;
	int len;
	f.open(QIODevice::ReadOnly);
	block = f.readAll();
	//qDebug()<<"====================>A size="<<block.length();
	if(block.length() < 8)
	{
		qDebug()<<"paraTable_toOptLink: file is too short";
		return -1;
	}
	if((unsigned char)(block[0]) != START_CODE_0 ||
	    (unsigned char)(block[1]) != START_CODE_1 ||
	    (unsigned char)(block[2]) != START_CODE_2 ||
	    (unsigned char)(block[3]) != START_CODE_3 )
	 {
		qDebug()<<"paraTable_toOptLink: fiel format is error";
		return -1;

	 }
	 int num = (unsigned char)(block[4]) >> 24 |
	            (unsigned char)(block[5]) >> 16 |
	            (unsigned char)(block[6]) >> 8 |
	            (unsigned char)(block[7]) ;
	 if(num < 0 )
	 {
		qDebug()<<"paraTable_toOptLink: num get error";
		return -1;
	 }

	 //创建链表（请确保初始化--linkOptInit已完成）
	 opt_link_int->linkDelNodeAll(out_head);
	 int size = sizeof(STRUCT_PARA_TABLE_ELEMENT);
	 STRUCT_PARA_TABLE_ELEMENT ele;
	 OptLink *node;
	 int seek =8;
	 if(size * num > len)
	 {
		num = len / size;
		if(num < 1)//表明没有节点数据
		{
			return 0;
		}
	 }
	 for(i = 0; i < num; i++)
	 {
		memset((void *)(&ele), 0, size);
		my_memcpy((void *)(block.data()), (void *)(&ele), seek, size);


		//printf("====>ele  name=%s  index=%d  opt=%d  unique=%s\n", ele.file_name, ele.index, ele.opt, ele.unique);
		node = opt_link_int->linkAddNode(out_head);
		if(node == NULL)
		{
			continue;
		}
		node->index = ele.index;//这个成员比较无用，可以考虑删除
		node->obj.clear();
		node->obj.append((char *)(ele.file_name));
		strcpy(node->unique, ele.unique);
		node->opt = ele.opt;
		node->time = ele.time;

		seek += size;
	 }

	 return 0;
}

//文件操作，并存于链表
int FileOpt::manageAndSave(int opt, QString name, QByteArray content)
{
    int ret;
    ret = manage(opt, name, content);
    if(opt_link_head != NULL && ret == 0)//ret == 0 代表操作成功
    {
        OptLink *node = opt_link_int->linkAddNode(opt_link_head);
        if(node != NULL)
        {
            node->opt = opt;
            node->obj.clear();
            node->obj.append(name);//不包含路径
            node->content.clear();
            node->content.append(content);
        }
    }

    return 0;
}

//执行链表的所有动作
int FileOpt::exeLinkAction(OptLink *head)
{
	if(head == NULL)
	{
		qDebug()<<"exeLinkAction: head is NULL";
		return -1;
	}
    OptLink *prob = head->next;
    while(prob != NULL)
    {
        //qDebug()<<"==>prob unique="<<prob->unique;
        linkNodeAction(prob);
        prob = prob->next;
    }
    return 0;
}

//回退链表的所有动作，并删除所有节点（应该从尾端开始执行）
int FileOpt::backLinkAction(OptLink *head)
{
    if(head == NULL)
    {
        return -1;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {
            linkBackAction(prob);
            delete prob;
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }
    return 0;
}

//################################################## 文件同步处理
//云端参数表和本地参数表的比较分析
int FileOpt::paraTableAnalyze(QString local_p_table, QString local_d_table, QString download_tab)
{
//	qDebug()<<"=========>local_p_table="<<local_p_table;
//	qDebug()<<"=========>local_d_table="<<local_d_table;
//	qDebug()<<"=========>download_tab="<<download_tab;

	OptLink *local_head = opt_link_int->linkCreate();
	local_head = fileToLink(local_p_table, local_d_table);
	qDebug()<<"======================>local_head :";
	opt_link_int->show(local_head);

	OptLink *download_head = opt_link_int->linkCreate();
	paraTable_toOptLink(download_tab, download_head);
	qDebug()<<"======================>download_head :";
	opt_link_int->show(download_head);


	//得到云端多于客户端的部分
	OptLink *diff_server_head = opt_link_int->linkCreate();
	OptLink *prob1 = download_head->next;
	OptLink *prob2;
	int mark;
	while(prob1 != NULL)
	{

		mark = 0;
		prob2 = local_head->next;
		while(prob2 != NULL)
		{
			if(strcmp(prob1->unique, prob2->unique) == 0 && prob1->time == prob2->time)//时间戳和唯一码都相等
			{
				mark = 1;
				break;
			}
			prob2 = prob2->next;
		}
		if(mark == 0)
		{
			OptLink *node = opt_link_int->linkAddNode(diff_server_head);
			node->Optlink(prob1);
		}
		prob1 = prob1->next;
	}
	//得到客户端多于云端的部分
	OptLink *diff_client_head = opt_link_int->linkCreate();
	prob1 = local_head->next;
	while(prob1 != NULL)
	{
		mark = 0;
		prob2 = download_head->next;
		while(prob2 != NULL)
		{
			if(strcmp(prob1->unique, prob2->unique) == 0 && prob1->time == prob2->time)//时间戳和唯一码都相等
			{
				mark = 1;
				break;
			}
			prob2 = prob2->next;
		}
		if(mark == 0)
		{
			OptLink *node = opt_link_int->linkAddNode(diff_client_head);
			node->Optlink(prob1);
		}
		prob1 = prob1->next;
	}
	//得到合并冲突的节点(包括客户端参数表 和 云端参数表的节点，如果操作的对象相同，那么冲突)
	OptLink *conflict_head = opt_link_int->linkCreate();
	prob1 = diff_client_head->next;
	while(prob1 != NULL)
	{
		mark = 0;
		prob2 = diff_server_head->next;
		while(prob2 != NULL)
		{
			if(prob1->obj.compare(prob2->obj) == 0)
			{
				mark = 1;
			}
			prob2 = prob2->next;
		}
		if(mark == 0)
		{
			OptLink *node = opt_link_int->linkAddNode(conflict_head);
			node->Optlink(prob1);
		}
		prob1 = prob1->next;
	}

	//显示
	qDebug()<<"======================>diff_client_head :";
	opt_link_int->show(diff_client_head);
	qDebug()<<"======================>diff_server_head :";
	opt_link_int->show(diff_server_head);
	qDebug()<<"======================>conflict_head :";
	opt_link_int->show(conflict_head);

	// 输出
	out_diff_server_head = diff_server_head;
	out_diff_client_head = diff_client_head;
	out_conflict_head = conflict_head;

	return 0;
}

//同步操作
int FileOpt::syncFile()
{
	//从云端来的更新(如果参数表和数据表没有一一映射，可能会丢失某些节点)
    OptLink *server_head = fileToLink(current_para_table_download, current_data_table_download);
	qDebug()<<"=====================>syncFile: ";
    opt_link_int->show(server_head);
	//-1 追加到本地
	OptLink *local_head = fileToLink(current_para_table_local, current_data_table_local);
	opt_link_int->linkAppend(local_head, server_head);
	linkToFile(local_head, current_para_table_local, current_data_table_local);
	//-2 执行
	exeLinkAction(server_head);
	opt_link_int->linkDestory(local_head);

	//冲突处理
	// -1 复制冲突文件到备份
	OptLink *prob = out_conflict_head->next;
	QString tmp1, tmp2;
	QByteArray t;
	QFile file1, file2;
	while(prob != NULL)
	{
		tmp1.clear();
		tmp1.append(current_user_path);
		tmp1.append("/");
		tmp1.append(prob->obj);
		tmp2.clear();
		tmp2.append(current_back_path);
		tmp2.append("/");
		tmp2.append(prob->obj);
		file1.setFileName(tmp1);
		file2.setFileName(tmp2);
		if(false == file1.open(QIODevice::ReadOnly))
		{
			qDebug()<<"syncFile: file open error, file="<<tmp1;
			prob = prob->next;
            file1.close();
            file2.close();
			continue;
		}
		if(false == file2.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			qDebug()<<"syncFile: file open error, file="<<tmp2;
			prob = prob->next;
            file1.close();
            file2.close();
			continue;
		}
		t = file1.readAll();
		file2.write(t);
		file1.close();
		file2.close();

		prob = prob->next;
	}
	//-2 以本地更新覆盖云端的更新
	exeLinkAction(out_conflict_head);

	return 0;
}


//###################################################接口函数

//解析链表节点动作，并执行
int FileOpt::linkNodeAction(OptLink *node)
{
	QString tmp;
	tmp.clear();
	tmp.append(current_user_path);
	tmp.append("/");
	tmp.append(node->obj);
	return manage(node->opt, tmp, node->content);
}

//回退链表节点动作，并删除链表节点
int FileOpt::linkBackAction(OptLink *node)
{
    int ret;
	QString tmp;
	tmp.clear();
	tmp.append(current_user_path);
	tmp.append("/");
	tmp.append(node->obj);
    if(node != NULL && node->opt == fileOptCode_new)
    {
        //新建的回退动作是删除
		ret = manage(fileOptCode_del, tmp, node->back);
        //opt_link_int->linkDelNode_byIndex();
    }
    else if(node != NULL && node->opt == fileOptCode_cover)
    {
        //覆盖的回退动作还是覆盖
		ret = manage(fileOptCode_cover, tmp, node->back);

    }
    else if(node != NULL && node->opt == fileOptCode_del)
    {
        //删除的动作是新建
		ret = manage(fileOptCode_new, tmp, node->back);
    }
    else
    {
        return -1;
    }


    return ret;
}

//判断文件是否存在
bool FileOpt::fileExist(QString name)
{
    QFile file(name);
    return file.open(QIODevice::ReadOnly);
}
//判断文件夹是否存在
bool FileOpt::floderExist(QString name)
{
    QDir dir;
    return dir.cd(name);
}

//文件操作管理
int FileOpt::manage(int opt, QString name, QByteArray content)
{
    int ret;
    if(opt == fileOptCode_new)
    {
        ret = createFile(name, content);
    }
    else if(opt == fileOptCode_cover)
    {
        ret = coverFile(name, content);
    }
    else if(opt == fileOptCode_del)
    {
        ret = delFile(name);
    }
    return ret;
}

//新建文件
int FileOpt::createFile(QString name, QByteArray content)
{

	if(fileExist(name) == true)//文件存在，怎么处理？
    {
		qDebug()<<"createFile: file exist,abort! file"<<name;
//        tmp.clear();
//        tmp.append(back_path);
//        tmp.append(name.append(".back"));
//        QFile f1(tmp);
//        if(true == f1.open(QIODevice::ReadWrite | QIODevice::Truncate))
//        {
//            f1.write(content);
//        }
//        f1.close();
        return -1;
    }

	QFile f(name);
    if(true == f.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        f.write(content);
    }
    f.close();
    return 0;
}
//覆盖文件
int FileOpt::coverFile(QString name, QByteArray content)
{
	QFile f(name);
    if(true == f.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        f.write(content);
    }
    f.close();
    return 0;

}
//删除文件
int FileOpt::delFile(QString name)
{
	QFile f(name);
    if(true == f.remove())
    {
        return 0;
    }
    return -1;
}







