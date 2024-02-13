#include <bits/stdc++.h>
#include <omp.h>
#include "main.h"
#include "LR.h"
using namespace std;
#define db long double

int main()
{
    db PnL = INT_MIN;
    string best_strategy;

    map<string, vector<string>> run;
    run["BASIC"] = {"main", "BASIC", "7", "5"};
    run["DMA"] = {"main", "DMA", "50", "5", "2"};
    run["DMA++"] = {"main", "DMA++", "14", "5", "5", "28", "2", "0.2"};
    run["MACD"] = {"main", "MACD", "0", "5"};
    run["RSI"] = {"main", "RSI", "14", "5", "70", "30"};
    run["ADX"] = {"main", "ADX", "14", "5", "25"};
    run["LR"] = {"LR", "5", "2"};

    map<int, string> index;
    index[0] = "BASIC";
    index[1] = "DMA";
    index[2] = "DMA++";
    index[3] = "MACD";
    index[4] = "RSI";
    index[5] = "ADX";
    index[6] = "LR";

#pragma omp parallel for num_threads(7)

    for (int i = 0; i < 7; i++)
    {
        vector<string> args = run[index[i]];
        db pnl;
        if (index[i] == "LR")
            pnl = workLR(args);
        else
        {
            args.insert(args.begin(), "1");
            pnl = work(args);
        }
#pragma omp critical
        {
            // std::cout << index[i] << ": " << pnl << endl;
            if (pnl > PnL)
            {
                PnL = pnl;
                best_strategy = index[i];
            }
        }
    }
    // cout << "Best Strategy: " << best_strategy << endl;
    // cout << best_strategy << endl;
    vector<string> args = run[best_strategy];
    if (best_strategy == "LR")
        workLR(args);
    else
    {
        args.insert(args.begin(), "1");
        work(args);
    }

    // vector<string> args = {"main", "BASIC", "7", "5"};
    // db pnl = work(4, args);
    // cout << "BASIC: " << pnl << endl;
    // if (pnl > PnL)
    // {
    //     PnL = pnl;
    //     best_strategy = "BASIC";
    // }

    // args = {"main", "DMA", "50", "5", "2"};
    // pnl = work(5, args);
    // cout << "DMA: " << pnl << endl;
    // if (pnl > PnL)
    // {
    //     PnL = pnl;
    //     best_strategy = "DMA";
    // }

    // args = {"main", "DMA++", "14", "5", "5", "28", "2", "0.2"};
    // pnl = work(8, args);
    // cout << "DMA++: " << pnl << endl;
    // if (pnl > PnL)
    // {
    //     PnL = pnl;
    //     best_strategy = "DMA";
    // }

    // args = {"main", "MACD", "0", "5"};
    // pnl = work(4, args);
    // cout << "MACD: " << pnl << endl;
    // if (pnl > PnL)
    // {
    //     PnL = pnl;
    //     best_strategy = "DMA";
    // }

    // args = {"main", "RSI", "14", "5", "70", "30"};
    // pnl = work(6, args);
    // cout << "RSI: " << pnl << endl;
    // if (pnl > PnL)
    // {
    //     PnL = pnl;
    //     best_strategy = "DMA";
    // }

    // args = {"main", "ADX", "14", "5", "25"};
    // pnl = work(5, args);
    // cout << "ADX: " << pnl << endl;
    // if (pnl > PnL)
    // {
    //     PnL = pnl;
    //     best_strategy = "DMA";
    // }

    return 0;
}