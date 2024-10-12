//主函数区
//#include "enemy.cpp"
#include <easyx.h>
#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h>
#pragma comment (lib, "Winmm.lib")
//using namespace Enemy;


/******宏定义*****/
#define MAXJET 10
#define MAXENEMY 5     //所有敌机种类数
#define WIDTH 900
#define LENGTH 1500
#define plusgradenum 11     //当前最大等级数+1
#define RIGHT 1100
#define DDL 100
#define MMAXNUMENEMY 20

/*******其他处理******/
char c,txt[30],key;
bool gameon = 1;
bool gamebegin = 0;
bool goon;
int MAXSORTENEMY;   //当前敌机种类范围
int MAXNUMENEMY = 7;  //当前最大敌机数
bool kd_a , kd_d , kd_w ,kd_s,kd_esc,kd_enter;
ExMessage msg;

/******结构定义*****/

//子弹
struct JET
{
    int x, y;				// 当前坐标
    bool shoot;				// 发射状态
    int size;               // 大小

    DWORD t1, t2, dt;		// 发射速度
    IMAGE img[2];			// 储存子弹一亮一暗图片
    byte n : 1;				// 图片下标
}jet[MAXJET];

//敌机
//所有敌机，见"enemy.cpp"文件夹
struct ENEMY1       //所有敌机
{
    bool boss = 0;              //是否boss
    int emergnum = 0;				// 出现个数，boss专用（暂未设置）
    int blood;              //敌机最大血量
    int size;               //敌机大小
    int cblood;             //敌机初始最大血量（加成前）

    DWORD dt;		// 行动速度
    IMAGE img[2];			// 储存敌机一亮一暗图片
    byte n : 1;				// 图片下标

}enemy1[MAXENEMY];

struct ENEMY2       //当前的敌机
{   
    bool boss = 0;
    int x, y;				// 当前坐标
    int lx, ly;             //上一帧的坐标
    bool emerg;				// 出现状态
    int blood;              //敌机当前血量
    int to;                 //指向库中的敌机
    int size;               //敌机大小

    DWORD t1, t2, dt;	    	// 发射速度
    
}enemy2[MMAXNUMENEMY];


//自机
struct YOU
{
    int x, y;				// 当前坐标
    int blood;              //自机残机数（等于0时已经死亡）
    int powerplus[plusgradenum] = { 0,0,9,29,49,79,119,159,199,399,599 };         //子弹伤害加成，i表示等级
    int grade;
    int step;               //步长
    int power;              //子弹威力
    int multi;              //敌人的加成系数，注意是整数
    int score;
    int gsocre[plusgradenum] = {0,0,6000,10000,32000,256000,1024000,4096000,16384000,33554432,99999999};              //到达该等级所需要的分数

    DWORD t1, t2, dt;		// 子弹发射时间间隔
    DWORD et1, et2, edt;     //敌人出现时间间隔
    IMAGE img[2];			// 储存自机一亮一暗图片
    byte n : 1;				// 图片下标

}you;
//其他图像
struct IMAGEE
{
    IMAGE mainpicture;
    IMAGE lose;
}imagee;



/**** 函数声明区 ****/
void welcome();
void mainmenu();        //主菜单
void Initj(int);		// 初始化子弹
void Inite(int,int);    //初始化敌机
void Load();		    // 加载图片
void Loadenemy();       //加载敌机初始数据
void cd(int,int);
void dcd(int,int);              //选择框处理
void fixedgamewindow();         //静态游戏界面(未编写完成)
void movinggamewindow();        //动态游戏界面(未编写完成)
void gamebeginpre();            //游戏开始预处理
void gamepause();               //游戏暂停
void shootj();                  //处理子弹
void shoote();                  //处理敌机
void handle();                  //处理碰撞与分数（未编写）
void handlegrade();             //处理等级相关（未编写）
void printt(int,int,int,int,int,int,int,int);           //输出文字（先设置格式）
void gameover();
void clear();                   //清除动态界面
void keydown();
void handleenemy(int, int);     //特殊敌人处理

int main() {
    
    Load();

    welcome();
    mciSendString(_T("open \"123.mp3\"type mpegvideo alias bk"), NULL, 0, NULL);
    mciSendString(_T("play bk from 0"),NULL,0,NULL);
    //正式开始
    while (gameon) {

        if (!gamebegin) {
            mainmenu();
        }

        if (!gameon)return 0;
        
        
        //开始游戏！
        //先初始化
        if (gamebegin) {
            gamebeginpre();
            for (int i = 0; i < MAXJET; i++) {
                Initj(i);
            }
            for (int i = 0; i < MMAXNUMENEMY; i++) {
                Inite(i,0);
            }
            cleardevice();
            setcolor(BLACK);
            settextstyle(25, 0, "楷体");
            outtextxy(450, 350, "初始化成功！按任意键进入开始战斗！");
            setcolor(RED);
            outtextxy(450, 400, "不要让敌人（作业）碰到红线（DDL）！");
            getmessage(&msg, EM_KEY);
            getmessage(&msg, EM_KEY);
            cleardevice();
        }

        //BeginBatchDraw();
        
        //处理静态游戏界面
        fixedgamewindow();

        
        while (gamebegin) {

            //处理游戏动态界面
            movinggamewindow();

            //cleardevice();
            putimage(you.x, you.y, &you.img[1]);
            
            //操作
            
            //keydown
            if (peekmessage(&msg, EM_KEY)) {
                
                keydown();
                kd_enter = 0;
            }

            //操作界面
            if (kd_esc) {
                    kd_esc = 0;
                    clear();
                    gamepause();
                    clear();
                    if (!goon)break;
                    else;
                    //BeginBatchDraw();
                }
            else {
                    putimage(you.x, you.y, &you.img[0]);
                    if (kd_w && you.y - you.step >= 200) {
                        kd_w = 0;
                        if (you.y - you.step == 200) {
                            setfillcolor(WHITE);
                            solidrectangle(0, 0, 400, 100);
                            setcolor(BLACK);
                            settextstyle(20, 0, "楷体");
                            outtextxy(0, 0, "想去哪呢");

                        }
                        else you.y -= you.step;
                    }
                    else if (kd_s && you.y + you.step <= WIDTH - 150) {
                        kd_s = 0;
                        if (you.y + you.step == WIDTH - 150) {
                            setfillcolor(WHITE);
                            solidrectangle(0, 0, 400, 100);
                            setcolor(BLACK);
                            settextstyle(20, 0, "楷体");
                            outtextxy(0, 0, "想去哪呢");

                        }
                        else you.y += you.step;
                    }
                    else if (kd_a && you.x - you.step >= -80) {
                        kd_a = 0;
                        if (you.x - you.step == -80) {
                            setfillcolor(WHITE);
                            solidrectangle(0, 0, 400, 100);
                            setcolor(BLACK);
                            settextstyle(20, 0, "楷体");
                            outtextxy(0, 0, "没有退路可言！");

                        }
                        else you.x -= you.step;
                    }
                    else if (kd_d && you.x + you.step <= RIGHT - 80) {
                        kd_d = 0;
                        if (you.x + you.step  >= RIGHT - 100) {
                            setfillcolor(WHITE);
                            solidrectangle(0, 0, 400, 100);
                            setcolor(BLACK);
                            settextstyle(20, 0, "楷体");
                            outtextxy(0, 0, "着急啥呢，卷不死你");

                        }
                        else you.x += you.step;
                    }
                    else;
                    putimage(you.x, you.y, &you.img[1]);



                    //}
                    //else;

                    

                }


            shootj();//操作子弹
    
            shoote();//此处处理敌机

            handle();//此处处理碰撞

            you.score+=5;
            handlegrade();//此处处理分数

            if (you.blood <= 0 || you.score>99999999) {
                cleardevice();
                gameover();
                if (!goon)break;
                else;
            }
            
            //FlushBatchDraw();	
            Sleep(40);
        }//游戏进行循环




    }//大循环

    return 0;
}


void welcome() {

    kd_a = 0, kd_d = 0, kd_w = 0, kd_s = 0, kd_esc = 0, kd_enter = 0;

    std::cout << "loading......\n";
    std::cout << "请将鼠标全程保持在白色窗口中进行键盘操作\n";
    std::cout << "若无反应，则有两种情况：\n";
    std::cout <<"1.请用鼠标点击白色窗口部分后重新点击键盘\n";
    std::cout << "2.你按错按键了（总之不可能是程序问题（划掉））\n";
    std::cout << "本程序支持的按键有：w,s,a,d,esc,enter(回车)\n";
    std::cout << "制作不易，一键三连（划掉）\n\n";
    std::cout << "版本号：1.0.2\n";
    std::cout << "作者：@koishi";
    std::cout << "特别鸣谢：\n";
    std::cout << "@离世九灵\n";
    std::cout << "@Ryaho of the Forggetten Vallery\n";
    std::cout << "其他参与人员：\n";
    std::cout << "@zzr\n";
    std::cout << "@cyh\n";
    std::cout << "@gzf\n";
    

    initgraph(LENGTH, WIDTH);
    setbkcolor(RGB(255, 255, 255));

    srand((unsigned int)time(0));

    mciSendString("open ./file/123.mp3 alias bk", 0, 0, 0);
    mciSendString("play bk repeat", 0, 0, 0);

    setcolor(BLACK);
    settextstyle(25, 0, "楷体");
    outtextxy(200, 600, "loading……");
    Sleep(3000);
    cleardevice();

    setcolor(BLACK);
    settextstyle(25, 0, "楷体");
    
    outtextxy(600, 300, "欢迎来到百丽宫!");
    setcolor(GREEN);
    settextstyle(20, 0, "楷体");
    outtextxy(200, 600, "请先将鼠标指针移到白色窗口中");
    outtextxy(200, 650, "点击回车进入百丽宫");
    outtextxy(200, 700, "若无反应则点击窗口白色部分");
    //if (peekmessage(&msg, EM_KEY) == 0)while (1);
    //if (!_kbhit())while (1)Sleep(30);
    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    cleardevice();

    setcolor(BLACK);
    settextstyle(25, 0, "楷体");

    outtextxy(300, 300, "欢迎来到百丽宫!");
    outtextxy(100, 100, "按任意键继续……");
    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    outtextxy(300, 350, "不幸的是，小五的假期快结束了");
    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    outtextxy(300, 400, "更不幸的是，大量作业的DDL和考试接踵而至");
    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    outtextxy(300, 450, "小五要去备考考试周了，不然他会破防");
    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    outtextxy(300, 500, "于是无助的小五把目光投向了你……");
    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    cleardevice();
    outtextxy(500, 300, "身为小五的好朋友，你愿意伸出援手帮助小五吗");
    outtextxy(200, 700, "操作：wsad上下左右，esc退出，enter确认");

    setcolor(BLACK);
    settextstyle(25, 0, "楷体");
    outtextxy(500, 500, "愿意");
    outtextxy(900, 500, "不愿意");
    bool bingo = 1;//正的顺序

    //选择框
    setlinecolor(GREEN);
    rectangle(500, 500, 575, 525);
    while (1) {
        getmessage(&msg, EM_KEY);
        keydown();

            if (kd_a||kd_d) {
                kd_a = 0, kd_d = 0;
                setfillcolor(WHITE);
                solidrectangle(500, 500, 1000, 600);
                if (bingo) {
                    setcolor(BLACK);
                    settextstyle(25, 0, "楷体");
                    outtextxy(500, 500, "不愿意");
                    outtextxy(900, 500, "愿意");
                    setlinecolor(GREEN);
                    rectangle(900, 500, 975, 525);
                    bingo = 0;
                }
                else {
                    setcolor(BLACK);
                    settextstyle(25, 0, "楷体");
                    outtextxy(500, 500, "愿意");
                    outtextxy(900, 500, "不愿意");
                    setlinecolor(GREEN);
                    rectangle(500, 500, 575, 525);
                    bingo = 1;
                }

            }
            else if (kd_esc) {
                kd_esc = 0;
                outtextxy(400, 100, "        小五面带微笑给你递来了一瓶热带风味冰红茶");
            }
            else if (kd_enter) {
                kd_enter = 0;
                cleardevice();
                outtextxy(500, 400, "     好兄弟还得是你！");
                outtextxy(100, 100, "按任意键继续……");
                getmessage(&msg, EM_KEY);
                getmessage(&msg, EM_KEY);
                break;

            }
            else {
                kd_w = 0, kd_s = 0;
                outtextxy(700, 250, "有点意思");
            }
        
    }


    setcolor(GREEN);
    settextstyle(40, 0, "楷体");
    outtextxy(500, 400, "勇敢小5，不怕DDL！");

    getmessage(&msg, EM_KEY);
    getmessage(&msg, EM_KEY);
    cleardevice();
    setcolor(GREEN);

    for (int i = 0; i <= 200; i += 10) {

        cleardevice();
        settextstyle(40, 0, "楷体");
        outtextxy(500, 400-i, "勇敢小5，不怕DDL！");
        Sleep(50);

    }

}

void cd(int x,int y) {
    setlinecolor(GREEN);
    rectangle(x, y, x + 200, y + 25);

}//选择框的绘制
void dcd(int x, int y) {
    setlinecolor(WHITE);
    rectangle(x, y, x + 200, y + 25);

}//选择框的清除

void mainmenu() {

    cleardevice();
    setcolor(GREEN);
    settextstyle(40, 0, "楷体");
    outtextxy(500, 200, "勇敢小5，不怕DDL！");
    putimage(200, 200, &imagee.mainpicture);

    setcolor(BLACK);
    settextstyle(25, 0, "楷体");
    outtextxy(700, 350, "开始");
    outtextxy(700, 400, "最高分（没做好）");
    outtextxy(700, 450, "结束");

    settextstyle(20, 0, "楷体");
    outtextxy(200, 700, "操作：wsad上下左右，esc退出，enter确认");
    settextstyle(15, 0, "楷体");
    outtextxy(50, 800, "copyright2024@koishi");

    //选择框
    int chox=700, choy=350;
    cd(chox, choy);
    while (1){
        getmessage(&msg, EM_KEY);
        keydown();

            if (kd_w) {
                kd_w = 0;
                dcd(chox, choy);
                if (choy == 350) {
                    choy = 450;
                }
                else choy -= 50;
                cd(chox, choy);
            }
            else if (kd_s) {
                kd_s = 0;
                dcd(chox, choy);
                if (choy == 450) {
                    choy = 350;
                }
                else choy += 50;
                cd(chox, choy);
            }
            else if (kd_esc) {
                kd_esc = 0;
                dcd(chox, choy);
                choy = 450;
                cd(chox, choy);
            }
            else if (kd_enter) {
                kd_enter = 0;
                if (choy == 450) {
                    cleardevice();
                    closegraph();
                    gameon = 0;
                    break;
                }
                else if (choy == 350) {
                    gamebegin = 1;
                    break;
                }
                else if (choy == 400) {
                    setcolor(BLACK);
                    settextstyle(20, 0, "楷体");
                    outtextxy(500, 0, "都说了没做好了，咋这么皮呢");
                }
            }
            else {
                kd_a = 0, kd_d = 0;
            }
    }
}

void gamebeginpre() {

    cleardevice();
    outtextxy(200, 600, "loading……");
    
    

    you.x = 80, you.y = 400;				// 当前坐标
    you.step = 40;
    you.grade = 1;
    you.score = 3000;
    you.blood = 3;              //残机
    you.power = 1;
    you.multi = 1;              //敌人的加成系数

    you.t1 = timeGetTime();
    you.dt = 200;	    // 子弹发射间隔
    you.et1 = timeGetTime();
    you.edt = 2000;     //出怪间隔 
    you.n = 0;
    goon = 1;
    MAXSORTENEMY = 3;   //当前敌机种类范围

    for (int i = 0; i < MAXENEMY; i++) {
        enemy1[i].blood = enemy1[i].cblood;
    }

}

void gamepause() {


    setcolor(BLUE);
    settextstyle(40, 0, "楷体");
    outtextxy(700, 300, "已暂停");

    setcolor(BLACK);
    settextstyle(25, 0, "楷体");
    outtextxy(700, 400, "继续");
    outtextxy(700, 450, "回到主界面");
    outtextxy(700, 500, "重新开始");

    settextstyle(20, 0, "楷体");
    //outtextxy(200, 700, "操作：wsad上下左右，esc退出，enter确认");

    int chox = 700, choy = 400;
    cd(chox, choy);
    while (1) {

        getmessage(&msg, EM_KEY);
        keydown();

            if (kd_w) {
                kd_w = 0;
                dcd(chox, choy);
                if (choy == 400) {
                    choy = 500;
                }
                else choy -= 50;
                cd(chox, choy);
            }
            else if (kd_s) {
                kd_s = 0;
                dcd(chox, choy);
                if (choy == 500) {
                    choy = 400;
                }
                else choy += 50;
                cd(chox, choy);
            }
            else if (kd_esc) {
                kd_esc = 0;
                return;
            }
            else if (kd_enter) {
                kd_enter = 0;
                if (choy == 500) {
                    goon = 0;
                    cleardevice();
                    break;
                }
                else if (choy == 400) {
                    return;
                }
                else if (choy == 450) {
                    gamebegin = 0;
                    return;
                }

            }
            else {
                kd_a = 0, kd_d = 0;
            }
        
    }
}

void Load() {

    //界面
    loadimage(&imagee.mainpicture, "./file/car.jpg", 300, 300);
    loadimage(&imagee.lose, "./file/lose.jpg", 400, 400);

    //自机
    loadimage(&you.img[1], "./file/car.jpg",80,80);
    loadimage(&you.img[0], "./file/carw.jpg",80,80);//暗

    //子弹
    
    IMAGE sm,smw;
    loadimage(&sm, "./file/shoot.jpg", 150, 30);
    loadimage(&smw, "./file/shoot2.jpg", 100, 30);
    SetWorkingImage(&smw);
    for (int i = 0; i < MAXJET; i++)
    {

        getimage(&jet[i].img[0], 0, 0, 30, 30);			// 暗
    }
    SetWorkingImage();
    SetWorkingImage(&sm);
    for (int i = 0; i < MAXJET; i++)
    {
        int n = rand() % 5;
        getimage(&jet[i].img[1], n * 30, 0, 30, 30);		// 亮
    }
    SetWorkingImage();		// 设置回绘图窗口

    //敌机
    Loadenemy();
}

void Loadenemy() {
    IMAGE sm, smw;
    loadimage(&smw, "./file/enemyw.jpg", 100, 100);
    for (int i = 0; i < MAXENEMY; i++) {
    enemy1[i].img[0] = smw;
    }
    
    loadimage(&enemy1[0].img[1], "./file/enemy1.jpg", 100, 100);
    loadimage(&enemy1[1].img[1], "./file/enemy2.jpg", 100, 100);
    loadimage(&enemy1[2].img[1], "./file/enemy3.jpg", 100, 100);
    loadimage(&enemy1[3].img[1], "./file/enemy4.jpg", 50, 50);
    loadimage(&enemy1[4].img[1], "./file/enemy5.jpg", 100, 100);

    enemy1[0].blood = 10;
    enemy1[0].dt = 600;
    enemy1[0].size = 100;
    

    enemy1[1].blood = 15;
    enemy1[1].dt = 600;
    enemy1[1].size = 100;

    enemy1[2].blood = 5;
    enemy1[2].dt = 300;
    enemy1[2].size = 100;

    enemy1[3].blood = 20;
    enemy1[3].dt = 200;
    enemy1[3].size = 50;

    enemy1[4].blood = 300;
    enemy1[4].dt = 2000;
    enemy1[4].size = 100;
    enemy1[4].boss = 1;

    for (int i = 0; i < MAXENEMY; i++) {
        enemy1[i].cblood = enemy1[i].blood;
    }
}

void Initj(int i) {

    jet[i].x = you.x + 80;				// 子弹弹左上角坐标
    jet[i].y = you.y + 40;
    jet[i].t1 = timeGetTime();
    jet[i].dt = 50;		        // 发射速度
    jet[i].n = 0;				// 烟花弹闪烁图片下标
    jet[i].shoot = false;		// 是否发射
    jet[i].size = 30;
}

void Inite(int i,int j) {
    enemy2[i].x = RIGHT - 150;
    enemy2[i].y = 246 + (rand() % 5) * 110;
    enemy2[i].t1 = timeGetTime();
    enemy2[i].emerg = false;
    enemy2[i].blood = enemy1[j].blood;
    enemy2[i].to = j;
    enemy2[i].boss = enemy1[j].boss;
    enemy2[i].dt = enemy1[j].dt;
    enemy2[i].size = enemy1[j].size;
    enemy2[i].lx = -1;
}

void shootj()
{
    bool isshoot = 0;//是否需要发射子弹
    you.t2 = timeGetTime();
    if (you.t2 - you.t1 > you.dt) {
        isshoot = 1;
        you.t1 = you.t2;
    }
    else;
    for (int temp = 0; temp < MAXJET;temp++) {
        jet[temp].t2 = timeGetTime();

        //测试：
        //putimage((temp + 1) * 50, (temp + 1) * 20, &jet[temp].img[1]);

        if (jet[temp].shoot) {
            if (jet[temp].x < RIGHT - jet[temp].size && jet[temp].t2 - jet[temp].t1 > jet[temp].dt) {
                putimage(jet[temp].x, jet[temp].y, &jet[temp].img[0]);
                jet[temp].x += you.step;
                putimage(jet[temp].x, jet[temp].y, &jet[temp].img[1]);
                jet[temp].t1 = jet[temp].t2;
            }
            else if (jet[temp].x >= RIGHT - jet[temp].size - jet[temp].size) {
                putimage(jet[temp].x, jet[temp].y, &jet[temp].img[0]);
                jet[temp].shoot = 0;
            }
            else;
        }
        else;

        if (isshoot && !jet[temp].shoot) {
            Initj(temp);
            putimage(jet[temp].x, jet[temp].y, &jet[temp].img[1]);
            jet[temp].shoot = 1;
            isshoot = 0;
        }

        else ;
        
        
    }
        
}

void shoote() {

    bool isshoot = 0;//是否需要发射敌机
    you.et2 = timeGetTime();
    if (you.et2 - you.et1 > you.edt) {
        isshoot = 1;
        you.et1 = you.et2;
    }
    for (int temp = 0; temp < MAXNUMENEMY; temp++) {
        enemy2[temp].t2 = timeGetTime();

        //测试：
        //putimage((temp + 1) * 200, 200, &enemy1[enemy2[temp].to].img[1]);

        if (enemy2[temp].emerg) {
            if (enemy2[temp].x > DDL && enemy2[temp].t2 - enemy2[temp].t1 > enemy2[temp].dt) {
                putimage(enemy2[temp].x, enemy2[temp].y, &enemy1[enemy2[temp].to].img[0]);
                
                enemy2[temp].x -= you.step;
                settextcolor(RED);
                
                putimage(enemy2[temp].x, enemy2[temp].y, &enemy1[enemy2[temp].to].img[1]);
                enemy2[temp].t1 = enemy2[temp].t2;
            }
            else if (enemy2[temp].x <= DDL) {
                putimage(enemy2[temp].x, enemy2[temp].y, &enemy1[enemy2[temp].to].img[0]);
                setcolor(WHITE);
                solidrectangle(enemy2[temp].x, enemy2[temp].y + enemy2[temp].size, enemy2[temp].x + enemy2[temp].size, enemy2[temp].y + enemy2[temp].size + 10);
                enemy2[temp].emerg = 0;
                you.blood--;
            }
            else;
        }
        else;

        if (isshoot && !enemy2[temp].emerg) {
            Inite(temp,rand()% MAXSORTENEMY);
            putimage(enemy2[temp].x, enemy2[temp].y, &enemy1[enemy2[temp].to].img[1]);
            enemy2[temp].emerg = 1;
            isshoot = 0;
        }

        else;


    }

}

void clear() {
    setfillcolor(WHITE);
    solidrectangle(0, 200, RIGHT, WIDTH - 100);
}

void fixedgamewindow() {
    setlinecolor(BLUE);
    line(0, 199, RIGHT, 199);
    line(0, 801, RIGHT, 801);
    line(RIGHT+1, 0, RIGHT+1, WIDTH);
    settextcolor(RED);
    settextstyle(25, 0, "宋体");
    outtextxy(RIGHT + 25, 300, "player：");
    settextcolor(BLUE);
    outtextxy(RIGHT + 25, 400, "grade：");
    settextcolor(RED);
    outtextxy(RIGHT + 25, 500, "power：");
    settextcolor(BLACK);
    settextstyle(30, 0, "宋体");
    outtextxy(RIGHT + 25, 75, "score：");
    settextstyle(20, 0, "楷体");
    outtextxy(200, 810, "操作：wsad上下左右，esc退出，enter确认");

}

void movinggamewindow() {
        
       //处理基本窗口
        setlinecolor(BLUE);
        line(RIGHT + 1, 0, RIGHT + 1, WIDTH);
        setlinecolor(RED);
        line(DDL, 200, DDL, WIDTH - 100);
        settextcolor(RED);
        settextstyle(20, 0, "宋体");
        outtextxy(DDL, 200, "This is DDL!");
        printt(RIGHT + 150, 300,255,0,0,you.blood-1,25,2 );
        printt(RIGHT + 150, 400, 0, 0, 255, you.grade, 25, 2);
        printt(RIGHT + 150, 75, 0, 0, 0, you.score, 30, 8);
        printt(RIGHT + 150, 500, 255, 0, 0, you.power + you.powerplus[you.grade], 25, 2);

       //处理敌机血量显示文字
       for (int temp = 0; temp < MAXNUMENEMY; temp++) {
           if (enemy2[temp].emerg) {
               if (~enemy2[temp].lx) {
                   setcolor(WHITE);
                   solidrectangle(enemy2[temp].lx, enemy2[temp].ly + enemy2[temp].size, enemy2[temp].lx + enemy2[temp].size - 10, enemy2[temp].ly + enemy2[temp].size + 10);
               }//如果不是刚发射，除掉上一次的血量显示
               else;
               //显示血量
               setcolor(RED);
               settextstyle(10, 0, "宋体");
               outtextxy(enemy2[temp].x, enemy2[temp].y + enemy2[temp].size, "HP：");
               printt(enemy2[temp].x + 25, enemy2[temp].y + enemy2[temp].size, 255, 0, 0, enemy2[temp].blood, 10, 0);
               //记录上一次的坐标
               enemy2[temp].lx = enemy2[temp].x;
               enemy2[temp].ly = enemy2[temp].y;
           }
           else;
       }

}

void printt(int x,int y,int a,int b,int d, int num,int size,int o) {
    settextcolor(RGB(a,b,d));
    settextstyle(size, 0, "宋体");
    if (o == 8) {
        sprintf_s(txt, "%08d", num);
    }

    else if (o == 2) {
        sprintf_s(txt, "%02d", num);
    }
    else {
        sprintf_s(txt, "%d", num);;
    }
    outtextxy(x,y,txt);

    return;
}//坐标，颜色，变量，字的大小，格式

void handle() {

    for (int i = 0; i < MAXJET; i++) {
        for (int j = 0; j < MAXNUMENEMY; j++) {
            if (jet[i].shoot && enemy2[j].emerg) {
                if (jet[i].x + jet[i].size >= enemy2[j].x) {
                    //printf("crush1!:");
                    //printf("%d %d\n",min(jet[i].y, enemy2[j].y), max(jet[i].y + jet[i].size, enemy2[j].y + enemy2[j].size));
                    //碰撞处理
                    if (max(jet[i].y, enemy2[j].y) < min(jet[i].y + jet[i].size, enemy2[j].y + enemy2[j].size)) {
                        //printf("crush2!\n");
                        enemy2[j].blood -= (you.power + you.powerplus[you.grade]);
                        putimage(jet[i].x, jet[i].y, &jet[i].img[0]);
                        jet[i].shoot = 0;
                    }
                    else;
                    //敌机血量处理
                    if (enemy2[j].blood <= 0) {
                        putimage(enemy2[j].x, enemy2[j].y, &enemy1[enemy2[j].to].img[0]);
                        setcolor(WHITE);
                        solidrectangle(enemy2[j].x, enemy2[j].y + enemy2[j].size, enemy2[j].x + enemy2[j].size - 10, enemy2[j].y + enemy2[j].size + 10);
                        if(enemy2[j].to == 3)printf("GOODBYE world!\n");
                        //分数处理
                        you.score += enemy1[enemy2[j].to].blood * you.multi * (you.grade / 5 + 1);
                        //boss处理
                        if (enemy2[j].boss) {
                            printf("oh shit!\n");
                            handleenemy(j, enemy2[j].to);
                        }
                        else;

                        enemy2[j].emerg = 0;//别忘了重置

                    }
                    else;
                }
                else;
            }
            else;
        }
    }
}

void handlegrade() {
    //if(you.score>99999999)
    if (you.grade <= 9 &&you.score > you.gsocre[you.grade + 1]) {
        you.grade++;
        setfillcolor(WHITE);
        solidrectangle(0, 0, 400, 100);
        setcolor(RED);
        settextstyle(30, 0, "楷体");
        outtextxy(0, 50, "*Next DDL！");
        outtextxy(0, 100, "*现在你充满了决心");


        you.edt -= 100;
        you.multi = you.grade / 3;
        for (int i = 0; i < MAXSORTENEMY; i++) {
            enemy1[i].blood = enemy1[i].cblood * (you.multi + 2);
        }

        if (MAXSORTENEMY < 5) {
            MAXSORTENEMY++;
            MAXNUMENEMY += 5;
        }
    }
    else;
}

void gameover() {
    if (you.blood <= 0) {

        putimage(200, 200, &imagee.lose);
        setcolor(RED);
        settextstyle(40, 0, "楷体");
        outtextxy(700, 200, "GAMEOVER！");
        outtextxy(650, 500, "最终得分：");
        printt(850, 500, 0, 0, 255, you.score, 50, 8);
        setcolor(BLACK);
        settextstyle(25, 0, "楷体");
        outtextxy(700, 350, "记录得分（没做好）");
        outtextxy(700, 400, "回到主界面");
        outtextxy(700, 450, "重新开始");

        settextstyle(20, 0, "楷体");
        outtextxy(200, 700, "操作：wsad上下左右，enter确认");

        int chox = 700, choy = 350;
        cd(chox, choy);
        while (1) {

            getmessage(&msg, EM_KEY);
            keydown();

                if (kd_w) {
                    kd_w = 0;
                    dcd(chox, choy);
                    if (choy == 350) {
                        choy = 450;
                    }
                    else choy -= 50;
                    cd(chox, choy);
                }
                else if (kd_s) {
                    kd_s = 0;
                    dcd(chox, choy);
                    if (choy == 450) {
                        choy = 350;
                    }
                    else choy += 50;
                    cd(chox, choy);
                }

                else if (kd_enter) {
                    kd_enter = 0;
                    if (choy == 450) {
                        goon = 0;
                        cleardevice();
                        break;
                    }
                    else if (choy == 350) {
                        setcolor(BLACK);
                        settextstyle(20, 0, "楷体");
                        outtextxy(500, 0, "都说了没做好了，咋这么皮呢");
                    }
                    else if (choy == 400) {
                        gamebegin = 0;
                        return;
                    }

                }
                else {
                    kd_a = 0, kd_d = 0, kd_esc = 0;
                }
            



        }
    }
    else if (you.score > 99999999) {
        you.score = 99999999;
        cleardevice();

        setcolor(RED);
        settextstyle(40, 0, "楷体");
        outtextxy(700, 200, "WIN！");
        outtextxy(650, 500, "最终得分：");
        printt(850, 500, 0, 0, 255, you.score, 50, 8);
        setcolor(BLACK);
        settextstyle(25, 0, "楷体");
        outtextxy(700, 350, "记录得分（没做好）");
        outtextxy(700, 400, "回到主界面");
        outtextxy(700, 450, "重新开始");

        settextstyle(20, 0, "楷体");
        outtextxy(200, 700, "操作：wsad上下左右，esc退出，enter确认");

        int chox = 700, choy = 350;
        cd(chox, choy);
        while (1) {

            getmessage(&msg, EM_KEY);
            keydown();

                if (kd_w) {
                    kd_w = 0;
                    dcd(chox, choy);
                    if (choy == 350) {
                        choy = 450;
                    }
                    else choy -= 50;
                    cd(chox, choy);
                }
                else if (kd_s) {
                    kd_s = 0;
                    dcd(chox, choy);
                    if (choy == 450) {
                        choy = 350;
                    }
                    else choy += 50;
                    cd(chox, choy);
                }

                else if (kd_enter) {
                    kd_enter = 0;
                    if (choy == 450) {
                        goon = 0;
                        cleardevice();
                        break;
                    }
                    else if (choy == 350) {
                        setcolor(BLACK);
                        settextstyle(20, 0, "楷体");
                        outtextxy(500, 0, "都说了没做好了，咋这么皮呢");
                    }
                    else if (choy == 400) {
                        gamebegin = 0;
                        return;
                    }

                }
                else {
                    kd_a = 0, kd_d = 0, kd_esc = 0;
                }
            




        }
    }
}

void keydown() {

    switch (msg.message)
    {
    case WM_KEYDOWN: {
        switch (msg.vkcode)
        {
        case 0x41:
            kd_a = 1;
            break;
        case 0x44:
            kd_d = 1;
            break;
        case 0x57:
            kd_w = 1;
            break;
        case 0x53:
            kd_s = 1;
            break;
        case VK_ESCAPE:
            kd_esc = 1;
            break;
        case VK_RETURN:
            kd_enter = 1;
            break;
        }
        break;
    }

    }

}

void handleenemy(int j,int to) {
    if (to == 4) {
        int num = rand() % 4 + 1;   //1~4个
        int e4x[5] = { 0,0,0,50, 50};
        int e4y[5] = { 0,0,50,0 ,50};
        for (int i = 0; i < MAXNUMENEMY; i++) {
            if (!enemy2[i].emerg) {
                Inite(i, 3);
                enemy2[i].x = enemy2[j].x + e4x[num];
                enemy2[i].y = enemy2[j].y + e4y[num];
                putimage(enemy2[i].x,enemy2[i].y, &enemy1[3].img[1]);
                //printf("%d %d\n", enemy2[j].x + e4x[num], enemy2[j].y + e4y[num]);
                enemy2[i].emerg = 1;
                printf("Hello world!\n");
                num--;
            }
            else;
            if (num == 0) {
                break;
            }
            else;
        }
    }
    else;
}