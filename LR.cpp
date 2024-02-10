// Sample code for inverse of matrix
// For normal linear regression, use eqn theta = (X'X)^-1 * X'Y

// // Function to find the determinant of a matrix
// double determinant(vector<vector<double>> matrix) {
//   int n = matrix.size();
//   double det = 1;
//   for (int i = 0; i < n; i++) {
//     for (int j = i + 1; j < n; j++) {
//       det *= matrix[i][i] / matrix[j][i];
//       for (int k = i; k < n; k++) {
//         matrix[j][k] -= matrix[i][k] * det;
//       }
//     }
//   }
//   for (int i = 0; i < n; i++) {
//     det *= matrix[i][i];
//   }
//   return det;
// }

// // Function to find the adjoint of a matrix
// vector<vector<double>> adjoint(vector<vector<double>> matrix) {
//   int n = matrix.size();
//   vector<vector<double>> adjoint(n, vector<double>(n));
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       adjoint[i][j] = cofactor(matrix, i, j);
//     }
//   }
//   return adjoint;
// }

// // Function to find the cofactor of a matrix
// double cofactor(vector<vector<double>> matrix, int i, int j) {
//   int n = matrix.size();
//   vector<vector<double>> minor(n - 1, vector<double>(n - 1));
//   for (int x = 0; x < n - 1; x++) {
//     for (int y = 0; y < n - 1; y++) {
//       if (x < i) {
//         minor[x][y] = matrix[x][y];
//       } else if (x >= i) {
//         minor[x][y] = matrix[x + 1][y];
//       }
//       if (y < j) {
//         minor[x][y] = matrix[x][y];
//       } else if (y >= j) {
//         minor[x][y] = matrix[x][y + 1];
//       }
//     }
//   }
//   return pow(-1, i + j) * determinant(minor);
// }

// // Function to find the inverse of a matrix
// vector<vector<double>> inverse(vector<vector<double>> matrix) {
//   double det = determinant(matrix);
//   if (det == 0) {
//     cout << "Matrix is not invertible" << endl;
//     return vector<vector<double>>();
//   }
//   vector<vector<double>> adjoint = adjoint(matrix);
//   for (int i = 0; i < matrix.size(); i++) {
//     for (int j = 0; j < matrix.size(); j++) {
//       adjoint[i][j] /= det;
//     }
//   }
//   return adjoint;
// }

// // Main function
// int main() {
//   vector<vector<double>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
//   vector<vector<double>> inverseMatrix = inverse(matrix);
//   for (int i = 0; i < matrix.size(); i++) {
//     for (int j = 0; j < matrix.size(); j++) {
//       cout << inverseMatrix[i][j] << " ";
//     }
//     cout << endl;
//   }
//   return 0;
// }
#include <bits/stdc++.h>
// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <string>
// #include <ifstream>
using namespace std;
#define db double

void printMat(vector<vector<db>> v)
{
    int n = v.size();
    for (int i = 0; i < n; ++i)
    {
        for(int j = 0 ; j < v[i].size() ; ++j)
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

vector<vector<db>> matrixMultiply(vector<vector<db>> X1, vector<vector<db>> X2)
{
    int n = X1.size(), m = X1[0].size();
    vector<vector<db>> prod(X1.size(), vector<db>(X2[0].size(), 0));
    if(m == X2.size())
    {
        for(int i = 0 ; i < n ; ++i)
        {
            for(int j = 0 ; j < m ; ++j)
            {
                for(int k = 0 ; k < m ; ++k)
                {
                    prod[i][j] += X1[i][k] * X2[k][j];
                }
            }
        }
    }
    return prod;
}

vector<vector<db>> matrixInverse(vector<vector<db>> matrix) {
    int n = matrix.size();
    vector<vector<db>> inverse(n, vector<db>(n, 0));

    // Initialize the inverse matrix as an identity matrix
    for (int i = 0; i < n; i++) {
        inverse[i][i] = 1.0;
    }

    // Perform Gaussian elimination
    for (int i = 0; i < n; i++) {
        // Find the pivot
        db maxEl = abs(matrix[i][i]);
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(matrix[k][i]) > maxEl) {
                maxEl = abs(matrix[k][i]);
                maxRow = k;
            }
        }

        //Swap maximum row with current row
        for (int k = i; k < n; k++) {
            swap(matrix[maxRow][k], matrix[i][k]);
            swap(inverse[maxRow][k], inverse[i][k]);
        }

        // Make all rows below this one 0 in current column
        for (int k = i + 1; k < n; k++) {
            db c = -matrix[k][i] / matrix[i][i];
            for (int j = 0; j < n; j++) {
                 matrix[k][j] += c * matrix[i][j];
                inverse[k][j] += c * inverse[i][j];
            }
        }
    }

    // Solve equation Ax=b for an upper triangular matrix A
    for (int i = n - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            db c = -matrix[j][i] / matrix[i][i];
            for (int k = 0; k < n; k++) {
                matrix[j][k] += c * matrix[i][k];
                inverse[j][k] += c * inverse[i][k];
            }
        }
    }

    // Normalize diagonal to 1
    for (int i = 0; i < n; i++) {
        db c = 1.0 / matrix[i][i];
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= c;
            inverse[i][j] *= c;
        }
    }
    return inverse;
}


int main(int argc, char *argv[])
{
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

    int sz = dates.size() - 1;
    vector<vector<db>> X(sz);
    vector<db> Y1(sz);
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
        Y1.push_back(closePrices[i + 1]);
    }
    vector<vector<db>> Y = {Y1};
    vector<vector<db>> theta = matrixMultiply(matrixMultiply(matrixInverse(matrixMultiply(matrixTranspose(X), X)), matrixTranspose(X)), Y);
    printMat(theta);
    return 0;
}