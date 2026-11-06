#include <stdio.h>
// 用联合体判断大小端



union check {
    int  i;
    char c;
};

int main(void)
{
    union check u;
    u.i = 1;

    if (u.c == 1)
        printf("小端\n");
    else
        printf("大端\n");

    return 0;
}
