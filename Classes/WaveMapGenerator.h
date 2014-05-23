//
//  WaveMapGenerator.h
//  sampleGame
//
//  Created by 張程 on 2014/05/23.
//
//

#ifndef __WAVE_MAP_GENERATOR__
#define __WAVE_MAP_GENERATOR__

#include "cocos2d.h"

USING_NS_CC;

class WaveMapGenerator
{
    WaveMapGenerator();
    static WaveMapGenerator *instance;
    
public:
    static WaveMapGenerator* getInstance();

    std::vector<Point> genContinuousWaves(const Point position, const Size* wave_sizes, int const count, int const smoother, bool const start_with_convex = true);
    
    std::vector<Point> genPointsOfStartWave(Point const position, Size const size, int const num_of_points, bool const convex);
    std::vector<Point> genPointsOfEndWave(Point const position, Size const size, int const num_of_points, bool const convex);
    std::vector<Point> genPointsInSingleWave(Point const position, Size const size, int const num_of_points, bool const convex);
    
    
    
private:
    Point _last_point;
};

#endif /* defined(__sampleGame__WaveMapGenerator__) */
