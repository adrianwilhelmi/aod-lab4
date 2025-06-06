#pragma once
#include <stdio.h>


int openMoves(int nr,int b[5][5]){
    switch (nr)
    {
    case 1:
        return 33;
        break;
    case 2:
        if(b[0][0]!=0)
            return 44;
        if(b[0][4]!=0)
            return 42;
        if(b[4][0]!=0)
            return 24;
        if(b[4][4]!=0)
            return 22;
        if(b[2][2]!=0)
            return 22;
        if(b[1][1]!=0 || b[1][3]!=0)
            return 23;
        if(b[3][1]!=0 || b[3][3]!=0)
            return 43;
        if(b[0][1]!=0)
            return 42;
        if(b[1][0]!=0)
            return 24;
        if(b[3][4]!=0)
            return 42;
        if(b[4][3]!=0)
            return 24;
        if(b[0][3]!=0)
            return 44;
        if(b[4][1]!=0)
            return 22;
        if(b[3][0]!=0)
            return 24;
        if(b[1][4]!=0)
            return 22;
        if(b[0][2]!=0 || b[2][0]!=0 || b[2][4]!=0 || b[4][2]!=0)
            return 33;
        return 33;
    case 3:

        break;
    case 4:
        break;
    case 5:

        break;
    default:
        return -1;
        break;
    }
}