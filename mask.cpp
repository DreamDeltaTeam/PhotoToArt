#include "mask.h"

Mask makePolygon(vector <Point> points,int w,int h)
{
    int y_min = h, y_max = 0;
    Mask m(w,h);
    for(int i = 0; i < points.size(); i++) {
        if (points[i].y<y_min) {
            y_min = points[i].y;
        }

        if (points[i].y>y_max) {
            y_max = points[i].y;
        }
    }
    for (int y = y_min; y<=y_max;y++){
        vector<int> found;
        for(int i = 0; i < points.size(); i++) {
            Point p1 = points[i];
            Point p2 = points[(i+1)%points.size()];
            if((p1.y==p2.y) && (p2.y==y)){
                if(p1.x>p2.x) {
                    std::swap(p1, p2);
                }
                for (int x=p1.x;x<=p2.x;x++){
                    m.putOne(x,y);
                }
                continue;
            }

            if((p1.y-y)*(p2.y-y)<=0){
                int x = intersection(p1,p2,y);
                if(std::find(found.begin(), found.end(), x) == found.end())
                    found.push_back(x);
            }
        }

        sort(found.begin(), found.end());

        if(found.size() == 1) {
            m.putOne(found[0],y);
        }

        if(found.size() > 1) {
            for(int i = 0; i < found.size() - 1; i+=2) {
                for(int x=found[i];x<=found[i+1];x++){
                    if (x<0) break;
                    if (x>w) break;
                    if (y<0) break;
                    if (y>h) break;
                    m.putOne(x,y);
                }
            }
        }
    }

    return m;
}

Mask genQuad(int x,int y,int side,int w,int h){
    Point p1 = {x,y};
    Point p2 = {x+side,y};
    Point p3 = {x+side,y+side};
    Point p4 = {x,y+side};
    vector<Point> quad = {p1,p2,p3,p4};
    return makePolygon(quad,w,h);
}
