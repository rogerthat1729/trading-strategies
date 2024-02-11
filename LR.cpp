#include <bits/stdc++.h>
#include "LR.h"
using namespace std;
#define db double

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

// Recursive function for finding determinant of matrix.
db determinant(vector<vector<db>> &mat, int n)
{
    db D = 0;

    // Base case : if matrix contains single element
    if (n == 1)
        return mat[0][0];

    vector<vector<db>> temp(n, vector<db>(n)); // To store cofactors

    int sign = 1; // To store sign multiplier

    // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of mat[0][f]
        getCofactor(mat, temp, 0, f, n);
        D += sign * mat[0][f] * determinant(temp, n - 1);

        // terms are to be added with alternate sign
        sign = -sign;
    }

    return D;
}

// Function to get adjoint of A in adj[][]
void adjoint(vector<vector<db>> &A, vector<vector<db>> &adj)
{
    int N = A.size();
    if (N == 1)
    {
        adj[0][0] = 1;
        return;
    }

    // temp is used to store cofactors of A[][]
    int sign = 1;
    vector<vector<db>> temp(N, vector<db>(N));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, N);

            // sign of adj[j][i] positive if sum of row and column indexes is even.
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            // Interchanging rows and columns to get the transpose of the cofactor matrix
            adj[j][i] = (sign) * (determinant(temp, N - 1));
        }
    }
}

// Function to calculate and store inverse, returns false if matrix is singular
vector<vector<db>> inverse(vector<vector<db>> A)
{
    int N = A.size();
    // Find determinant of A[][]
    db det = determinant(A, N);
    if (det == 0)
    {
        cout << "Singular matrix, can't find its inverse";
        return {{}};
    }

    // Find adjoint
    vector<vector<db>> inv(N, vector<db>(N));
    vector<vector<db>> adj(N, vector<db>(N));
    adjoint(A, adj);

    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
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
    int sz = dates.size() - 1;
    vector<vector<db>> X(sz);
    vector<vector<db>> Y;
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
        Y.push_back({closePrices[i + 1]});
    }
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
        file_1 << dates[i] << "," << final_amt[i] << "\n";
        if (buy_sell[i] != 0)
            file_2 << dates[i] << "," << (buy_sell[i] < 0 ? "BUY" : "SELL") << "," << abs(buy_sell[i]) << "," << prices[i] << "\n";
    }

    pnl << final_amt.back() + portfolio * prices[sz - 1] << "\n";

    file_1.close();
    file_2.close();
    pnl.close();
}

void doLR(vector<string> dates, vector<db> prices, db p, int x, vector<db> yy, db& pnl)
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
        if(i>0)
            final_amt[i] = final_amt[i-1];
        final_amt[i] += buy_sell[i] * prices[i];
    }
    pnl = final_amt.back() + portfolio * prices[sz - 1];
    make_csv_LR(dates, prices, buy_sell, portfolio, final_amt, 0);
}

db workLR(int argc, vector<string> args)
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

