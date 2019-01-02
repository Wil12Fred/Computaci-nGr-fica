#include "polygon.h"
#include "geometry.h"

// constructor
polygon::polygon(matrix<4, 4>& __t, material &__material, vector<3>* __v, int n) :
        object(__t, __material) {
    // initialize vertices
	_v=new vector<3>[n];
	for (int i=0;i<n;i++){
		_v[i]=__v[i];
	}
	_n=n;
}

// rasterize
bool polygon::rasterize(camera* __camera, unsigned short __w, unsigned short __h,
        vector<3> __ambient, dlight *__dlights, unsigned long __num_dlights, plight *__plights,
        unsigned long __num_plights, float*& __color_buffer, float*& __depth_buffer) {
    //<<TODO>> transform, lit, project, rasterize, calculate final color and depth

    // initialize color buffer

    __color_buffer = new float[__w * __h * 4];
    for (int i=0;i<__w*__h*4;i++){
        __color_buffer[i]=0;
    }

    // initialize depth buffer
    __depth_buffer = new float[__w * __h];

    // model transform

    vector<4> __v[_n];
    for (int i=0;i<_n;i++){
	for (int j=0;j<3;j++){
          __v[i][j]=_v[i][j];
	}
	__v[i][3]=1.0;
    }
    // world
    matrix<4, 4> __ww;
    __ww[0][0] = __camera->right()[0];
    __ww[1][0] = __camera->right()[1];
    __ww[2][0] = __camera->right()[2];
    __ww[0][1] = __camera->up()[0];
    __ww[1][1] = __camera->up()[1];
    __ww[2][1] = __camera->up()[2];
    __ww[0][2] = __camera->dir()[0];
    __ww[1][2] = __camera->dir()[1];
    __ww[2][2] = __camera->dir()[2];

    // world transform
	for (int i=0;i<_n;i++){
		__v[i]=__ww*(__v[i]-__camera->pos());
	}
    // orthogonal projection
    vector<2> __pv[_n];
	for (int i=0;i<_n;i++){
		__pv[i][0]=__v[i][0];
		__pv[i][1]=__v[i][1];
		printf("%.3f %.3f \n",__v[i][0],__pv[i][1]);
	}

    /*
    // simulating a frustrum
    __pv1[0] += 5.f;
    __pv1[0] *= __w / 10.f;
    __pv1[1] += 5.f;
    __pv1[1] *= __h / 10.f;
    __pv2[0] += 5.f;
    __pv2[0] *= __w / 10.f;
    __pv2[1] += 5.f;
    __pv2[1] *= __h / 10.f;
    __pv3[0] += 5.f;
    __pv3[0] *= __w / 10.f;
    __pv3[1] += 5.f;
    __pv3[1] *= __h / 10.f;*/
	Point tri[_n];
	for (int i=0;i<_n;i++){
		tri[i]=Point(__pv[i][0],__pv[i][1]);
	}
	Line pLines[_n];
	for (int i=0;i<_n-1;i++){
		pLines[i]=Line(tri[i],tri[i+1]);
	}
	pLines[_n-1]=Line(tri[_n-1],tri[0]);
     /****************************************************                
     ******** CLIPPING COHENSUTHERLAND + BRESENHAM *******                
     ****************************************************/                
     /*Bresenham linesCS[_n];
     for (int i=0;i<_n;i++){                                               
         CohenSutherland CS(pLines[i]);                                   
         // ++ Clipping de acuerdo a Classroom: Computación Gráfica 2018-1
         // ++ Algoritmo CohenSuther                                      
         if(CS.clipping(Point(0,0),Point(__w,__h))){                      
             // ++ Bresenham                                              
             linesCS[i].generatePoints(CS.L);                             
             int n=linesCS[i].size;                                       
             for (int j=0;j<n;j++){                                       
                 vector<2> pvnew;                                         
                 pvnew[0]=linesCS[i].points[j].x;                         
                 pvnew[1]=linesCS[i].points[j].y;                         
                 draw_point(pvnew,__w,__h,__color_buffer);                
             }                                                            
         }                                                                
     }*/
     /***************************************************/                


     /******************************************************                                         
     * CLIPPING SUTHERLANDHODGEMAN + BRESENHAM + Scanline **                                         
     *******************************************************/                                        
     SutherlandHodgeman SH(_n,pLines);                                                                
     Line* npLines;                                                                                  
     // ++ Clipping de acuerdo a Classroom: Computación Gráfica 2018-1                               
     // ++ Algoritmo SutherlandHodgeman                                                              
     int sizeSH=SH.clipping(Point(0,0),Point(__w,__h),npLines);                                      
     Bresenham linesSH[sizeSH];                                                                      
     for (int i=0;i<sizeSH;i++){                                                                     
             // ++ Bresenham                                                                         
             linesSH[i].generatePoints(npLines[i]);                                                  
             int n=linesSH[i].size;
             for (int j=0;j<n;j++){                                                                  
                 vector<2,int> pvnew;                                                                
                 pvnew[0]=linesSH[i].points[j].x;                                                    
                 pvnew[1]=linesSH[i].points[j].y;                                                    
                 draw_point(pvnew,__w,__h,__color_buffer);//COMENTAR SI SOLO RASTERIZAR SCANLINE     
             }
     }
     // ++ Scanline de acuerdo a Classroom: Computación Gráfica 2018-1                               
     ScanLine SL(sizeSH,npLines);                                                                    
     std::vector<Point> scan;                                                                        
     SL.draw_points(scan);                                                                           
     for (int i=0;i<scan.size();i++){                                                                
         vector<2,int> pvnew;                                                                        
         pvnew[0]=scan[i].x;                                                                         
         pvnew[1]=scan[i].y;                                                                         
         draw_point(pvnew,__w,__h,__color_buffer);                                                   
     }
     /****************************************************/                                          
                                                                                                     

    // RELLENAR AQUI BRESENHAM para trazar
    // las lineas entre los puntos __pv1, __pv2 y __pv3
    // Esto es: trazar la linea entre __pv1 y __pv2,
    // entre __pv1 y __pv3; y entre __pv2 y __pv3

    //draw_point(__pv1, __w, __h, __color_buffer);
    //draw_point(__pv2, __w, __h, __color_buffer);
    //draw_point(__pv3, __w, __h, __color_buffer);
    // everything is alright
    return true;
}

void polygon::draw_point(vector<2> p, unsigned int __w, unsigned int __h, float*& __color_buffer) {
    int __x = p[0];
    int __y = p[1];
    __color_buffer[(__y * __w + __x) * 4 + 0] = _material._color[0];
    __color_buffer[(__y * __w + __x) * 4 + 1] = _material._color[1];
    __color_buffer[(__y * __w + __x) * 4 + 2] = _material._color[2];
    __color_buffer[(__y * __w + __x) * 4 + 3] = _material._color[3];
}

