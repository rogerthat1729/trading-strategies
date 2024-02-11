#ifndef LR_H
#define LR_H

#include <bits/stdc++.h>
using namespace std;
#define db double

void printMat(vector<vector<db>> v);

vector<vector<db>> matrixTranspose(vector<vector<db>> X);

void getCofactor(vector<vector<double>> &mat, vector<vector<double>> &temp, int p, int q, int n);

double determinant(vector<vector<double>> &mat, int n);

void adjoint(vector<vector<double>> &A, vector<vector<double>> &adj);

vector<vector<db>> inverse(vector<vector<double>> A);

vector<vector<db>> matrixMultiply(vector<vector<db>> X1, vector<vector<db>> X2);

vector<vector<db>> trainmodel();

void make_csv_LR(vector<string> &dates, vector<db> &prices, vector<int> &buy_sell, int portfolio, vector<db> &final_amt, int n);

db workLR(int argc, vector<string> args);

#endif