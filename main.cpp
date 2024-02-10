#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <ifstream>
using namespace std;
#define db double

// Remove cashflow floating point error
// Match outputs in all cases

void make_csv(vector<string> &dates, vector<db> &prices, vector<int> &buy_sell, int portfolio, vector<db> &final_amt, int n)
{
    int sz = prices.size();
    ofstream file_1("daily_cashflow.csv"), file_2("order_statistics.csv"), pnl("final_pnl.txt");

    file_1 << "Date,Cashflow\n";
    file_2 << "Date,Order_dir,Quantity,Price\n";

    // Square off kab karna hai ? after enddate or on enddate currently on enddate
    for (int i = n; i < sz; i++)
    {
        file_1 << dates[i] << "," << final_amt[i] << "\n";
        if (buy_sell[i] != 0)
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[i]) << "," << prices[i] << "\n";
    }

    pnl << final_amt.back() + portfolio * prices[sz - 1] << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

void S1_1(vector<string> &dates, vector<db> &prices, int n, int x)
{
    int sz = prices.size();
    vector<db> final_amt(sz, 0);
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
        final_amt[i] = final_amt[i-1] + buy_sell[i] * prices[i];
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

void S1_2(vector<string> dates, vector<db> prices, int n, int x, db p)
{
    int sz = dates.size(), portfolio = 0;
    vector<int> buy_sell(sz, 0);
    vector<db> DMA(sz, 0), final_amt(sz, 0);
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
        final_amt[i] = final_amt[i-1] + buy_sell[i] * prices[i];
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
    vector<db> final_amt(sz, 0);
    db running_denom = 0, SF = 0.5, AMA = prices[n], ER;
    queue<int> bought, sold;

    // update runningdenom
    for (int i = 0; i < n; i++)
        running_denom += abs(prices[i] - prices[i + 1]);

    for (int i = n; i < sz; i++)
    {
        // Check if no of trades in a day can be > 1 - basically follow AUTOGRADER CONDITIONS !!!!

        // Reason for implementing stop loss before AMA condition:
        // Edge case - You are at the boundary position +x, and the price is increasing.
        // You won't be able to buy directly using AMA condition, but you can sell using stop loss condition and then buy using AMA condition
        // Also, since AMA condition does not take into account the buy and sell history, there is no specific need of order of implementation

        // check for max holding days
        if (earliest_index != 1e9 && i - earliest_index >= mhd)
        {
            // Verify condition when both stop loss and AMA are applicable - to do nothing or to buy then sell etc.

            // We don't need to worry about the max position check here as it always reduces the magnitude of position
            // Here, abs(buy_sell[i]) cannot exceed 1
            // Moreover, since both bought and sold are not non-empty, we only need to check size of either one of them

            if (bought.size() > 0)
            {
                bought.pop();
                if (bought.size() != 0)
                    earliest_index = bought.front();
                else
                    earliest_index = 1e9;
                portfolio--;
                buy_sell[i] += 1;
            }
            else if (sold.size() > 0)
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

        if (prices[i] >= (1 + (p / 100.0)) * AMA && portfolio < x && buy_sell[i] != -1)
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
        else if (prices[i] <= (1 - (p / 100.0)) * AMA && portfolio > -x && buy_sell[i] != 1)
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

        final_amt[i] = final_amt[i-1] + buy_sell[i] * prices[i];

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

void S1_4_1(vector<string> dates, vector<db> prices, int n, int x)
{
    int sz = dates.size(), portfolio = 0;
    vector<int> buy_sell(sz, 0);
    vector<db> final_amt(sz, 0);
    db shortEWM = prices[n], longEWM = prices[n], MACD = 0, signal = 0;
    db shortalpha = 2.0 / 13.0, longalpha = 2.0 / 27.0, MACDalpha = 2.0 / 10.0;
    //Error in cashflow - floating point stuff
    for (int i = n; i < sz; ++i)
    {
        if (MACD > signal && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (MACD < signal && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }

        shortEWM += shortalpha * (prices[i] - shortEWM);
        longEWM += longalpha * (prices[i] - longEWM);
        MACD = shortEWM - longEWM;
        signal += MACDalpha * (MACD - signal);

        final_amt[i] = final_amt[i-1] + buy_sell[i] * prices[i];
    }
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

void S1_4_2(vector<string> dates, vector<db> prices, int n, int x, db overbought, db oversold)
{
    int sz = dates.size(), portfolio = 0;
    vector<int> buy_sell(sz, 0);
    vector<db> final_amt(sz, 0);

    db avgGain = 0, avgLoss = 0, RS, RSI;

    for (int i = 1; i <= n; i++)
    {
        avgGain += max(prices[i] - prices[i - 1], 0.0);
        avgLoss += max(prices[i - 1] - prices[i], 0.0);
    }
    avgGain /= n;
    avgLoss /= n;
    RS = avgGain / avgLoss;
    RSI = 100 - (100 / (1 + RS));

    for (int i = n; i < sz; i++)
    {
        RS = avgGain / avgLoss;
        RSI = 100 - (100 / (1 + RS));

        if (RSI > overbought && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }
        else if (RSI < oversold && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }

        avgGain = (avgGain * (n) + max(prices[i] - prices[i - 1], 0.0) - max(prices[i - n + 1] - prices[i - n], 0.0)) / n;
        avgLoss = (avgLoss * (n) + max(prices[i - 1] - prices[i], 0.0) - max(prices[i - n] - prices[i - n + 1], 0.0)) / n;

        final_amt[i] = final_amt[i-1] + buy_sell[i] * prices[i];
    }

    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

void S1_4_3(vector<string> dates, vector<db> highPrices, vector<db> lowPrices, vector<db> prevClosePrices, vector<db> prices, int n, int x, int adx_threshold)
{
    //In buy_sell, -1 means buy, 1 means sell
    int sz = dates.size(), portfolio = 0;
    vector<int> buy_sell(sz, 0);
    vector<db> final_amt(sz, 0);
    db TR = max(highPrices[n] - lowPrices[n], max(abs(highPrices[n] - prevClosePrices[n]), abs(lowPrices[n] - prevClosePrices[n])));

    // ADX has been set to DX for the start day due to ambiguity in the formula
    db ATR = TR, DMplus = max(0.0, highPrices[n] - highPrices[n - 1]), DMminus = max(0.0, lowPrices[n] - lowPrices[n - 1]), DIplus = DMplus/ATR, DIminus = DMminus/ATR, DX = (DIplus - DIminus)/(DIplus + DIminus), ADX = DX, alpha = 2.0 / (n + 1);
    //handle cases when any of the values in denom become 0
    for (int i = n; i < sz; i++)
    {
        if (ADX > adx_threshold && portfolio < x)
        {
            portfolio++;
            buy_sell[i] = -1;
        }
        else if (ADX < adx_threshold && portfolio > -x)
        {
            portfolio--;
            buy_sell[i] = 1;
        }
        if (i < sz - 1)
        {
            TR = max(highPrices[i + 1] - lowPrices[i + 1], max(abs(highPrices[i + 1] - prevClosePrices[i + 1]), abs(lowPrices[i + 1] - prevClosePrices[i + 1])));
            ATR += alpha * (TR - ATR);
            DIplus += alpha * (DMplus/ATR - DIplus);
            DIminus += alpha * (DMminus/ATR - DIminus);
            DX = (DIplus - DIminus)/(DIplus + DIminus);
            ADX += alpha * (DX - ADX);
        }
        final_amt[i] = final_amt[i-1] + buy_sell[i] * prices[i];
    }
    
    make_csv(dates, prices, buy_sell, portfolio, final_amt, n);
}

int main(int argc, char *argv[])
{
    string strategy = argv[1];
    cout << strategy << endl;

    ifstream file("data.csv");
    string line;

    vector<string> dates;
    vector<db> highPrices, lowPrices, prevClosePrices, prices;

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

        // Get the high price
        if (getline(ss, field, ','))
        {
            highPrices.push_back(stod(field));
        }

        // Get the low price
        if (getline(ss, field, ','))
        {
            lowPrices.push_back(stod(field));
        }

        // Get the previous close price
        if (getline(ss, field, ','))
        {
            prevClosePrices.push_back(stod(field));
        }

        if (getline(ss, field, ','))
        {
            prices.push_back(stod(field));
        }
    }
    file.close();

    int n = stoi(argv[2]);
    int x = stoi(argv[3]);

    if (strategy == "BASIC")
    {
        S1_1(dates, prices, n, x);
    }
    else if (strategy == "DMA")
    {
        db p = stod(argv[4]);
        S1_2(dates, prices, n, x, p);
    }
    else if (strategy == "DMA++")
    {
        db p = stod(argv[4]);
        int mhd = stoi(argv[5]);
        db c1 = stod(argv[6]);
        db c2 = stod(argv[7]);
        S1_3(dates, prices, n, x, p, mhd, c1, c2);
    }
    else if (strategy == "MACD")
    {
        S1_4_1(dates, prices, n, x);
    }
    else if (strategy == "RSI")
    {
        db overbought = stod(argv[4]);
        db oversold = stod(argv[5]);
        S1_4_2(dates, prices, n, x, overbought, oversold);
    }
    else if (strategy == "ADX")
    {
        int adx_threshold = stoi(argv[4]);
        S1_4_3(dates, highPrices, lowPrices, prevClosePrices, prices, n, x, adx_threshold);
    }
    return 0;
}