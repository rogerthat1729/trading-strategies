#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
using namespace std;
#define db double

void make_csv_main(vector<string> &dates, vector<db> &prices, vector<int> &buy_sell, int portfolio, vector<db> &final_amt, int n);

void S1_1(vector<string> &dates, vector<db> &prices, int n, int x);

void S1_2(vector<string> dates, vector<db> prices, int n, int x, db p);

db updateSF(db SF, db ER, db c1, db c2);

void S1_3(vector<string> dates, vector<db> prices, int n, int x, db p, int mhd, db c1, db c2);

void S1_4_1(vector<string> dates, vector<db> prices, int n, int x);

void S1_4_2(vector<string> dates, vector<db> prices, int n, int x, db overbought, db oversold);

void S1_4_3(vector<string> dates, vector<db> highPrices, vector<db> lowPrices, vector<db> prevClosePrices, vector<db> prices, int n, int x, db adx_threshold);

db work(int argc, vector<string> args);

#endif