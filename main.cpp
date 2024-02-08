#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <ifstream>
using namespace std;
#define db double

void make_csv(vector<string> &dates, vector<db> &prices, vector<int> &buy_sell, int portfolio, db final_amt, int n)
{
    int sz = prices.size();
    ofstream file_1("daily_cashflow.csv"), file_2("order_statistics.csv"), pnl("final_pnl.txt");

    file_1 << "Date,Cashflow\n";
    file_2 << "Date,Order_dir,Quantity,Price\n";

    // Square off kab karna hai ? after enddate or on enddate currently on enddate
    for (int i = n; i < sz - 1; i++)
    {
        file_1 << dates[i] << "," << prices[i] * buy_sell[i] << "\n";
        if (buy_sell[i] != 0)
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << 1 << "," << prices[i] << "\n";
    }

    // Squaring off
    file_1 << dates[sz - 1] << "," << prices[sz - 1] * (buy_sell[sz - 1] + portfolio) << "\n";
    if ((buy_sell[sz - 1] + portfolio) != 0)
        file_2 << dates[sz - 1] << "," << ((buy_sell[sz - 1] + portfolio) < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[sz - 1] + portfolio) << "," << prices[sz - 1] << "\n";

    final_amt += portfolio * prices[sz - 1];
    pnl << final_amt << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

void S1_1(vector<string> &dates, vector<db> &prices, int n, int x)
{
    int sz = prices.size();
    vector<int> changes(sz, 0), buy_sell(sz, 0);
    for (int i = 1; i < sz; i++)
    {
        if (prices[i] > prices[i - 1])
            changes[i] = changes[i - 1] + 1;
        else if (prices[i] < prices[i - 1])
            changes[i] = changes[i - 1] - 1;
        else
            changes[i] = changes[i - 1];
    }

    int portfolio = 0;
    db final_amt = 0;

    // Clarify what last n days really means
    for (int i = n; i < sz; i++)
    {
        if (changes[i] - changes[i - n] == n && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (changes[i] - changes[i - n] == -n && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }
        final_amt += buy_sell[i] * prices[i];
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

void S1_2(vector<string> dates, vector<db> prices, int n, int x, db p)
{
    int sz = dates.size(), portfolio = 0;
    db final_amt = 0;
    vector<int> buy_sell(sz, 0);
    vector<db> DMA(sz, 0);
    db sum = 0, sum_sq = 0;
    for(int i = 0 ; i < n ; ++i)
    {
        sum += prices[i];
        sum_sq += prices[i] * prices[i];
    }
    sum /= n;
    sum_sq /= n;
    DMA[n-1] = sum;
    for (int i = n; i < sz; i++)
    {
        DMA[i] = DMA[i - 1] + (prices[i] - prices[i - n]) / n;
        sum_sq += (prices[i] * prices[i] - prices[i - n] * prices[i - n])/n;
        db sigma = sqrt(sum_sq - DMA[i] * DMA[i]);
        if (prices[i] > DMA[i] + p * sigma && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (prices[i] < DMA[i] - p * sigma && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }
        final_amt += buy_sell[i] * prices[i];
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

void S1_3(vector<string> dates, vector<db> prices, int n, int x, db p, int mhd, db c1, db c2)
{
    int sz = dates.size(), portfolio = 0, max_hold_days = 0;
    db final_amt = 0;
    vector<int> buy_sell(sz, 0);
    vector<db> DMA(sz, 0);
    db sum = 0, sum_sq = 0;
    for(int i = 0 ; i < n ; ++i)
    {
        sum += prices[i];
        sum_sq += prices[i] * prices[i];
    }
    sum /= n;
    sum_sq /= n;
    DMA[n-1] = sum;
    for (int i = n; i < sz; i++)
    {
        DMA[i] = DMA[i - 1] + (prices[i] - prices[i - n]) / n;
        sum_sq += (prices[i] * prices[i] - prices[i - n] * prices[i - n])/n;
        db sigma = sqrt(sum_sq - DMA[i] * DMA[i]);
        if (prices[i] > DMA[i] + p * sigma && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (prices[i] < DMA[i] - p * sigma && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }
        if(buy_sell[i] != 0)
            max_hold_days = 0;
        else
            max_hold_days++;
        if(max_hold_days >= mhd)
        {
            // Change the order_statistics file to reflect value of |buy_sell| > 1
            buy_sell[i] = portfolio;
            max_hold_days = 0;
            portfolio = 0;
        }
        final_amt += buy_sell[i] * prices[i];
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

int main(int argc, char *argv[])
{
    string strategy = argv[1];
    int n = stoi(argv[2]);
    int x = stoi(argv[3]);
    db p = stod(argv[4]);
    int mhd = stoi(argv[5]);
    db c1 = stod(argv[6]);
    db c2 = stod(argv[7]);
    
    ifstream file("data.csv");
    string line;

    vector<string> dates;
    vector<db> prices;

    std::getline(file, line);

    while (getline(file, line))
    {
        istringstream ss(line);
        string field;
        string date;
        db price;
        // remove index
        getline(ss, field, ',');
        // Get the date
        getline(ss, date, ',');
        dates.push_back(date);
        // Get the price
        if (getline(ss, field, ','))
        {
            price = stod(field);
            prices.push_back(price);
        }
    }
    file.close();
    if (strategy == "BASIC")
        S1_1(dates, prices, n, x);
    else if(strategy == "DMA")
        S1_2(dates, prices, n, x, p);
    else if(strategy == "DMA++")
        S1_3(dates, prices, n, x, p, mhd, c1, c2);
    return 0;
}