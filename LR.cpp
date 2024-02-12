#include <bits/stdc++.h>
#include "LR.h"
using namespace std;
#define db long double

void printMat(vector<vector<db>> v)
{
    int n = v.size();
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < v[i].size(); ++j)
        {
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

vector<vector<db>> matrixTranspose(vector<vector<db>> X)
{
    vector<vector<db>> Xt(X[0].size(), vector<db>(X.size()));
    int n = X.size();
    int m = X[0].size();
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            Xt[i][j] = X[j][i];
        }
    }
    return Xt;
}

void getCofactor(vector<vector<db>> &mat, vector<vector<db>> &temp, int p, int q, int n)
{
    // cofactor only for square matrix
    for (int i = 0, r = 0; i < n; i++)
    {
        if (i == p)
            continue;
        for (int j = 0, c = 0; j < n; j++)
        {
            if (j == q)
                continue;
            temp[r][c] = mat[i][j];
            c++;
        }
        r++;
    }
}

db determinant(vector<vector<db>> &mat, int n)
{
    db D = 0;

    // Base case
    if (n == 1)
        return mat[0][0];

    vector<vector<db>> temp(n - 1, vector<db>(n - 1));

    int sign = 1;

    // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {

        getCofactor(mat, temp, 0, f, n);
        D += sign * mat[0][f] * determinant(temp, n - 1);

        // change sign
        sign = -sign;
    }

    return D;
}

void adjoint(vector<vector<db>> &mat, vector<vector<db>> &adj)
{
    int n = mat.size();
    if (n == 1)
    {
        adj[0][0] = mat[0][0]; //hope never comes
        return;
    }

    // temp is used to store cofactors of A[][]
    int sign = 1;
    vector<vector<db>> temp(n, vector<db>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            getCofactor(mat, temp, i, j, n);

            // sign calculated
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            //transpose and store
            adj[j][i] = (sign) * (determinant(temp, n - 1));
        }
    }
}

vector<vector<db>> inverse(vector<vector<db>>mat)
{
    int n = mat.size();
    
    db det = determinant(mat, n);
    if (det == 0)
    {
        cout << "Singular matrix.\n";
        return {{}};
    }

   
    vector<vector<db>> inv(n, vector<db>(n));
    vector<vector<db>> adj(n, vector<db>(n));
    adjoint(mat, adj);

    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            inv[i][j] = adj[i][j] / det;

    return inv;
}

vector<vector<db>> matrixMultiply(vector<vector<db>> X1, vector<vector<db>> X2)
{
    int n = X1.size(), m = X1[0].size();
    int kk = X2[0].size();
    vector<vector<db>> prod(n, vector<db>(kk, 0));
    if (m == X2.size())
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < kk; ++j)
            {
                for (int k = 0; k < m; ++k)
                {
                    prod[i][j] += X1[i][k] * X2[k][j];
                }
            }
        }
    }
    return prod;
}

vector<vector<db>> trainmodel()
{
    // Training the model using the training data
    ifstream file("traindata.csv");
    string line;

    vector<string> dates;
    vector<db> highPrices;
    vector<db> lowPrices;
    vector<db> prevClosePrices;
    vector<db> openPrices;
    vector<db> closePrices;
    vector<db> vwap;
    vector<int> noOfTrades;

    // skip header
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string date, field;
        getline(ss, field, ',');
        if (getline(ss, date, ','))
            dates.push_back(date);
        if (getline(ss, field, ','))
            highPrices.push_back(stod(field));
        if (getline(ss, field, ','))
            lowPrices.push_back(stod(field));
        if (getline(ss, field, ','))
            prevClosePrices.push_back(stod(field));
        if (getline(ss, field, ','))
            closePrices.push_back(stod(field));
        if (getline(ss, field, ','))
            openPrices.push_back(stod(field));
        if (getline(ss, field, ','))
            vwap.push_back(stod(field));
        if (getline(ss, field))
            noOfTrades.push_back(stoi(field));
    }

    file.close();
    int sz = dates.size() - 1; // sz reduced here
    vector<vector<db>> X(sz);
    vector<vector<db>> Y;
    for (int i = 0; i < sz; i++) // sz again reduced here correct
    {
        X[i].push_back(1);
        X[i].push_back(closePrices[i]);
        X[i].push_back(openPrices[i]);
        X[i].push_back(vwap[i]);
        X[i].push_back(lowPrices[i]);
        X[i].push_back(highPrices[i]);
        X[i].push_back(noOfTrades[i]);
        X[i].push_back(openPrices[i + 1]);
        Y.push_back({closePrices[i + 1]});
    }
    // printMat(matrixTranspose(X));
    vector<vector<db>> theta = matrixMultiply(matrixMultiply(inverse(matrixMultiply(matrixTranspose(X), X)), matrixTranspose(X)), Y);
    return theta;
}

void make_csv_LR(vector<string> &dates, vector<db> &prices, vector<int> &buy_sell, int portfolio, vector<db> &final_amt, int n)
{
    int sz = prices.size();
    ofstream file_1("daily_cashflow.csv"), file_2("order_statistics.csv"), pnl("final_pnl.txt");

    file_1 << "Date,Cashflow\n";
    file_2 << "Date,Order_dir,Quantity,Price\n";

    for (int i = n; i < sz; i++)
    {
        file_1 << dates[i] << "," << to_string(final_amt[i]) << "\n";
        if (buy_sell[i] != 0)
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[i]) << "," << prices[i] << "\n";
    }

    pnl << final_amt.back() + portfolio * prices[sz - 1] << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

void doLR(vector<string> dates, vector<db> prices, db p, int x, vector<db> yy, db &pnl)
{
    dates.erase(dates.begin());
    prices.erase(prices.begin());

    int sz = dates.size();
    vector<int> buy_sell(sz, 0);
    vector<db> final_amt(sz, 0);

    int portfolio = 0;
    for (int i = 0; i < sz; i++)
    {
        if (yy[i] > prices[i] * (1 + p / 100.0) && portfolio < x)
        {
            buy_sell[i] = -1;
            portfolio += 1;
        }
        else if (yy[i] < prices[i] * (1 - p / 100.0) && portfolio > -x)
        {
            buy_sell[i] = 1;
            portfolio -= 1;
        }
        if (i > 0)
            final_amt[i] = final_amt[i - 1];
        final_amt[i] += buy_sell[i] * prices[i];
    }
    pnl = final_amt.back() + portfolio * prices[sz - 1];
    make_csv_LR(dates, prices, buy_sell, portfolio, final_amt, 0);
}

db workLR(vector<string> args)
{
    // getting the arguments
    int x = stoi(args[1]);
    db p = stod(args[2]);

    vector<vector<db>> theta = trainmodel();

    // Model has been trained now and we have the theta values.
    // Now we can use these theta values to predict the stock prices for the next n days.

    ifstream file("testdata.csv");
    string line;

    vector<string> dates;
    vector<db> highPrices;
    vector<db> lowPrices;
    vector<db> prevClosePrices;
    vector<db> openPrices;
    vector<db> closePrices;
    vector<db> vwap;
    vector<int> noOfTrades;

    // skip header
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string date, field;
        getline(ss, field, ',');
        if (getline(ss, date, ','))
            dates.push_back(date);
        if (getline(ss, field, ','))
            highPrices.push_back(stod(field));
        if (getline(ss, field, ','))
            lowPrices.push_back(stod(field));
        if (getline(ss, field, ','))
            prevClosePrices.push_back(stod(field));
        if (getline(ss, field, ','))
            closePrices.push_back(stod(field));
        if (getline(ss, field, ','))
            openPrices.push_back(stod(field));
        if (getline(ss, field, ','))
            vwap.push_back(stod(field));
        if (getline(ss, field))
            noOfTrades.push_back(stoi(field));
    }
    file.close();
    int sz = dates.size() - 1;
    vector<vector<db>> X(sz, vector<db>(0));

    for (int i = 0; i < sz; i++)
    {
        X[i].push_back(1);
        X[i].push_back(closePrices[i]);
        X[i].push_back(openPrices[i]);
        X[i].push_back(vwap[i]);
        X[i].push_back(lowPrices[i]);
        X[i].push_back(highPrices[i]);
        X[i].push_back(noOfTrades[i]);
        X[i].push_back(openPrices[i + 1]);
    }
    // printMat(theta);
    vector<vector<db>> Y = matrixMultiply(X, theta);
    vector<db> yy;
    for (int i = 0; i < sz; i++)
        yy.push_back(Y[i][0]);
    db pnl = 0;
    doLR(dates, closePrices, p, x, yy, pnl);
    // cout << "Printing Y\n";
    // printMat(Y);
    // cout << endl;
    return pnl;
}

