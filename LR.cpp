//Sample code for inverse of matrix
//For normal linear regression, use eqn theta = (X'X)^-1 * X'Y

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