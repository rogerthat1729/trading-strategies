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
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[i]) << "," << prices[i] << "\n";
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
    for (int i = 0; i < n; ++i)
    {
        sum += prices[i];
        sum_sq += prices[i] * prices[i];
    }
    sum /= n;
    sum_sq /= n;
    DMA[n - 1] = sum;
    for (int i = n; i < sz; i++)
    {
        db sigma = sqrt(sum_sq - DMA[i - 1] * DMA[i - 1]);
        if (prices[i] > DMA[i - 1] + p * sigma && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (prices[i] < DMA[i - 1] - p * sigma && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }
        final_amt += buy_sell[i] * prices[i];
        DMA[i] = DMA[i - 1] + (prices[i] - prices[i - n]) / n;
        sum_sq += (prices[i] * prices[i] - prices[i - n] * prices[i - n]) / n;
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

db updateSF(db SF, db ER, db c1, db c2)
{
    return SF + c1 * ((((2 * ER) / (1 + c2) - 1) / ((2 * ER) / (1 + c2) + 1)) - SF);
}

void S1_3(vector<string> dates, vector<db> prices, int n, int x, db p, int mhd, db c1, db c2)
{
    int sz = dates.size(), portfolio = 0, max_hold_days = 0, running_days = 0, earliest_index = 1e9;
    vector<int> buy_sell(sz, 0);
    db running_denom = 0, SF = 0.5, AMA = prices[n], ER, final_amt = 0;
    queue<int> bought, sold;

    // update runningdenom
    for (int i = 0; i < n; i++)
        running_denom += abs(prices[i] - prices[i + 1]);
        
    for (int i = n; i < sz; i++)
    {
        //Check if no of trades in a day can be > 1 - basically follow AUTOGRADER CONDITIONS !!!!
        
        // Reason for implementing stop loss before AMA condition:
        // Edge case - You are at the boundary position +x, and the price is increasing. 
        // You won't be able to buy directly using AMA condition, but you can sell using stop loss condition and then buy using AMA condition
        // Also, since AMA condition does not take into account the buy and sell history, there is no specific need of order of implementation
        
        // check for max holding days
        if(earliest_index != 1e9 && i - earliest_index >= mhd)
        {
            //Verify condition when both stop loss and AMA are applicable - to do nothing or to buy then sell etc.
            
            //We don't need to worry about the max position check here as it always reduces the magnitude of position
            //Here, abs(buy_sell[i]) cannot exceed 1
            //Moreover, since both bought and sold are not non-empty, we only need to check size of either one of them
            
            if(bought.size()>0)
            {
                bought.pop();
                if (bought.size() != 0)
                    earliest_index = bought.front();
                else
                    earliest_index = 1e9;
                portfolio--;
                buy_sell[i] += 1;
            }
            else if(sold.size()>0)
            {
                sold.pop();
                if (sold.size() != 0)
                    earliest_index = sold.front();
                else
                    earliest_index = 1e9;
                portfolio++;
                buy_sell[i] -= 1;
            }
        }
        
        if (prices[i] >= (1 + (p / 100.0)) * AMA && portfolio < x)
        {
            if (sold.size() > 0)
            {
                int j = sold.front();
                sold.pop();
                // We know bought is empty
                if (sold.size() != 0)
                    earliest_index = sold.front();
                else
                    earliest_index = 1e9;
            }
            else
            {
                bought.push(i);
                earliest_index = min(i, earliest_index);
            }
            buy_sell[i] -= 1;
            portfolio++;
        }
        else if (prices[i] <= (1 - (p / 100.0)) * AMA && portfolio > -x)
        {
            if (bought.size() > 0)
            {
                int j = bought.front();
                bought.pop();
                // We know sold is empty
                if (bought.size() != 0)
                    earliest_index = bought.front();
                else
                    earliest_index = 1e9;
            }
            else
            {
                sold.push(i);
                earliest_index = min(i, earliest_index);
            }
            buy_sell[i] += 1;
            portfolio--;
        }
        
        final_amt += buy_sell[i] * prices[i];

        // update runningdenom
        if (i < sz - 1)
        {
            // Verify the meaning of skip when runningdenom = 0
            if (running_denom != 0)
            {
                ER = (prices[i] - prices[i - n]) / running_denom;
                SF = updateSF(SF, ER, c1, c2);
                AMA += SF * (prices[i] - AMA);
            }
            running_denom -= abs(prices[i - n] - prices[i - n + 1]);
            running_denom += abs(prices[i] - prices[i + 1]);
        }
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

void S1_4_1(vector<string> dates, vector<db> prices, int x)
{
    
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
    else if (strategy == "DMA")
        S1_2(dates, prices, n, x, p);
    else if (strategy == "DMA++")
        S1_3(dates, prices, n, x, p, mhd, c1, c2);
    else if(strategy == "MACD")
        S1_4_1(dates, prices, x);
    return 0;
}