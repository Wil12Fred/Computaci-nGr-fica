#include "triangle.h"
// constructor
triangle::triangle(matrix<4, 4>& __t, material &__material, vector<3> __v1, vector<3> __v2,
        vector<3> __v3) :
        object(__t, __material) {
    // initialize vertices
    _v1 = __v1;
    _v2 = __v2;
    _v3 = __v3;
}

// rasterize
bool triangle::rasterize(camera* __camera, unsigned short __w, unsigned short __h,
        vector<3> __ambient, dlight *__dlights, unsigned long __num_dlights, plight *__plights,
        unsigned long __num_plights, float*& __color_buffer, float*& __depth_buffer,bool *& __zdraw) {
    //<<TODO>> transform, lit, project, rasterize, calculate final color and depth

    // initialize color buffer
    /*__color_buffer = new float[__w * __h * 4];
    for (int i=0;i<__w*__h*4;i++){
        __color_buffer[i]=0;
    }*/

    // initialize depth buffer
    //__depth_buffer = new float[__w * __h];

    // model transform
    vector<4> __v1;
    __v1[0] = _v1[0];
    __v1[1] = _v1[1];
    __v1[2] = _v1[2];
    __v1[3] = 1.0;
    vector<4> __v2;
    __v2[0] = _v2[0];
    __v2[1] = _v2[1];
    __v2[2] = _v2[2];
    __v2[3] = 1.0;
    vector<4> __v3;
    __v3[0] = _v3[0];
    __v3[1] = _v3[1];
    __v3[2] = _v3[2];
    __v3[3] = 1.0;
    __v1 = _t * __v1;
    __v2 = _t * __v2;
    __v3 = _t * __v3;

	vector<4> tvec1,tvec2;
	vector<3> __normal;
	tvec1=__v1-__v2;
	tvec2=__v3-__v2;
	__normal[0]=tvec1[1]*tvec2[2]-tvec1[2]*tvec2[1];
	__normal[1]=tvec1[2]*tvec2[0]-tvec1[0]*tvec2[2];
	__normal[2]=tvec1[0]*tvec2[1]-tvec1[1]*tvec2[0];

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
    __v1 = __ww * (__v1 - __camera->pos());
    __v2 = __ww * (__v2 - __camera->pos());
    __v3 = __ww * (__v3 - __camera->pos());

    vector<2> __pv1,__pv2,__pv3;

    // orthogonal projection
    /*__pv1[0] = __v1[0];
    __pv1[1] = __v1[1];
    __pv2[0] = __v2[0];
    __pv2[1] = __v2[1];
    __pv3[0] = __v3[0];
    __pv3[1] = __v3[1];*/

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
    __pv3[1] *= __h / 10.f;
*/

    /****************************************************
         ************* Ilumination ****************
    ****************************************************/
	//N=normal
	//Ka, Kd, Ks constantes de iluminación
	float K_a=_material._reflective;
	float K_d=0.4;
	float K_s=0.3;
	//Il=intesidad de la luz
	float I_l=1;
	//n=valor de especularidad
	float especularidad=1;
	//Ia=Ka : Ilimuniación ambiental
	float Ia=K_a;
	float Id,Is;
	float ilumination[3];
	vector<3> normal,light,camera,reflection;
	normal=__normal.normalized();

	light[0]=__plights->_pos[0]-__v1[0];
	light[1]=__plights->_pos[1]-__v1[1];
	light[2]=__plights->_pos[2]-__v1[2];
	light=light.normalized();
	camera[0]=__camera->pos()[0]-__v1[0];
	camera[1]=__camera->pos()[1]-__v1[1];
	camera[2]=__camera->pos()[2]-__v1[2];
	camera=camera.normalized();
	//Id=Kd*Il*(Normal.Light) : Iluminación difusa
	Id=K_d*I_l*pow(normal*light,especularidad);
	//Reflection=Light-Normal*(2(Normal*Light))
	reflection=Transformations().reflection(normal,light);
	//Is=Ks*Il*(Reflection*Camera)^n : Iluminación especular
	Is=K_s*I_l*(reflection*camera);
	ilumination[0]=Ia+Id+Is;
	ilumination[0]=1;

	light[0]=__plights->_pos[0]-__v2[0];
	light[1]=__plights->_pos[1]-__v2[1];
	light[2]=__plights->_pos[2]-__v2[2];
	light=light.normalized();
	camera[0]=__camera->pos()[0]-__v2[0];
	camera[1]=__camera->pos()[1]-__v2[1];
	camera[2]=__camera->pos()[2]-__v2[2];
	camera=camera.normalized();
	//Id=Kd*Il*(Normal.Light) : Iluminación difusa
	Id=K_d*I_l*pow(__normal*light,especularidad);
	//Reflection=Light-2N(R_Light*N)
	reflection=Transformations().reflection(normal,light);
	//Is=Ks*Il*(Reflection*Camera)^n : Iluminación especular
	Is=K_s*I_l*(reflection*camera);
	ilumination[1]=Ia+Id+Is;
	ilumination[1]=1;

	light[0]=__plights->_pos[0]-__v3[0];
	light[1]=__plights->_pos[1]-__v3[1];
	light[2]=__plights->_pos[2]-__v3[2];
	light=light.normalized();
	camera[0]=__camera->pos()[0]-__v3[0];
	camera[1]=__camera->pos()[1]-__v3[1];
	camera[2]=__camera->pos()[2]-__v3[2];
	camera=camera.normalized();
	//Id=Kd*Il*(Normal.Light) : Iluminación difusa
	Id=K_d*I_l*pow(__normal*light,especularidad);
	//Reflection=Light-2N(R_Light*N)
	reflection=Transformations().reflection(normal,light);
	//Is=Ks*Il*(Reflection*Camera)^n : Iluminación especular
	Is=K_s*I_l*(reflection*camera);
	ilumination[2]=Ia+Id+Is;
	ilumination[2]=1;

    /****************************************************
    ************* PERSPECTIVE PROJECTION ****************
    ****************************************************/

	us FOV = 100;
	double dist = __camera->pos()[2];
	matrix<4,4> perspective=Transformations().MatrixPerspective(__w,__h,1,-1,FOV,dist);
	vector<4> aux = perspective*__v1;
	__pv1[0] = (aux[0]/aux[3]+1)*__w/2;
	__pv1[1] = (aux[1]/aux[3]+1)*__h/2;
	aux=perspective*__v2;
	__pv2[0] = (aux[0]/aux[3]+1)*__w/2;
	__pv2[1] = (aux[1]/aux[3]+1)*__h/2;
	aux=perspective*__v3;
	__pv3[0] = (aux[0]/aux[3]+1)*__w/2;
	__pv3[1] = (aux[1]/aux[3]+1)*__h/2;

    // RELLENAR AQUI BRESENHAM para trazar
    // las lineas entre los puntos __pv1, __pv2 y __pv3
    // Esto es: trazar la linea entre __pv1 y __pv2,
    // entre __pv1 y __pv3; y entre __pv2 y __pv3

    // CREAR PUNTOS
    Point tri[3];
    tri[0]=Point(__pv1[0],__pv1[1],__v1[2],Color(_material._color[0],_material._color[1],_material._color[2]));
    tri[1]=Point(__pv2[0],__pv2[1],__v2[2],Color(_material._color[0],_material._color[1],_material._color[2]));  
    tri[2]=Point(__pv3[0],__pv3[1],__v3[2],Color(_material._color[0],_material._color[1],_material._color[2]));  
    tri[0].light=ilumination[0];
    tri[1].light=ilumination[1];
    tri[2].light=ilumination[2];

    // CREAR LINEAS
    Line pLines[3];
    pLines[0]=Line(tri[0],tri[1]);
    pLines[1]=Line(tri[0],tri[2]);
    pLines[2]=Line(tri[1],tri[2]);

    /****************************************************
    ******** CLIPPING COHENSUTHERLAND + BRESENHAM *******
    ****************************************************/
    /*Bresenham linesCS[3];
    for (int i=0;i<3;i++){
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
    SutherlandHodgeman SH(3,pLines);
    Line* npLines;
    // ++ Clipping de acuerdo a Classroom: Computación Gráfica 2018-1
    // ++ Algoritmo SutherlandHodgeman
    int sizeSH=SH.clipping(Point(0,0),Point(__w,__h),npLines);
    // ++ Bresenham
    /*Bresenham linesSH[sizeSH];
    for (int i=0;i<sizeSH;i++){
            linesSH[i].generatePoints(npLines[i]);
            int n=linesSH[i].size;
            for (int j=0;j<n;j++){
                vector<2,int> pvnew;
                pvnew[0]=linesSH[i].points[j].x;
                pvnew[1]=linesSH[i].points[j].y;
		draw_point(pvnew,__w,__h,__color_buffer);//COMENTAR SI SOLO RASTERIZAR SCANLINE
	    }
    }*/
    // ++ Scanline de acuerdo a Classroom: Computación Gráfica 2018-1
    ScanLine SL(sizeSH,npLines);
    std::vector<Point> scan;
    SL.draw_points(scan,true);
    bool isdraw=true;
    for (int i=0;i<scan.size();i++){
	vector<2,int> pvnew;
	vector<4> color;
	pvnew[0]=scan[i].x;
	pvnew[1]=scan[i].y;
	color[0]=scan[i].color.get(0);
	color[1]=scan[i].color.get(1);
	color[2]=scan[i].color.get(2);
        color[3]=scan[i].z;
	if (!draw_point(pvnew,color, __w,__h,__color_buffer,__depth_buffer,__zdraw)){
		isdraw=false;
	}
    }
    /****************************************************/

    //draw_point(__pv1, __w, __h, __color_buffer);
    //draw_point(__pv2, __w, __h, __color_buffer);
    //draw_point(__pv3, __w, __h, __color_buffer);
    // everything is alright
    return true;
}

void triangle::draw_point(vector<2> p, unsigned int __w, unsigned int __h, float*& __color_buffer) {
    int __x = p[0];
    int __y = p[1];
    __color_buffer[(__y * __w + __x) * 4 + 0] = _material._color[0];
    __color_buffer[(__y * __w + __x) * 4 + 1] = _material._color[1];
    __color_buffer[(__y * __w + __x) * 4 + 2] = _material._color[2];
    __color_buffer[(__y * __w + __x) * 4 + 3] = _material._color[3];
}

bool triangle::draw_point(vector<2> p, vector<4>c, unsigned int __w, unsigned int __h, float*& __color_buffer, float*& __zbuffer,bool *& __zdraw) {
    int __x = p[0];
    int __y = p[1];
    if(__zdraw[__y * __w + __x]) {
	if (c[3]<=__zbuffer[__y*__w+__x]){
	//if (c[3]>=__color_buffer[(__y*__w+__x)*4+3]){
	    __color_buffer[(__y * __w + __x) * 4 + 0] = c[0];
	    __color_buffer[(__y * __w + __x) * 4 + 1] = c[1];
	    __color_buffer[(__y * __w + __x) * 4 + 2] = c[2];
		__color_buffer[(__y * __w + __x) * 4 + 3] = 1.0f;//c[3];//_material._color[3];
		__zbuffer[__y * __w+ __x] = c[3];
	} else {
		return false;
	}
    } else {
	
        __zdraw[__y * __w+ __x] = 1;
        __zbuffer[__y * __w+ __x] = c[3];
	    __color_buffer[(__y * __w + __x) * 4 + 0] = c[0];
	    __color_buffer[(__y * __w + __x) * 4 + 1] = c[1];
	    __color_buffer[(__y * __w + __x) * 4 + 2] = c[2];
	__color_buffer[(__y * __w + __x) * 4 + 3] = 1.0f;//c[3];//_material._color[3];
    }
    return true;
}

