//
//  main.cpp
//  tsp
//
//  Created by Сергей Пиденко on 27.05.2022.
//

#include "TSP.hpp"
#include <iostream>

using namespace std;

TSP::TSP (vector<vector<float> >& matrix) {
    float sum = matrix_reduction(matrix);
    res = recursion(matrix, sum);
}


float TSP::get_res() {
    return res;
}

vector<v_point> TSP::get_res_path() {
    return ribes;
}
float TSP::recursion (vector<vector<float> >& matrix, float sum) {
//    print_matrix(matrix);
    if (matrix.size() > 3) {
        vector<SPoint> s = first_line_S(matrix);
        vector<float> di = d_i(matrix, s);
        vector<float> dj = d_j(matrix, s);
        v_point arg_max = argmax(di, dj, s);
        vector<vector<float>> resize_matrix = matrix_modify(matrix, arg_max.s_point);
        matrix.clear();
 //       print_matrix(resize_matrix);
        float new_sum = matrix_reduction(resize_matrix);
        auto par = NGFC(sum, new_sum, arg_max);
        if (par.first) {
            v_point ribe;
            ribe.value = par.second;
            ribe.s_point = arg_max.s_point;
            ribes.push_back(ribe);
        } else {
            v_point ribe;
            ribe.value = par.second;
            ribe.s_point.absolute_coord.first = arg_max.s_point.absolute_coord.second;
            ribe.s_point.absolute_coord.second = arg_max.s_point.absolute_coord.first;
            ribes.push_back(ribe);
        }
        s.clear();
        di.clear();
        dj.clear();
        return recursion(resize_matrix, sum + new_sum);
    } else {
        for(int i = 1; i <= 2; i++){
            for(int j = 1; j <= 2; j++){
                if(matrix[i][j] < MAXFLOAT){
                    v_point ribe;
                    ribe.value = sum;
                    ribe.s_point.absolute_coord.first = matrix[i][0];
                    ribe.s_point.absolute_coord.second = matrix[0][j];
                    ribes.push_back(ribe);
                }
            }
        }
        matrix.clear();
        return ribes.back().value;
    }
    
}

pair<bool, float> TSP::NGFC (float old_H, int new_H, v_point kl) {
    float neg = old_H + kl.value;
    float pos = old_H + new_H;
    if (pos <= neg) {
        auto p = make_pair(true, pos);
        return p;
    } else {
        auto p = make_pair(false, neg);
        return p;
    }
}

float TSP::matrix_reduction(vector<vector<float> >& matrix){
    vector<v_point> min_cols = columns_reduction(matrix);
    vector<v_point> min_rows = rows_reduction(matrix);
    return sumColummRow(min_cols,min_rows);
}

float TSP::sumColummRow(const vector<v_point>& cols, const vector<v_point>& rows){
    float sum = 0;
    for(auto c : cols){
        sum += c.value;
    }
    for(auto r : rows){
        sum += r.value;
    }
    return sum;
}

vector<v_point> TSP::columns_reduction(vector<vector<float> >& matrix) {
    int v_size = matrix.size();
    vector<v_point> column_value;
    pair <int,int> min_pair;
    for (int i = 1; i < v_size; i++) {
        float min = MAXFLOAT;
        for(int j = 1; j < v_size; j++){
            if(matrix[i][j] < min){
                min = matrix[i][j];
                min_pair.first = i;
                min_pair.second = j;
            }
        }
        v_point point;
        point.s_point.real_coord = min_pair;
        point.value = min;
        column_value.push_back(point);
    }
    for(auto k : column_value){
        for(int j = 1; j <= column_value.size(); j++){
            matrix[k.s_point.real_coord.first][j] -= k.value;
        }
    }
    return column_value;
}

vector<v_point> TSP::rows_reduction(vector<vector<float> >& matrix) {
    int v_size = matrix.size();
    vector<v_point> row_value;
    pair <int,int> min_pair;
    for (int j = 1; j < v_size; j++) {
        float  min = MAXFLOAT;
        for(int i = 1; i < v_size; i++){
            if(matrix[i][j] < min){
                min = matrix[i][j];
                min_pair.first = i;
                min_pair.second = j;
            }
        }
        v_point point;
        point.s_point.real_coord = min_pair;
        point.value = min;
        row_value.push_back(point);
    }
    for(auto k : row_value){
        for(int i = 1; i <= row_value.size(); i++){
            matrix[i][k.s_point.real_coord.second] -= k.value;
        }
    }
    return row_value;
}

vector<float> TSP::d_i (const vector<vector<float> >& matrix, const vector<SPoint>& s) {
    int v_size = matrix.size();
    vector<float> result;
    for (auto s_p : s) {
        float min = MAXFLOAT;
        for (int j = 1; j < v_size; j++) {
            if (matrix[s_p.real_coord.first][j] < min && j != s_p.real_coord.second) {
                min = matrix[s_p.real_coord.first][j];
            }
        }
        result.push_back(min);
    }
    return result;
}
vector<float> TSP::d_j (const vector<vector<float> >& matrix, const vector<SPoint>& s) {
    int v_size = matrix.size();
    vector<float> result;
    for (auto s_p : s) {
        float min = MAXFLOAT;
        for (int i = 1; i < v_size; i++) {
            if (matrix[i][s_p.real_coord.second] < min && i != s_p.real_coord.first) {
                min = matrix[i][s_p.real_coord.second];
            }
        }
        result.push_back(min);
    }
    return result;
}

vector<SPoint> TSP::first_line_S (const vector<vector<float> >& matrix) {
    int v_size = matrix.size();
    vector<SPoint> first_line;
    for (int i = 1; i < v_size; i++) {
        for (int j = 1; j < v_size; j++) {
            if (matrix[i][j] == 0) {
                SPoint s_p;
                s_p.real_coord = make_pair(i, j);
                s_p.absolute_coord = make_pair(matrix[i][0], matrix[0][j]);
                first_line.push_back(s_p);
            }
        }
    }
    return first_line;
}

v_point TSP::argmax (const vector<float>& di, const vector<float>& dj, const vector<SPoint>& s) {
    v_point p;
    float max = -1;
    for (int i = 0; i < di.size(); i++) {
        float temp = di[i] + dj[i];
        if (temp > max) {
            max = temp;
            p.value = max;
            p.s_point.absolute_coord = s[i].absolute_coord;
            p.s_point.real_coord = s[i].real_coord;
        }
    }
    return p;
}

vector<vector<float>> TSP::matrix_modify(const vector<vector<float> >& matrix, SPoint kl){
    vector<vector<float>> resize_matrix;
    int size = matrix.size();
    float a_first;
    float a_second;
    float first;
    float second;
    second = kl.real_coord.second;

    for(int i = 1; i < size; i++){
        if(matrix[i][second] == MAXFLOAT){
            a_second = matrix[i][0];
            break;
        }
    }
    
    first = kl.real_coord.first;

    for(int j = 1; j < size; j++){
        if(matrix[first][j] == MAXFLOAT){
            a_first = matrix[0][j];
            break;
        }
    }
    
    for(int i = 0; i < size; i++){
        if(matrix[i][0] != kl.absolute_coord.first){
            vector<float> resize_row;
            for(int j = 0; j < size; j++){
                if(matrix[0][j] != kl.absolute_coord.second){
                    resize_row.push_back(matrix[i][j]);
                }
            }
            resize_matrix.push_back(resize_row);
        }
    }
    
    size--;
    for(int i = 1; i < size; i++){
        if(resize_matrix[i][0] == a_second){
            first = i;
            break;
        }
    }
    
    for(int j = 1; j < size; j++){
        if(resize_matrix[0][j] == a_first){
            second = j;
            break;
        }
    }

    
    resize_matrix[first][second] = MAXFLOAT;
    return resize_matrix;
    

void TSP::print_matrix(const vector<vector<float> >& matrix){
    cout << endl;
    auto v_size = matrix.size();
    for (auto i = 0; i < v_size; i++) {
        for (auto j = 0; j < v_size; j++) {
            if(matrix[i][j] == MAXFLOAT){
                cout << "nf" << " ";
            }else{
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
    
}
