#include <bits/stdc++.h>
#include <omp.h>
#include "main.h"
using namespace std;
#define db double

db read_data()
{
    ifstream file("final_pnl.txt");
    db x;
    file >> x;
    file.close();
    return x;
}

int main()
{
    db PnL = INT_MAX;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            vector<string> args = {"main", "BASIC", "7", "5"};
            work(4, args);
            PnL = min(PnL, read_data());
        }
        #pragma omp section
        {
            vector<string> args = {"main", "DMA", "50", "5", "2"};
            work(5, args);
            PnL = min(PnL, read_data());
        }
    }
    return 0;
}