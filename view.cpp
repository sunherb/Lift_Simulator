#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;
#include <iomanip>
#include"lift.h"

extern int MaxClientNum;
extern int  MaxRunTime;
void ShowTitle()
{
    printf("\n\n\n");
    printf("\n\t\t\t *********************************************************");
    printf("\n\t\t\t _________________________________________________________");
    printf("\n\t\t\t ===========Welcome to the elevator management system ============\n");
    printf("\t\t\t ---------------------------------------------------------");
    printf("\n\t\t\t _________________________________________________________");
    printf("\n\t\t\t ================ Lift_Simulator ===================\n");
    printf("\t\t\t ---------------------------------------------------------");
    printf("\n");
    printf("\n\t\t\t *********************************************************\n");

} // here is the title of the view, welcome page.

void get_pos_xy(short x, short y)// Used to determine the output location of the information
{
    COORD coord = { x, y };
    /*
     * COORD is a structure defined in the Windows API that represents the coordinates of a character on the console screen. It is defined as:

    typedef struct _COORD {

    SHORT X; // horizontal coordinate

    SHORT Y; // vertical coordinate
    } COORD;
*/

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void PrintElevatorFrame() //Output the overall outline of the lift
{
    int j, i, k;
    for (i = 0; i <= 1; i++)
    {
        for (j = MaxFloor+1; j >= 0; j--)
        {
            if (j == MaxFloor)
            {
                get_pos_xy(6+i*27,9);
                printf("--------------------------");
            }
            get_pos_xy(6+i*27,9+j* FloorHight);
            printf("--------------------------");

        }
        j= MaxFloor + 1;
        for (k = 9; k <= 9 + j * FloorHight; k++)
        {
            get_pos_xy(5+i*27,k);
            printf("|");
            get_pos_xy(5+i*27+26,k);
            printf("|");
        }
    }

}
//隐藏在x层的电梯
void DisappearEvelatorBody(Elevator e,int k,int x)
{
    int i, j;
    //先设置字体为背景颜色(黑色)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
    for (i = LeftElevatorX + k * 27; i <= LeftElevatorX + k * 26 + 24; i++)
    {
        get_pos_xy(i, LeftElevatorY - 1 + (4 - x) * FloorHight);
        printf(" ");
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//在x层显示电梯
void ShowElevatorBody(Elevator e, int k,int x)
{
    int i, j;
    Client *p;
    get_pos_xy(LeftElevatorX+k*27,LeftElevatorY-1+(4-x)*FloorHight);
    printf("[");
    for (i = 0; i < e->ClientNum; i++)
    {
        if(e->ClientID[i]!=0)
            printf(" %d", e->ClientID[i]);
    }
    get_pos_xy(LeftElevatorX+k*26+24,LeftElevatorY - 1 + (4 - x)*FloorHight);
    printf("]");
}
//显示等待队列的乘客,flag 为0 表示该队列的乘客要往下，flag为1表示往上
//num表示同层中另一个队列的人数，显示必须在其之后，不然会覆盖上一个队列的数据
void ShowWaitingClient(WaitQueue Q, int flag,int num)
{
    ClientQueueNode * p;
    p = Q.front->next;
    int j=num;
    while (p != NULL)
    {
        get_pos_xy(RightElevatorX + 26+j*6, RightElevatorY-1 + (4-p->data->InFloor)*FloorHight);
        printf(" %d", p->data->ClientID);
        if (flag)// falg == 1 up else down
            printf("will--up");
        else
            printf("will--down");
        printf("%d",p->data->OutFloor);
        p = p->next;
        j++;
    }
}
//消隐处于等待状态的乘客
//当有新乘客加入或者有乘客入电梯，乘客离开时，需要消隐
void DisappearWaitingClient(WaitQueue Q, int flag,int num)
{
    //设置字体的颜色，用背景颜色再写一遍就能消去字体 0是黑色 7是白色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
    ShowWaitingClient(Q, flag,num);
    //消隐完后再重新恢复字体颜色
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//显示电梯的动作
void PrintfElevatorMove(Elevator e,int k)
{
    //Opening, Opened, Closing, Closed, Moving, SlowDown, Waiting, Accelerate
    get_pos_xy(10 + 30 * k, 4);
    printf("                         ");
    get_pos_xy(10+30*k,4);
    switch (e->move)
    {
    case Opening:printf(" %dlift door is opening", k); break;
    case Opened:printf(" %dlift The door is opened", k); break;
    case Closing:printf(" %dlift door closing", k); break;
    case Closed:printf(" %dlift door closed", k); break;
    case Moving:printf(" %dlift moving", k); break;
    case SlowDown:printf(" %dlift slowdown-ing", k); break;
    case Waiting:printf(" %dlift waiting", k); break;
    case Accelerate:printf(" %dlift accelerating", k); break;
    }
}
//tips as follows
void PrintfHelp()
{
    get_pos_xy(60, 7);
    printf("tips:The preceding number represents the passenger ID");
    get_pos_xy(60, 9);
    printf("The numbers at the back represent the passenger's target floor");
}
