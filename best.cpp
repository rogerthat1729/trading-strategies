#include <bits/stdc++.h>
#include <omp.h>
#include "main.h"
using namespace std;
#define db double

int main()
{
    db PnL = INT_MAX;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            vector<string> args = {"main", "BASIC", "7", "5"};
            db pnl = work(4, args);
            // cout << pnl << endl;
        }
        #pragma omp section
        {
            vector<string> args = {"main", "DMA", "50", "5", "2"};
            db pnl = work(5, args);
            // cout << pnl << endl;
        }
    }
    return 0;
}