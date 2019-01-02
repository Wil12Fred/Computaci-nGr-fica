#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "object.h"

//! polygon
class polygon: public object {
public:
    //! constructor
    polygon(matrix<4, 4>& __t, material &__material, vector<3>* __v, int n);

    //! rasterize
    virtual bool rasterize(camera* __camera, unsigned short __w, unsigned short __h,
            vector<3> __ambient, dlight *__dlights, unsigned long __num_dlights, plight *__plights,
            unsigned long __num_plights, float*& __color_buffer, float*& __depth_buffer);

    //! draw_points
    void draw_point(vector<2> p, unsigned int __w, unsigned int __h, float*& __color_buffer);

protected:
    //! vertexs
    vector<3>* _v;
    int _n;//dimension
};

#endif
