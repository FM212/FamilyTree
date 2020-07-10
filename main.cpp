/*代码作者:小狐狸*/
//
//代码仍有部分缺陷，由于个人技术的原因，没有使用层次遍历实现打印所有成员信息
//打印家族树和存入文件只会存储嫡系子代的信息
//注：Findloc函数在本代码中的使用率很高，你可以使用该函数来查询某个成员在家族树中的位置(返回一个指针).
//	  代码末尾有附带部分可用函数，若需要扩展功能时将函数放置main主函数之前即可
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;
#define OK 1
#define ERROR -1
#define Max 200 //字符串数组最大值
typedef struct TNode{
    string Wife;//妻子的姓名,若没有妻子时为空
    string Husband;//丈夫的姓名,若没有丈夫时为空
    struct TNode *parent;//双亲指针
    struct TNode *brother;//兄弟指针
    struct TNode *children;//孩子指针,仅指向最年长的孩子
}tree,*Tree;//三叉链表
int AddChild(Tree &Now,string Husbandname,string Wifename);//添加孩子到双亲节点Now
int AddZuXian(Tree &T,string Husbandname,string Wifename);//添加祖先
void CaiDanMain();//主菜单界面
int CheckCunZai(Tree T);//判断某个节点是否有成员,有则返回OK,否则返回ERROR
int CheckFile(string file);//判断文件是否已经存在,存在返回OK，否则返回ERROR
int ChangeZuXian(Tree &T,string Husbandname,string Wifename);//修改家族的祖先
int ChangeNow(Tree &Now,string Husbandname,string Wifename);//修改当前节点的信息，应保证当前节点存在
void Findloc(Tree T,string &Husbandname,string &Wifename,Tree &Now);//通过丈夫和妻子的名字查找到对应的节点，Now为记录该节点的指针,递归循环(深度遍历)
void PrintTree(Tree T);//将家族树的嫡系信息输出
int PrintChild(Tree Now);//显示某个节点的所有孩子,Now为当前节点
int PrintParent(Tree Now);//显示当前节点的双亲信息,Now为当前节点
int PrintZuXian(Tree Now);//显示祖先,Now为当前节点
int PrintBrother(Tree Now);//显示兄弟,Now为当前节点
int PrintLater(Tree Now);//查询当前节点的所有嫡系后代的信息,不包括当前节点,Now为当前节点
int InitTree(Tree &T);//初始化家族树
int InputFile(Tree T,string file);//将家族树存入文件中
int UploadFile(Tree &T,string file);//从已有的文件中将家族树信息覆盖

int main()
{
    Tree T;
    InitTree(T);//初始化
    while(1)
    {
        //fox存储用户输入的选项变量,初始值为0
        CaiDanMain();//主菜单显示
        int fox=0;//选项的选择
        cout<<"请选择: ";
        cin>>fox;
        if(CheckCunZai(T)==ERROR&&(fox>3&&fox<13))//若祖先不存在时先输入
        {
            cout<<"请先添加祖先!"<<endl;
            continue;
        }
        switch(fox)
        {
            case 0:return 0;//退出系统
            case 1://选择已有文件覆盖当前家族树
                {

                    while(1){
                        //truth1变量值为1时表示文件导入成功，并退出循环;值为0时文件导入失败，显示提示信息。初始值为0
                        string file;
                        cout<<"请输入需要导入的家族姓氏: ";
                        cin>>file;
                        int truth1=0;
                        truth1=UploadFile(T,file);
                        if(truth1==OK)//从文件中导入家族树成功时
                        {
                            cout<<"导入成功!"<<endl;
                            break;
                        }
                        else
                        {
                            //truth2值为1时，继续输入家族姓氏；值为0时退出，初始值为1
                            cout<<"导入失败!退出按0，重新输入家族姓氏按1"<<endl;
                            int truth2=1;
                            cin>>truth2;
                            if(truth2==0) break;
                        }
                    }
                }
                break;
            case 2://将当前家族树存入文件中
                {
                    //flag值为1时，重新输入家族姓氏；值为0时退出，初始值为1
                    string file;
                    int flag=1;
                    while(1)
                    {
                    cout<<"请输入家族姓氏作为文件名: ";
                    cin>>file;
                    if(CheckFile(file)==OK)//文件名存在时
                    {
                        cout<<"该家族姓氏已存在，退出按0，重新输入家族姓氏按1"<<endl;
                        cin>>flag;
                        if(flag==0)
                            break;
                        else continue;
                    }
                    if(CheckFile(file)==ERROR) break;
                    }
                    if(flag==0)//退出操作
                        break;
                    if(CheckFile(file)==ERROR)//文件名不存在时
                    {
                        InputFile(T,file);//将家族树导入文件
                        cout<<"操作成功!"<<endl;
                    }
                }
                break;
            case 3://添加祖先
                {
                    if(CheckCunZai(T)==OK)//若祖先已存在时
                    {
                        cout<<"祖先已存在!"<<endl;
                    }
                    else//不存在时，输入祖先信息
                    {
                        string Husbandname,Wifename;
                        cout<<"祖父(祖先): ";
                        cin>>Husbandname;
                        cout<<"祖母(祖先): ";
                        cin>>Wifename;
                        AddZuXian(T,Husbandname,Wifename);
                    }

                }
                break;
            case 4://添加家族新成员
                {
                    //flag1值为0时退出，为非0值时输入后代信息，初始值为0
                    int flag1=0;//若值为0时->退出此操作
                    string Fathername,Mothername,Husbandname,Wifename;
                    while(1)//当输入的父亲和母亲找到时退出循环
                    {
                        //flag2值为0时退出，为非0值时重新输入双亲信息，初始值为1
                        cout<<"父亲: ";
                        cin>>Fathername;
                        cout<<"母亲: ";
                        cin>>Mothername;
                        Tree truth=NULL;//为NULL时->显示提示信息,不为NULL时->输入成员信息
                        Findloc(T,Fathername,Mothername,truth);
                        if(truth==NULL)
                        {
                            int flag2=1;
                            cout<<"未找到双亲，退出按0，重新输入双亲按1"<<endl;
                            cin>>flag2;
                            if(flag2==0) break;
                            else continue;
                        }
                        else
                        {
                            cout<<"查找成功!可输入成员信息"<<endl;
                            flag1=1;//查找成功时赋值为1，执行输入成员信息
                            break;
                        }
                    }
                    if(flag1==0) break;//若为退出时，返回主界面
                    Tree Now;//临时指针
                    cout<<"丈夫: ";
                    cin>>Husbandname;
                    cout<<"妻子: ";
                    cin>>Wifename;
                    Findloc(T,Fathername,Mothername,Now);//寻找双亲的节点
                    AddChild(Now,Husbandname,Wifename);//添加孩子
                    while(1)
                    {
                        int flag3;//值为0时退出循环
                        cout<<"输入其兄弟按1，退出系统按0"<<endl;
                        cin>>flag3;
                        if(flag3==0)
                            break;
                        cout<<"丈夫: ";
                        cin>>Husbandname;
                        cout<<"妻子: ";
                        cin>>Wifename;
                        AddChild(Now,Husbandname,Wifename);//直接使用双亲节点
                    }
                }
                break;
            case 5://修改家族祖先
                {
                    string Husbandname,Wifename;
                    cout<<"祖父(祖先): ";
                    cin>>Husbandname;
                    cout<<"祖母(祖先): ";
                    cin>>Wifename;
                    ChangeZuXian(T,Husbandname,Wifename);
                    cout<<"修改成功！"<<endl;
                }
                break;
            case 6://修改某个家族成员
                {
                    //flag值为0时退出，值为1时重新输入成员信息并查找该成员，初始值为1
                    string Husbandname,Wifename;
                    int flag=1;
                    while(1){
                        //truth值为NULL时不存在该成员，不为NULL时成功找到成员信息,初始值为NULL
                        cout<<"丈夫(旧): ";
                        cin>>Husbandname;
                        cout<<"妻子(旧): ";
                        cin>>Wifename;
                        Tree truth=NULL;
                        Findloc(T,Husbandname,Wifename,truth);
                        if(truth)//存在该成员时
                        {
                            cout<<"查找成功!"<<endl;
                            break;
                        }
                        else//不存在该成员时
                        {
                            cout<<"查找失败，退出按0，重新输入按1!"<<endl;
                            cin>>flag;
                            if(flag==0) break;
                        }
                    }
                    if(flag==0)break;//退出操作
                    Tree Now;//临时指针
                    Findloc(T,Husbandname,Wifename,Now);
                    cout<<"丈夫(新): ";
                    cin>>Husbandname;
                    cout<<"妻子(新): ";
                    cin>>Wifename;
                    ChangeNow(Now,Husbandname,Wifename);
                }
                break;
            case 7://显示家族的祖先
                {
                    PrintZuXian(T);
                }
                break;
            case 8://显示某个成员的双亲
                {
                    //Now为NULL时，无成员信息；不为NULL时，有成员信息，初始值为NULL
                    string Husbandname,Wifename;
                    cout<<"丈夫: ";
                    cin>>Husbandname;
                    cout<<"妻子: ";
                    cin>>Wifename;
                    Tree Now=NULL;//临时指针
                    Findloc(T,Husbandname,Wifename,Now);
                    if(Now)
                        PrintParent(Now);
                    else cout<<"该成员信息不存在，无法找出双亲!"<<endl;;
                }
                break;
            case 9://显示兄弟
                {
                    while(1){
                        //Now为NULL时，无成员信息；不为NULL时，有成员信息，初始值为NULL
                        string Husbandname,Wifename;
                        cout<<"丈夫: ";
                        cin>>Husbandname;
                        cout<<"妻子: ";
                        cin>>Wifename;
                        Tree Now=NULL;//临时指针
                        Findloc(T,Husbandname,Wifename,Now);
                        if(Now)//找到信息时
                        {
                            PrintBrother(Now);
                            break;
                        }
                        else//未找到成员信息时
                        {
                            //truth值为0时退出，非0时重新输入成员信息。初始值为1
                            cout<<"无该成员信息，退出按0，重新输入信息按1"<<endl;
                            int truth=1;
                            cin>>truth;
                            if(truth==0)
                                break;
                        }
                    }
                }
                break;
            case 10://显示孩子
                {
                    while(1)
                        {
                        //Now为NULL时，无成员信息；不为NULL时，有成员信息，初始值为NULL
                        string Husbandname,Wifename,Sonname,SonWifename;
                        cout<<"丈夫: ";
                        cin>>Husbandname;
                        cout<<"妻子: ";
                        cin>>Wifename;
                        Tree Now=NULL;//临时指针
                        Findloc(T,Husbandname,Wifename,Now);//找到节点位置
                        if(Now)//找到节点时
                        {
                            if(PrintChild(Now)==ERROR)
                            {
                                cout<<"该成员无孩子!，退出按0，重新输入信息按1"<<endl;
                                int truth=0;
                                cin>>truth;
                                if(truth==0)
                                    break;
                            }
                            else break;
                        }
                        else//未找到成员信息时
                        {
                            cout<<"无该成员信息，退出按0，重新输入信息按1"<<endl;
                            int truth=0;
                            cin>>truth;
                            if(truth==0)
                                break;
                        }
                    }
                }
                break;
            case 11://显示所有后代
                {
                    while(1)
                    {
                        //Now为NULL时，无成员信息；不为NULL时，有成员信息，初始值为NULL
                        string Husbandname,Wifename;
                        cout<<"丈夫: ";
                        cin>>Husbandname;
                        cout<<"妻子: ";
                        cin>>Wifename;
                        Tree Now=NULL;
                        Findloc(T,Husbandname,Wifename,Now);
                        if(Now)//存在时
                        {
                            PrintLater(Now);
                            break;
                        }
                        else//不存在该成员时
                        {
                            cout<<"无该成员信息，退出按0，重新输入信息按1"<<endl;
                            int truth=0;
                            cin>>truth;
                            if(truth==0)
                                break;
                        }
                    }
                }
                break;
            case 12://显示家族树
                {

                    PrintTree(T);
                }
                break;
            case 13://清屏操作
                {
                    system("cls");
                }
                break;
            default:break;
        }
    }
    return 0;
}
int AddChild(Tree &Now,string Husbandname,string Wifename)//添加孩子到双亲节点Now
{
    if(Now->children==NULL)//没有孩子时
    {
        Tree p;//创建新的节点
        p=new TNode;
        p->brother=NULL;
        p->children=NULL;
        p->Husband=Husbandname;
        p->Wife=Wifename;
        p->parent=Now;
        Now->children=p;
    }
    else//有孩子时
    {
        Tree now;
        now=Now->children;//指向主孩子
        while(now->brother!=NULL)//在兄弟中进行循环操作
            now=now->brother;
        Tree p;//创建新的节点
        p=new TNode;
        p->brother=NULL;
        p->children=NULL;
        p->Husband=Husbandname;
        p->Wife=Wifename;
        p->parent=Now;
        now->brother=p;//now此时为末位的兄弟节点
    }
    return OK;
}
int AddZuXian(Tree &T,string Husbandname,string Wifename)//添加祖先
{
    Tree t;
    t=T;
    t->Husband=Husbandname;
    t->Wife=Wifename;
    t->brother=NULL;
    t->parent=NULL;
    t->children=NULL;
    return OK;
}

void CaiDanMain()//主菜单界面
{
    cout<<endl;
    cout<<"-----------------------------<主菜单>----------------------------------"<<endl;
    cout<<"|    0 退出系统                                                        |"<<endl;
    cout<<"|    1 选择已有家族谱覆盖当前家族树                                    |"<<endl;
    cout<<"|    2 将嫡系家族树存入文件                                            |"<<endl;
    cout<<"|    3 添加祖先                                                        |"<<endl;
    cout<<"|    4 添加家族新成员[最年长的兄弟应首先输入，无妻子时请填写无]        |"<<endl;
    cout<<"|    5 修改家族祖先                                                    |"<<endl;
    cout<<"|    6 修改某个家族成员                                                |"<<endl;
    cout<<"|    7 显示家族的祖先                                                  |"<<endl;
    cout<<"|    8 显示某个成员的双亲                                              |"<<endl;
    cout<<"|    9 显示某个成员的兄弟                                              |"<<endl;
    cout<<"|   10 显示某个成员的孩子                                              |"<<endl;
    cout<<"|   11 显示某个成员的所有嫡系后代                                      |"<<endl;
    cout<<"|   12 显示嫡系家族树信息                                              |"<<endl;
    cout<<"|   13 清屏                                                            |"<<endl;
    cout<<"------------------------------------------------------------------------"<<endl;
    cout<<endl;
}
int CheckCunZai(Tree T)//判断某个节点是否有成员,有则返回OK,否则返回ERROR
{
    Tree t;//临时指针
    t=T;
    if(t->Husband==""&&t->Wife=="")//对当前节点进行判断
    {
        return ERROR;
    }
    else return OK;
}
int CheckFile(string file)//判断文件是否已经存在,存在返回OK，否则返回ERROR
{
    //flag为文件句柄，flag=串1.find(串2)
    //flag的返回值为string::npos时则表示串2不为串1的子串
    string::size_type flag;
    flag=file.find(".txt");
    if(flag==string::npos)//不含有后缀.txt时,添加后缀
        file+=".txt";
    FILE*fp;
    char filename[Max];
    strcpy(filename,file.c_str());
    if((fp=fopen(filename,"r"))==NULL)//不存在时返回ERROR
        return ERROR;
    else return OK;
}
int ChangeZuXian(Tree &T,string Husbandname,string Wifename)//修改家族的祖先
{
    Tree t;
    t=T;
    if(t==NULL)
        return ERROR;
    t->Husband=Husbandname;
    t->Wife=Wifename;
    return OK;
}
int ChangeNow(Tree &Now,string Husbandname,string Wifename)//修改当前节点的信息，应保证当前节点存在
{
    Tree now;
    now=Now;
    now->Husband=Husbandname;
    now->Wife=Wifename;
    return OK;
}
void Findloc(Tree T,string &Husbandname,string &Wifename,Tree &Now)//通过丈夫和妻子的名字查找到对应的节点，Now为记录该节点的指针,递归循环(深度遍历)
{
    if(T==NULL)
        return;
    else
    {
        if(Husbandname==T->Husband&&Wifename==T->Wife)//若当前节点符合条件则Now指向当前节点
            Now=T;
        Findloc(T->children,Husbandname,Wifename,Now);//先对孩子进行遍历
        Findloc(T->brother,Husbandname,Wifename,Now);//后对兄弟进行遍历
    }
}
void PrintTree(Tree T)//将家族树的嫡系信息输出
{
    //time表示第几代孩子，初始值为1
    Tree childs;//临时指针
    childs=T;
    int time=1;//第几代
    while(childs)//在孩子中进行循环
    {
        //cout<<"第"<<time<<"代:"<<endl;
        if(time!=1)
            cout<<"第"<<time<<"代:"<<endl;
        else//祖先节点时
            cout<<"第1代(祖先):"<<endl;
        Tree brothers;
        brothers=childs;
       while(brothers)//在兄弟中进行循环
       {
           cout<<brothers->Husband<<"(男) ";
           cout<<brothers->Wife<<"(女)"<<endl;
           brothers=brothers->brother;
       }
       time++;
       childs=childs->children;
    }
}
int PrintChild(Tree Now)//显示某个节点的所有孩子,Now为当前节点
{
    cout<<endl;
    Tree k;
    if(Now->children==NULL)//没有孩子时，返回错误
    {
        return ERROR;
    }
    k=Now->children;
    cout<<"[孩子]"<<endl;
    while(k)//在兄弟中进行循环
    {
        cout<<k->Husband<<"(男) ";
        cout<<k->Wife<<"(女)"<<endl;
        k=k->brother;
    }
    return OK;
}
int PrintParent(Tree Now)//显示当前节点的双亲信息,Now为当前节点
{
    cout<<endl;
    Tree now;
    now=Now;
    Tree parents=NULL;//parents为指向当前节点父母的指针
    if(now->parent==NULL)//若now为祖先节点时无父亲和母亲
    {
        cout<<"当前为祖先，没有双亲!"<<endl;
        return ERROR;
    }
    else parents=now->parent;
    cout<<"[父亲] ";
    cout<<parents->Husband<<endl;
    cout<<"[母亲] ";
    cout<<parents->Wife<<endl;
    return OK;
}
int PrintZuXian(Tree Now)//显示祖先,Now为当前节点
{
    Tree k;//临时指针
    k=Now;
    while(k->parent!=NULL)//当往上循环到没有双亲的结点时为祖先
    {
        k=k->parent;
    }
    cout<<"[祖先] "<<endl;//祖先共有两个人
    cout<<k->Husband<<"(男) ";
    cout<<k->Wife<<"(女)"<<endl;
    return OK;
}
int PrintBrother(Tree Now)//显示兄弟,Now为当前节点
{
    Tree k;//临时指针
    if(Now->parent==NULL)//若为祖先时
    {
        cout<<"祖先无兄弟!"<<endl;
        return ERROR;
    }
    k=Now->parent->children;//指向主孩子
    if(k->brother!=NULL)
    {
        cout<<"[兄弟] "<<endl;
    }
    else
        {
            cout<<"无兄弟!"<<endl;
            return ERROR;
        }
    while(k)//在节点的兄弟中进行循环输出
    {
        if(k->Husband!=Now->Husband&&k->Wife!=Now->Wife)//不输出当前节点的成员信息,仅输出兄弟的信息
        {
            cout<<k->Husband<<"(男) ";
            cout<<k->Wife<<"(女)"<<endl;
        }
        k=k->brother;
    }
    return OK;
}
int PrintLater(Tree Now)//查询当前节点的所有嫡系后代的信息,不包括当前节点,Now为当前节点
{
    //time表示第几代孩子,初始值为1
    Tree k;
    if(Now->children==NULL)//无孩子时
    {
        cout<<"无后代信息"<<endl;
        return ERROR;
    }
    k=Now->children;
    int time=1;//表示第几代的孩子
    while(k)//孩子中循环
    {
        Tree p;
        p=k;
        cout<<"第"<<time<<"代:"<<endl;
        while(p)//兄弟中循环
        {
            cout<<p->Husband<<"(男) ";
            cout<<p->Wife<<"(女)"<<endl;
            p=p->brother;
        }
        time++;
        k=k->children;
    }
    return OK;
}
int InitTree(Tree &T)//初始化家族树
{
    T=new TNode;//创建头节点
    T->Husband="";//初始化为空
    T->Husband="";
    T->children=NULL;
    T->brother=NULL;
    T->parent=NULL;
    return OK;
}
int InputFile(Tree T,string file)//将嫡系家族树存入文件中
{
    //flag为文件句柄，flag=串1.find(串2)
    //若串2不为串1子串时，flag值为string::npos
    if(CheckFile(file)==OK)//文件已存在时返回错误
        return ERROR;
    string::size_type flag;
    flag=file.find(".txt");
    if(flag==string::npos)//没有后缀时添加后缀
        file+=".txt";
    FILE*fp;
    char filename[Max];
    char Husbandname[Max],Wifename[Max];
    char Fathername[Max],Mothername[Max];
    strcpy(filename,file.c_str());
    fp=fopen(filename,"w");
    Tree k;//孩子指针
    k=T;
    while(k)
    {
        Tree p;//兄弟指针
        p=k;
        while(p)
        {
            if((p->Husband!=T->Husband)&&(p->Wife!=T->Wife))//不为祖先节点时,将父母信息输入
            {
                strcpy(Fathername,(p->parent->Husband).c_str());
                strcpy(Mothername,(p->parent->Wife).c_str());
                fprintf(fp,"%s ",Fathername);
                fprintf(fp,"%s\t",Mothername);
            }
            strcpy(Husbandname,(p->Husband).c_str());
            strcpy(Wifename,(p->Wife).c_str());
            fprintf(fp,"%s ",Husbandname);//写入当前节点信息
            fprintf(fp,"%s\n",Wifename);
            p=p->brother;
        }
        k=k->children;
    }
    fprintf(fp,"#\n");//结束标志
    fclose(fp);//关闭文件
    return OK;
}
int UploadFile(Tree &T,string file)//从已有的文件中将家族树信息覆盖,若该文件不存在则返回ERROR
{
    //flag为文件句柄，flag=串1.find(串2)
    //若串2不为串1子串时，flag值为string::npos
    string::size_type flag;//判断是否含有后缀.txt
    flag=file.find(".txt");
    if(flag==string::npos)//不含后缀时添加后缀.txt
        file+=".txt";
    char filename[Max];
    strcpy(filename,file.c_str());//将file复制到filename
    FILE*fp=fopen(filename,"r");//传入参数filename为字符数组类型
    if(fp==NULL)//文件不存在时
        return ERROR;
    char ZuFu[Max],ZuMu[Max];
    fscanf(fp,"%s",ZuFu);//将祖先信息存入
    fscanf(fp,"%s",ZuMu);
    string ZuFuname(ZuFu);
    string ZuMuname(ZuMu);
    AddZuXian(T,ZuFuname,ZuMuname);
    while(1)
    {
        char Father[Max],Mother[Max];
        char Husband[Max],Wife[Max];
        fscanf(fp,"%s",Father);
        string Fathername(Father);
        if(Fathername=="#")
            break;
        fscanf(fp,"%s",Mother);
        fscanf(fp,"%s",Husband);
        fscanf(fp,"%s",Wife);
        string Mothername(Mother);
        string Husbandname(Husband);
        string Wifename(Wife);
        Tree Now;
        Findloc(T,Fathername,Mothername,Now);//找到双亲节点
        if(Now)//存在双亲节点时
            AddChild(Now,Husbandname,Wifename);
    }
    fclose(fp);
    return OK;
}
/*
int CountBrother(Tree Now,string Husbandname,string Wifename)//统计当前节点兄弟的个数
{//可用
    int sum=0;//兄弟的个数
    Tree t;
    if(Now->parent==NULL)//当前为祖先时
        return 0;
    t=Now->parent->children;
    while(t)
    {
        if(t->Husband!=Husbandname&&t->Wife!=Wifename)
            sum++;
        t=t->brother;
    }
    return sum;
}
*/
/*
int InputTree(Tree &T)//输入家族成员到家族树中
{
    string Husbandname,Wifename,Fathername,Mothername;
    int flag1=2;//默认为需要重新输入双亲
    cout<<"祖父(祖先): "<<endl;
    cin>>Husbandname;
    cout<<"祖母(祖先): "<<endl;
    AddZuXian(T,Husbandname,Wifename);//添加覆盖祖先
    cout<<"家庭成员输入前，请输入其父母的信息！"<<endl<<endl;
    while(1)//根据父亲和母亲的名字输入家庭成员
    {
        if(flag1==2){//当输入新成员时需要重新输入父母信息
            cout<<"请输入父亲:"<<endl;
            cin>>Husbandname;
            cout<<"请输入母亲:"<<endl;
            cin>>Wifename;
        }

        Tree Now=NULL;//临时节点指针
        Findloc(T,Fathername,Mothername,Now);//从家族树中寻找符合父母亲的节点,用Now保存双亲节点
        int flag2=1;
        if(Now==NULL)//若家族树中无该双亲时
        {
            cout<<endl;
            cout<<"未查询到该双亲!"<<endl;
            cout<<"-------------------------"<<endl;
            cout<<"|    0 退出输入         |"<<endl;
            cout<<"|    1 重新输入双亲     |"<<endl;
            cout<<"|    2 打印已有家族树   |"<<endl;
            cout<<"-------------------------"<<endl;
            cout<<endl;
            cin>>flag2;
            int time=1,truth=1;//time表示第几代,truth表示递归中当前节点与前者的关系
            switch(flag2)
            {
                case 0:return OK;
                case 1:
                    {
                        cout<<"父亲: ";
                        cin>>Fathername;
                        cout<<"母亲: ";
                        cin>>Mothername;
                    }
                    break;
                case 2:
                    {
                        int time=1,flag=1;
                        PrintTree(T,time,flag);
                    }
                    break;
                default:break;
            }
            if(flag2==2) continue;
        }
        else cout<<"成功找到双亲!可输入成员信息"<<endl;
        string Husband,Wife;
        cout<<endl;
        cout<<"丈夫:"<<endl;
        cin>>Husband;
        cout<<"妻子:"<<endl;
        cin>>Wife;
        if(CheckChildren(Now)==ERROR)//若没有孩子时，直接插入到父母节点之后
        {
            AddChild(Now,Husband,Wife);
        }
        if(CheckChildren(Now)==OK)//若有孩子时，插入孩子的兄弟节点的后面
        {
            AddBrother(Now->children,Husband,Wife);//传入孩子节点
        }
        cout<<endl;
        cout<<"------------------------"<<endl;
        cout<<"|    0 退出输入        |"<<endl;
        cout<<"|    1 输入兄弟        |"<<endl;
        cout<<"|    2 输入新的成员    |"<<endl;
        cout<<"------------------------"<<endl<<endl;
        cin>>flag1;
        switch(flag1)
        {
            case 0:return OK;
            case 1:
                {

                }
                break;
            case 2:
                {

                }
                break;
            default:break;
        }
    }
    return OK;
}*/
/*
int AddBrother(Tree &Now,string Husbandname,string Wifename)//添加新成员到兄弟节点之后,前提：Now应为其兄弟节点
{
    Tree now;//临时指针
    now=Now;
    while(now->brother!=NULL)//从Now节点的孩子中找到需要插入的位置,即兄弟节点的最后一个位置
        now=now->brother;
    Tree p;
    p=new TNode;//创建一个节点
    p->Husband=Husbandname;
    p->Wife=Wifename;
    p->brother=NULL;
    p->parent=Now;
    p->children=NULL;
    //双亲节点的孩子指向最年长的孩子
}
*/
/*
int CheckChildren(Tree Now)//检测某个节点是否有孩子,有则返回OK，否则返回ERROR
{//可用
    Tree now;
    now=Now->children;
    if(now->Husband!=""&&now->Wife!="")
        return OK;
    else return ERROR;
}
int CheckBrother(Tree Now)//检测某个节点是否有兄弟，有则返回OK，否则返回ERROR
{//可用
    Tree k;//临时指针
    if(Now->parent!=NULL)//不为祖先时,指向主孩子
     {
         k=Now->parent;//指向双亲
         k=k->children;//指向兄弟
     }
    else k=Now;
    if(k->brother==NULL)
        return ERROR;
    return OK;
}
*/

