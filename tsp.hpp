//
//  main.hpp
//  tsp
//
//  Created by Сергей Пиденко on 27.05.2022.
//

#define TSP_hpp

#include <vector>
#include <cmath>
#include <utility>

using namespace std;

struct SPoint {
    pair <int,int> real_coord;
    pair <int,int> absolute_coord;
};

struct v_point {
    SPoint s_point;
    float value;
};


class TSP {
private:
    vector<v_point> ribes;
    float res;
public:
    TSP (vector<vector<float>>& matrix);
    vector<v_point> columns_reduction(vector<vector<float>>& matrix);
    vector<v_point> rows_reduction(vector<vector<float>>& matrix);
    float sumColummRow(const vector<v_point>& cols, const vector<v_point>& rows);
    float matrix_reduction(vector<vector<float>>& matrix);
    vector<SPoint> first_line_S (const vector<vector<float>>& matrix);
    vector<float> d_i(const vector<vector<float>>& matrix, const vector<SPoint>& s);
    vector<float> d_j(const vector<vector<float>>& matrix, const vector<SPoint>& s);
    v_point argmax (const vector<float>& di, const vector<float>& dj, const vector<SPoint>& s);
    vector<vector<float>> matrix_modify(const vector<vector<float>>& matrix, SPoint kl);
    pair<bool, float> NGFC (float old_H, int new_H, v_point kl);
    float recursion (vector<vector<float>>& matrix, float sum);
    float get_res();
    vector<v_point> get_res_path();
    void print_matrix(const vector<vector<float>>& matrix);
};

#endif
