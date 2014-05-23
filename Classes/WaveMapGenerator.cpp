//
//  WaveMapGenerator.cpp
//  sampleGame
//
//  Created by 張程 on 2014/05/23.
//
//

#include "WaveMapGenerator.h"

#define PI 3.1415926f

WaveMapGenerator::WaveMapGenerator():
_last_point(Point(0.0f,0.0f))
{
}

WaveMapGenerator* WaveMapGenerator::instance = 0;

WaveMapGenerator* WaveMapGenerator::getInstance()
{
    if (!instance)
    {
        instance = new WaveMapGenerator();
    }
    
    return instance;
}

std::vector<Point> WaveMapGenerator::genContinuousWaves(const Point position, const Size *wave_sizes, const int count, const int smoother, bool const start_with_convex)
{
    std::vector<Point> points;
    points.reserve(smoother);
    // Point数分の容量を確保
    
    bool is_convex = start_with_convex;
    
    float max_length = 0.0f;
    float max_height = 0.0f;
    for (int i = 0; i < count ; i++) {
        max_height+=wave_sizes[i].height;
        max_length+=wave_sizes[i].width;
    }
    float gap = max_length/smoother;
    CCLOG("max_length", max_length);
    CCLOG("Gap : %f", gap);
    Point gap_offset = Point(0.0f,0.0f);
    int smoother_remained = smoother;
    Point next_start_point = position;
    for (int i = 0; i < count; i++) {
        std::vector<Point>the_points;
        Size this_size = wave_sizes[i];
        int num_of_smoother = ceil(smoother * static_cast<float>(this_size.height/max_height));
        
        if (i != count)
        {
            float gap_tri = (atan2(this_size.height,this_size.width)+atan2(wave_sizes[i+1].height, wave_sizes[i+1].width))/2;
            gap_offset = is_convex ? Point(gap,-gap*cos(gap_tri)) : Point(gap, gap*tan(gap_tri));
        } else {
            gap_offset = Point(0.0f,0.0f);
        }
        
        if (smoother_remained < num_of_smoother){
            num_of_smoother = smoother_remained;
        } else {
            smoother_remained-= num_of_smoother;
        }
        
        the_points = this->genPointsInSingleWave(next_start_point, this_size, num_of_smoother, is_convex);
        
        if (is_convex) {
            next_start_point = the_points.back()+gap_offset;
        } else {
            next_start_point = the_points.back()+gap_offset;
        }
        
        
        if (points.size() == 0) {
            points = the_points;
        } else {
            points.insert(points.end(), the_points.begin(), the_points.end());
        }
        is_convex = !is_convex;
    }
    _last_point = points.back();
    
    return points;
}


std::vector<Point> WaveMapGenerator::genPointsInSingleWave(Point const position, Size const size, int const num_of_points, bool const convex)
{
    std::vector<Point> points;
    int const real_num_of_points = num_of_points - 1;
    float const real_width = size.width/real_num_of_points;
    
    for (int i = 0; i < num_of_points; i++) {
        if (convex)
        {
            points.push_back(Point(real_width*i,
                                   size.height*sin(PI*i/real_num_of_points)) + position);
        }
        else
        {
            points.push_back(Point(real_width*i,
                                   size.height*sin(PI+(PI*i/real_num_of_points))) + position);
        }
    }
    
    return points;
}