#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <ifstream>
using namespace std;

string convertDate(string date)
{
    string year = date.substr(0, 4);
    string month = date.substr(5, 2);
    string day = date.substr(8, 2);
    return day + "/" + month + "/" + year;
}

void S1_1(vector<string> dates, vector<double> prices, int n, int x)
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
    double final_amt = 0;

    // Clarify what last n days really means
    for (int i = n + 1; i < sz; i++)
    {
        if (changes[i - 1] - changes[i - 1 - n] == n && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (changes[i - 1] - changes[i - 1 - n] == -n && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = +1;
        }
        final_amt += buy_sell[i] * prices[i];
    }

    ofstream file_1("daily_cashflow.csv"), file_2("order_statistics.csv"), pnl("final_pnl.txt");

    file_1 << "date,cashflow\n";
    file_2 << "date,order_direction,quantity,price\n";

    // Square off kab karna hai ? after enddate or on enddate currently on enddate
    for (int i = 0; i < sz- 1; i++)
    {
        file_1 << dates[i] << "," << prices[i] * buy_sell[i] << "\n";
        if (buy_sell[i] != 0)
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << 1 << "," << prices[i] << "\n";
    }

    //Squaring off 
    file_1 << dates[sz-1] << "," << prices[sz-1] * (buy_sell[sz-1]+portfolio) << "\n";
    if ((buy_sell[sz-1] + portfolio) != 0)
        file_2 << dates[sz-1] << "," << ((buy_sell[sz-1]+ portfolio) < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[sz-1] + portfolio) << "," << prices[sz-1] << "\n";

    final_amt += portfolio*prices[sz-1];
    pnl << final_amt << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

int main(int argc, char *argv[])
{
    string strategy = argv[1];
    int n = stoi(argv[2]);
    int x = stoi(argv[3]);
    ifstream file("data.csv");
    string line;

    vector<string> dates;
    vector<double> prices;

    std::getline(file, line);

    while (getline(file, line))
    {
        istringstream ss(line);
        string field;
        string date;
        double price;
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
    {
        S1_1(dates, prices, n, x);
    }
    return 0;
}