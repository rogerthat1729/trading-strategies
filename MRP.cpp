#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <ifstream>
using namespace std;
#define db double

void make_csv(vector<string> &dates, vector<db> &spread, vector<int> &buy_sell1, int portfolio, vector<db> &final_amt, int n, vector<db> &prices, vector<db> &prices2)
{
    int sz = spread.size();
    ofstream file_1("daily_cashflow.csv"), file_2("order_statistics_1.csv"), file_3("order_statistics_2.csv"), pnl("final_pnl.txt");

    file_1 << "Date,Cashflow\n";
    file_2 << "Date,Order_dir,Quantity,Price\n";
    file_3 << "Date,Order_dir,Quantity,Price\n";

    for (int i = n - 1; i < sz; i++)
    {
        file_1 << dates[i] << "," << to_string(final_amt[i]) << "\n";
        if (buy_sell1[i] != 0)
        {
            file_2 << dates[i] << "," << (buy_sell1[i] < 0 ? "BUY" : "SELL") << "," << abs(buy_sell1[i]) << "," << prices[i] << "\n";
            file_3 << dates[i] << "," << (buy_sell1[i] > 0 ? "BUY" : "SELL") << "," << abs(buy_sell1[i]) << "," << prices2[i] << "\n";
        }
    }
    // file_1 << dates.back() << "," <<  portfolio * spread[sz - 1] <<"Cleared the portfolio" << "\n";
    pnl << final_amt.back() + portfolio * spread[sz - 1] << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

void MRP(vector<db> spread, vector<string> dates, int n, int x, db threshold, vector<db> &pri, vector<db> &pri2)
{
    int sz = spread.size();
    db mean = 0.0;
    db sqmean = 0.0;
    db stdev = 0.0;
    db zscore = 0.0;
    int portfolio = 0;
    vector<int> buy_sell1(sz, 0);
    vector<int> buy_sell2(sz, 0);
    vector<db> final_amt(sz, 0);

    for (int i = 0; i < n - 1; i++)
    {
        mean += spread[i];
        sqmean += spread[i] * spread[i];
    }
    mean = mean / n;
    sqmean = sqmean / n;

    for (int i = n - 1; i < sz; i++)
    {

        mean += spread[i] / n;
        sqmean += spread[i] * spread[i] / n;
        stdev = sqrt(sqmean - mean * mean);
        zscore = (spread[i] - mean) / stdev;

        if (zscore > threshold && portfolio > -x)
        { // SELL S1 and BUY S2
            portfolio -= 1;
            buy_sell1[i] = 1;
            buy_sell2[i] = -1;
        }
        else if (zscore < -threshold && portfolio < x)
        { // BUY S1 and SELL S2

            portfolio += 1;
            buy_sell1[i] = -1;
            buy_sell2[i] = 1;
        }
        if (i > 0)
        {
            final_amt[i] = final_amt[i - 1];
        }

        final_amt[i] += buy_sell1[i] * spread[i];

        mean -= (spread[i - n + 1]) / n;
        sqmean -= (spread[i - n + 1]) * (spread[i - n + 1]) / n;
    }

    make_csv(dates, spread, buy_sell1, portfolio, final_amt, n, pri, pri2);
}

void MRP_stop(vector<db> spread, vector<string> dates, int n, int x, db threshold, vector<db> &pri, vector<db> &pri2, db stop_loss_threshold)
{
    int sz = spread.size();

    db mean = 0.0;
    db sqmean = 0.0;
    db stdev = 0.0;
    db zscore = 0.0;
    int portfolio = 0;
    vector<int> buy_sell1(sz, 0);
    vector<int> buy_sell2(sz, 0);
    vector<db> final_amt(sz, 0);

    // stop_loss needs
    vector<pair<int, pair<db, db>>> sell_tracker; // (index, (mean, stdev)) this will store all days that have currently cossed +threshold check for +stop_loss_threshold
    vector<pair<int, pair<db, db>>> buy_tracker;  // (index, (mean, stdev)) this will store all days that have currently cossed -threshold check for -stop_loss_threshold

    for (int i = 0; i < n - 1; i++)
    {
        mean += spread[i];
        sqmean += spread[i] * spread[i];
    }
    mean = mean / n;
    sqmean = sqmean / n;

    for (int i = n - 1; i < sz; i++)
    {

        mean += spread[i] / n;
        sqmean += spread[i] * spread[i] / n;
        stdev = sqrt(sqmean - mean * mean);
        zscore = (spread[i] - mean) / stdev;

        // getting all the numbers
        int sell_tracker_sz = sell_tracker.size();
        int buy_tracker_sz = buy_tracker.size();

        if (zscore > threshold && portfolio > -x)
        { // SELL S1 and BUY S2

            if (buy_tracker_sz == 0)
            {
                // overall portfolio is +ve that is only sell signals generated generate another
                sell_tracker.push_back({i, {mean, stdev}});
                buy_sell1[i] += 1;
                sell_tracker_sz += 1;
                portfolio -= 1;
            }
            else
            {
                buy_tracker.erase(buy_tracker.begin());
                buy_tracker_sz -= 1;
                buy_sell1[i] += 1;
                portfolio -= 1;
            }
        }
        else if (zscore < -threshold && portfolio < x)
        { // BUY S1 and SELL S2

            if (sell_tracker_sz == 0)
            {
                // overall portfolio is -ve that is only buy signals generated generate another
                buy_tracker.push_back({i, {mean, stdev}});
                buy_sell1[i] -= 1;
                buy_tracker_sz += 1;
                portfolio += 1;
            }
            else
            {
                // no sell crossed so we will buy the oldest one
                sell_tracker.erase(sell_tracker.begin());
                sell_tracker_sz -= 1;
                buy_sell1[i] -= 1;
                portfolio += 1;
            }
        }
        int sell_crossed = 0;
        int buy_crossed = 0;

        for (int i = 0; i < sell_tracker_sz; i++)
        {
            if (spread[i] > sell_tracker[i].second.first + stop_loss_threshold * sell_tracker[i].second.second)
            {
                sell_crossed += 1;
            }
        }
        for (int i = 0; i < buy_tracker_sz; i++)
        {
            if (spread[i] < buy_tracker[i].second.first - stop_loss_threshold * buy_tracker[i].second.second)
            {
                buy_crossed += 1;
            }
        }

        // updating the buy and sell trackers finally if no signal was raised
        if (buy_crossed > 0)
        {
            // here we clear out all the crossed ones and include the sell in it only.
            buy_sell1[i] += buy_crossed; // overlapping today's sell with the crossed ones
            portfolio -= buy_crossed;
            // updated the buy_tracker
            vector<pair<int, pair<db, db>>> temp;
            for (int i = 0; i < buy_tracker_sz; i++)
            {
                if (spread[i] < buy_tracker[i].second.first - stop_loss_threshold * buy_tracker[i].second.second)
                {
                    // 
                }
                else
                {
                    temp.push_back(buy_tracker[i]);
                }
            }
            buy_tracker = temp;
            buy_crossed = 0;
            buy_tracker_sz = buy_tracker.size();
        }
        if (sell_crossed > 0)
        {
            // here we clear out all the crossed ones and include the buy in it only.
            buy_sell1[i] -= sell_crossed; // overlapping today's buy with the crossed ones
            portfolio += sell_crossed;
            // updated the sell_tracker
            vector<pair<int, pair<db, db>>> temp;
            for (int i = 0; i < sell_tracker_sz; i++)
            {
                if (spread[i] > sell_tracker[i].second.first + stop_loss_threshold * sell_tracker[i].second.second)
                {
                }
                else
                {
                    temp.push_back(sell_tracker[i]);
                }
            }
            sell_tracker = temp;
            sell_crossed = 0;
            sell_tracker_sz = sell_tracker.size();
        }

        if (i > 0)
        {
            final_amt[i] = final_amt[i - 1];
        }

        final_amt[i] += buy_sell1[i] * spread[i];

        mean -= (spread[i - n + 1]) / n;
        sqmean -= (spread[i - n + 1]) * (spread[i - n + 1]) / n;
    }

    make_csv(dates, spread, buy_sell1, portfolio, final_amt, n, pri, pri2);
}

int main(int argc, char *argv[])
{

    string strategy = argv[1];
    cout << strategy << endl;
    string sym1 = argv[2];
    string sym2 = argv[3];
    vector<string> dates;
    vector<db> prices, prices2;

    ifstream file(sym1 + ".csv");
    string line;
    // Skip the header line
    getline(file, line);

    while (getline(file, line))
    {
        istringstream ss(line);
        string field;
        string date;

        // skipping index column
        getline(ss, field, ',');

        // Get the date
        getline(ss, date, ',');
        dates.push_back(date);

        // Get the price
        if (getline(ss, field, ','))
        {
            prices.push_back(stod(field));
        }
    }
    file.close();

    ifstream file2(sym2 + ".csv");
    string line2;
    // Skip the header line
    getline(file2, line2);

    while (getline(file2, line))
    {
        istringstream ss(line);
        string field;
        string date;

        // skipping index column
        getline(ss, field, ',');

        // Get the date
        getline(ss, date, ',');
        // already have the date

        // Get the price
        if (getline(ss, field, ','))
        {
            prices2.push_back(stod(field));
        }
    }
    file2.close();
    vector<db> spread;
    for (int i = 0; i < prices.size(); i++)
    {
        spread.push_back(prices[i] - prices2[i]);
    }

    int x = stoi(argv[4]);
    int n = stoi(argv[5]);
    db threshold = stod(argv[6]);

    db stop_loss_threshold = stod(argv[7]);

    if (stop_loss_threshold == -5)
    {
        MRP(spread, dates, n, x, threshold, prices, prices2);
    }
    else
    {
        MRP_stop(spread, dates, n, x, threshold, prices, prices2, stop_loss_threshold);
    }

    return 0;
}