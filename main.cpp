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
    vector<int> changes(prices.size(), 0), buy_sell(prices.size(), 0);
    for (int i = 1; i < prices.size(); i++)
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
    for (int i = n + 1; i < prices.size(); i++)
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

    // Square off kab karna hai ? after endate or on endate currently on enddate
    for (int i = 0; i < dates.size()- 1; i++)
    {
        file_1 << dates[i] << "," << prices[i] * buy_sell[i] << "\n";
        if (buy_sell[i] != 0)
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << 1 << "," << prices[i] << "\n";
    }

    //Squaring off 
    file_1 << dates[dates.size()-1] << "," << prices[dates.size()-1] * (buy_sell[dates.size()-1]+portfolio) << "\n";
    if ((buy_sell[dates.size()-1] + portfolio) != 0)
        file_2 << dates[dates.size()-1] << "," << ((buy_sell[dates.size()-1]+ portfolio) < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[dates.size()-1] + portfolio) << "," << prices[dates.size()-1] << "\n";

    final_amt += portfolio*prices[dates.size()-1];
    pnl << final_amt << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

int main(int argc, char *argv[])
{
    // Less arguments
    //  if (argc < 2) {
    //      std::cerr << "Usage: " << argv[0] << " <argument>\n";
    //      return 1;
    //  }
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
        dates.push_back(convertDate(date));
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