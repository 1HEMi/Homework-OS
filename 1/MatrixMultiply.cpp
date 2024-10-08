#include <iostream>  
#include <vector>  
#include <random>  
#include <cassert>  
#include <future>  
#include "windows.h"  
#include "logduration.h"  

using namespace std;  

void pMatrix(const vector<vector<int>>& mat) {  
    for (const auto& row : mat) {  
        for (const auto& elem : row) {  
            cout << elem << " ";  
        }  
        cout << endl;  
    }  
    cout << endl;  
}  

vector<vector<int>> getTranspose(const vector<vector<int>>& mat) {  
    int rows = mat.size();  
    int cols = mat[0].size();  
    vector<vector<int>> transposed(cols, vector<int>(rows));  

    for (int i = 0; i < rows; i++) {  
        for (int j = 0; j < cols; j++) {  
            transposed[j][i] = mat[i][j];  
        }  
    }  

    return transposed;  
}  

int calculateStandardMult(const vector<int>& vec, const vector<vector<int>>& mat, int col) {  
    int result = 0;  
    for (size_t i = 0; i < vec.size(); ++i) {  
        result += vec[i] * mat[i][col];  
    }  
    return result;  
}  

int calculateOptimizedMult(const vector<int>& vec, const vector<int>& row) {  
    int result = 0;  
    for (size_t i = 0; i < vec.size(); ++i) {  
        result += vec[i] * row[i];  
    }  
    return result;  
}  

void parallelMultiply(const vector<vector<int>>& matA, const vector<vector<int>>& matB, vector<vector<int>>& result, int start, int end) {  
    for (size_t i = start; i < end; i++) {  
        for (size_t j = 0; j < matA[i].size(); j++) {  
            result[i][j] += calculateStandardMult(matA[i], matB, j);  
        }  
    }  
}  

vector<vector<int>> sequentialMultiplyWithoutOptimization(const vector<vector<int>>& matA, const vector<vector<int>>& matB) {  
    LOG_DURATION("sequential multiply without optimizations");  
    vector<vector<int>> result(matA.size(), vector<int>(matB.size()));  
    for (size_t i = 0; i < matA.size(); i++) {  
        for (size_t j = 0; j < matB[0].size(); j++) {  
            result[i][j] = calculateStandardMult(matA[i], matB, j);  
        }  
    }  
    return result;  
}  

vector<vector<int>> sequentialMultiplyWithOptimization(const vector<vector<int>>& matA, const vector<vector<int>>& matB) {  
    LOG_DURATION("sequential multiply with optimizations");  
    auto transposedB = getTranspose(matB);  
    vector<vector<int>> result(matA.size(), vector<int>(transposedB.size()));  
    for (size_t i = 0; i < matA.size(); i++) {  
        for (size_t j = 0; j < transposedB.size(); j++) {  
            result[i][j] = calculateOptimizedMult(matA[i], transposedB[j]);  
        }  
    }  
    return result;  
}  

vector<vector<int>> parallelMatrixMultiply(const vector<vector<int>>& matA, const vector<vector<int>>& matB) {  
    LOG_DURATION("parallel matrix multiply");  
    vector<vector<int>> result(matA.size(), vector<int>(matB[0].size()));  
    auto firstHalf = async(launch::async, parallelMultiply, ref(matA), ref(matB), ref(result), 0, matA.size() / 2);  
    auto secondHalf = async(launch::async, parallelMultiply, ref(matA), ref(matB), ref(result), matA.size() / 2, matA.size());  
    firstHalf.get(); // Wait for the first half to finish  
    secondHalf.get(); // Wait for the second half to finish  
    return result;  
}  

vector<vector<int>> generateMatrix(int matrixNumber) {  
    int rows, cols;  
    cout << "Введите количество столбцов и строк для " << matrixNumber << " матрицы:\n";  
    cin >> cols >> rows;  
    vector<vector<int>> mat(rows, vector<int>(cols));  
    random_device rd;  
    mt19937 gen(rd());  
    uniform_int_distribution<> distrib(0, 10);  
    
    for (size_t i = 0; i < rows; ++i) {  
        for (size_t j = 0; j < cols; ++j) {  
            mat[i][j] = distrib(gen);  
        }  
    }  
    return mat;
