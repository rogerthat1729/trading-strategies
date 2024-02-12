#ifndef MAIN_H
#define MAIN_H

#include <bits/stdc++.h>
using namespace std;
#define db long double

void make_csv_main(vector<string> &dates, vector<db> &prices, vector<int> &buy_sell, int portfolio, vector<db> &final_amt, int n);

void S1_1(vector<string> &dates, vector<db> &prices, int n, int x, vector<db> &final_amt, vector<int> &buy_sell, int &portfolio);

void S1_2(vector<string> dates, vector<db> prices, int n, int x, db p, vector<db> &final_amt, vector<int> &buy_sell, int &portfolio);

db updateSF(db SF, db ER, db c1, db c2);

void S1_3(vector<string> dates, vector<db> prices, int n, int x, db p, int mhd, db c1, db c2, vector<db> &final_amt, vector<int> &buy_sell, int &portfolio);

void S1_4_1(vector<string> dates, vector<db> prices, int x, vector<db> &final_amt, vector<int> &buy_sell, int &portfolio);

void S1_4_2(vector<string> dates, vector<db> prices, int n, int x, db overbought, db oversold, vector<db> &final_amt, vector<int> &buy_sell, int &portfolio);

void S1_4_3(vector<string> dates, vector<db> highPrices, vector<db> lowPrices, vector<db> prices, int n, int x, db adx_threshold, vector<db> &final_amt, vector<int> &buy_sell, int &portfolio);

db work(vector<string> args);

#endif