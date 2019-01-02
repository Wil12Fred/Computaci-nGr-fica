#include <algorithm>
#include <vector>
#include <set>
#include <cmath>

typedef unsigned short us;

struct Color{
	std::vector<float>* color;
	Color(){
		color=0;
	}
	Color(float a, float b, float c){
		color = new std::vector<float>(3);
		(*color)[0]=a;
		(*color)[1]=b;
		(*color)[2]=c;
	}
	void set(float a, float b, float c){
		color = new std::vector<float>(3);
		(*color)[0]=a;
		(*color)[1]=b;
		(*color)[2]=c;
	}
	void set(int pos, float a){
		if(!color){
			color = new std::vector<float>(3);
		}
		(*color)[pos]=a;
	}
	float get(int pos)const{
		return (*color)[pos];
	}
	Color(Color& A){
		color=A.color;
	}
	Color(const Color& A){
		color=A.color;
	}
	Color operator=(const Color& B){
		if(B.color==0){
			color=0;
		} else {
			set(B.get(0),B.get(1),B.get(2));
		}
		return *(this);
	}
};

struct Point{
	int x, y;
	float z;
	Color color;
	float light;
	Point(){}
	Point(int vx,int vy){
		x=vx;
		y=vy;
		z=0;
		light=1;
	}
	Point(int vx,int vy,Color _color,float _light=1){
		x=vx;
		y=vy;
		z=0;
		color=_color;
		light=_light;
	}
	Point(int vx,int vy, float vz, Color _color,float _light=1){
		x=vx;
		y=vy;
		z=vz;
		color=_color;
		light=_light;
	}
	Point(int vx,int vy,float vz,float _light=1){
		x=vx;
		y=vy;
		z=vz;
		light=_light;
	}
	Point(Point& A){
		x=A.x;
		y=A.y;
		z=A.z;
		color=A.color;
		light=A.light;
	}
	Point(const Point& A){
		x=A.x;
		y=A.y;
		z=A.z;
		color=A.color;
		light=A.light;
	}
	Point operator=(const Point& B){
		x=B.x;
		y=B.y;
		z=B.z;
		color=B.color;
		light=B.light;
		return *(this);
	}
	bool operator <(const Point B)const{
		if(x==B.x)
			return (y==B.y)?z<B.z:y<B.y;
		return x<B.x;
	}
	bool operator ==(const Point B)const{
		return (x==B.x && y==B.y && z==B.z);
	}
	int TBRL(Point Base, Point Top){
		int ans=0;
		if(x<Base.x){
			ans++;
		}
		if(x>=Top.x){
			ans+=2;
		}
		if(y<Base.y){
			ans+=4;
		}
		if(y>=Top.y){
			ans+=8;
		}
		return ans;
	}
};

struct Line{
	struct comp{
		bool compPointY(const Point& F,const Point& S){
			if(F.y==S.y)
				return (F.x==S.x)?F.z<S.z:F.x<S.x;
			return F.y<S.y;
		}
		bool operator()(const Point& F,const Point& S){
			return compPointY(F,S);
		}
	} myobject;
	Point F,S;
	int dx,dy;
	void calcDif(){
		dy=S.y-F.y;
		dx=S.x-F.x;
	}
	Point vector(){
		return Point(S.x-F.x,S.y-F.y);
	}
	Line(){}
	Line(Point& _F, Point& _S){
		F=_F;
		S=_S;
		if(!(comp().compPointY(F,S))){
			std::swap(F,S);
		}
		calcDif();
	}
	Line(Point& _F, Point& _S,int _dx,int _dy){
		F=_F;
		S=_S;
		if(!(comp().compPointY(F,S))){
			std::swap(F,S);
		}
		dx=_dx;
		dy=_dy;
	}
	Line(const Point& _F,const Point& _S){
		F=_F;
		S=_S;
		if(!(comp().compPointY(F,S))){
			std::swap(F,S);
		}
		calcDif();
	}
	Line(const Point& _F,const Point& _S,int _dx,int _dy){
		F=_F;
		S=_S;
		if(!(comp().compPointY(F,S))){
			std::swap(F,S);
		}
		dx=_dx;
		dy=_dy;
	}
	Line(Line& A){
		F=A.F;
		S=A.S;
		dx=A.dx;
		dy=A.dy;
	}
	Line(const Line& A){
		F=A.F;
		S=A.S;
		dx=A.dx;
		dy=A.dy;
	}
	Line operator=(const Line& B){
		F=B.F;
		S=B.S;
		dx=B.dx;
		dy=B.dy;
		return *(this);
	}
	bool operator <(const Line B)const{
		return (F==B.F)?S<B.S:comp().compPointY(F,B.F);
	}
	bool trivial_accept(Point Base, Point Top, int& outcode){
		int outcodef=F.TBRL(Base,Top);
		int outcodes=S.TBRL(Base,Top);
		if((outcodef | outcodes)==0){
			return true;
		}
		outcode=outcodef & outcodes;
		return false;
	}
	Point getInterY(int y){//mejorar
		double k=double(y-F.y)/vector().y;
		Color color;
		for (int i=0;i<3;i++){
			color.set(i,F.color.get(i)*(1-k)+S.color.get(i)*(k));
		}
		return Point(F.x+k*vector().x,y,F.z*(1-k)+S.z*k, color,F.light*(1-k)+S.light*(k));
	}
	Point getInterX(int x){//mejorar
		double k=double(x-F.x)/vector().x;
		Color color;
		for (int i=0;i<3;i++){
			color.set(i,F.color.get(i)*(1-k)+S.color.get(i)*(k));
		}
		return Point(x,F.y+k*vector().y,F.z*(1-k)+S.z*k,color,F.light*(1-k)+S.light*(k));
	}
};

struct CohenSutherland{
	Line L;
	Point Vector;
	CohenSutherland(){}
	CohenSutherland(Line _L){
		L=_L;
		Vector=L.vector();
	}
	bool clipping(Point Base, Point Top){
		int outcode;
		if(L.trivial_accept(Base,Top,outcode)){
			return true;
		} else if(outcode!=0){
			return false;
		}
		int outcodef=L.F.TBRL(Base,Top);
		int outcodes=L.S.TBRL(Base,Top);
		outcode=outcodef | outcodes;
		if(outcode&8){
			if(Vector.y==0){
				return false;
			}
			L.S=L.getInterY(Top.y-1);
		} else if(outcode&4){
			if(Vector.y==0){
				return false;
			}
			L.F=L.getInterY(Base.y);
		} else {
			if(L.S.x<L.F.x){
				std::swap(L.S,L.F);
			}
			Vector=L.vector();
			if(Vector.x==0){
				return false;
			}
			if(outcode&2){
				L.S=L.getInterX(Top.x-1);
			} else if(outcode&1){
				L.F=L.getInterX(Base.x);
			}
		}
		L=Line(L.F,L.S);
		Vector=L.vector();
		return clipping(Base,Top);
	}
};

struct SutherlandHodgeman{
	std::vector<Line> Ls;
	int n;
	SutherlandHodgeman(int _n,Line* _Ls){
		n=_n;
		for (int i=0;i<_n;i++){
			Ls.push_back(_Ls[i]);
		}
	}
	int clipping(Point Base,Point Top,Line*& npLines){
		clipping(Base,Top);
                npLines=new Line[Ls.size()];
		for (int i=0;i<Ls.size();i++){
			npLines[i]=Ls[i];
		}
                return Ls.size();
	}
	void clipping(Point Base,Point Top){
		Line ClippingLine;
		std::vector<Point> TBRL[4];
		std::vector<Line> nLs;
		Point inter;
		/*** top ***/
		for (int i=0;i<Ls.size();i++){
			if(Ls[i].F.y<Top.y && Ls[i].S.y>=Top.y){
				inter=Ls[i].getInterY(Top.y-1);
				Ls[i]=Line(Ls[i].F,inter);
				TBRL[0].push_back(inter);
			}
		}
		nLs.clear();
		for (int i=0;i<Ls.size();i++){
			/*CohenSutherland CS(Ls[i]);
			if(CS.clipping(Base,Top)){
				nLs.push_back(Ls[i]);
			}*/
			if(Ls[i].F.y>=Top.y && Ls[i].S.y>=Top.y){
			} else {
				nLs.push_back(Ls[i]);
			}
		}
		Ls=nLs;
		sort(TBRL[0].begin(),TBRL[0].end());
		for (int i=0;i<TBRL[0].size();i+=2){
			Ls.push_back(Line(TBRL[0][i],TBRL[0][i+1]));
		}
		/*** bottom ***/
		for (int i=0;i<Ls.size();i++){
			if(Ls[i].F.y<Base.y && Ls[i].S.y>=Base.y){
				inter=Ls[i].getInterY(Base.y);
				Ls[i]=Line(Ls[i].S,inter);
				TBRL[1].push_back(inter);
			}
		}
		nLs.clear();
		for (int i=0;i<Ls.size();i++){
			/*CohenSutherland CS(Ls[i]);
			if(CS.clipping(Base,Top)){
				nLs.push_back(Ls[i]);
			}*/
			if(Ls[i].F.y<Base.y && Ls[i].S.y<Base.y){
			} else {
				nLs.push_back(Ls[i]);
			}
		}
		Ls=nLs;
		sort(TBRL[1].begin(),TBRL[1].end());
		for (int i=0;i<TBRL[1].size();i+=2){
			Ls.push_back(Line(TBRL[1][i],TBRL[1][i+1]));
		}
		/*** right ***/
		for (int i=0;i<Ls.size();i++){
			if(std::min(Ls[i].F.x,Ls[i].S.x)<Top.x && std::max(Ls[i].F.x,Ls[i].S.x)>=Top.x){
				inter=Ls[i].getInterX(Top.x-1);
				Ls[i]=Line(std::min(Ls[i].F,Ls[i].S),inter);
				TBRL[2].push_back(inter);
			}
		}
		nLs.clear();
		for (int i=0;i<Ls.size();i++){
			/*CohenSutherland CS(Ls[i]);
			if(CS.clipping(Base,Top)){
				nLs.push_back(Ls[i]);
			}*/
			if(Ls[i].F.x>=Top.x && Ls[i].S.x>=Top.x){
			} else {
				nLs.push_back(Ls[i]);
			}
		}
		Ls=nLs;
		sort(TBRL[2].begin(),TBRL[2].end(),Line().myobject);
		for (int i=0;i<TBRL[2].size();i+=2){
			Ls.push_back(Line(TBRL[2][i],TBRL[2][i+1]));
		}
		/*** left ***/
		for (int i=0;i<Ls.size();i++){
			if(std::min(Ls[i].F.x,Ls[i].S.x)<Base.x && std::max(Ls[i].F.x,Ls[i].S.x)>=Base.x){
				inter=Ls[i].getInterX(Base.x);
				Ls[i]=Line(std::max(Ls[i].F,Ls[i].S),inter);
				TBRL[3].push_back(inter);
			}
		}
		nLs.clear();
		for (int i=0;i<Ls.size();i++){
			/*CohenSutherland CS(Ls[i]);
			if(CS.clipping(Base,Top)){
				nLs.push_back(Ls[i]);
			}*/
			if(Ls[i].F.x<Base.x && Ls[i].S.x<Base.x){
			} else {
				nLs.push_back(Ls[i]);
			}
		}
		Ls=nLs;
		sort(TBRL[3].begin(),TBRL[3].end(),Line().myobject);
		for (int i=0;i<TBRL[3].size();i+=2){
			Ls.push_back(Line(TBRL[3][i],TBRL[3][i+1]));
		}
		n=Ls.size();
	}
};

struct Bresenham{
	Point m_A,m_B;
	Point *points;
	int size;
	Bresenham(){
		points=NULL;
		size=0;
	}
	Bresenham(Line L){
		points=NULL;
		size=0;
		generatePoints(L);
	};
	void generatePoints(const Line& L,bool octant=0){
		Point A = L.F;
		Point B = L.S;
		points=NULL;
		int dx=L.dx;
		int dy=L.dy;
		if(B<A){
			std::swap(A,B);
			dx*=-1;
			dy*=-1;
		}
		if(dx<abs(dy)){
			std::swap(A.x,A.y);
			std::swap(B.x,B.y);
			generatePoints(Line(A,B,dy,dx),1);
			return;
		}
		int incY=1;
		if(dy<0){
			dy*=-1;
			incY=-1;
			B.y=A.y+dy;
		}
		int d=2*dy-dx;
		int incE=2*dy;
		int incNE=-2*dx;
		int pi=0;
		points= new Point[size=B.x-A.x+1];
		if(octant)
			points[pi++]=Point(A.y,A.x,0);
		else
			points[pi++]=A;
		while(pi<size){
			if(d>0){
				A.y+=incY;
				d+=incNE;
			}
			A.x++;
			d+=incE;
			if(octant)
				points[pi++]=Point(A.y,A.x,0);
			else
				points[pi++]=Point(A.x,A.y,0);;
		}
	}
};

struct n_AET{
	int x;
	int e;
	int dx;
	int dy;
	int xfrac;
	int inter_x,inter_y;
	Color colors[2];
	float lights[2];
	float depths[2];
	Color color;
	float light;
	float depth;
	n_AET(){}
	n_AET(int _e,int _x,int _dx,int _dy){
		inter_y=0;
		inter_x=0;
		x=_x;
		e=_e;
		dx=_dx;
		dy=_dy;
		xfrac=0;
	}
	n_AET(int _e,int _x,int _dx,int _dy,Color a,Color b, float lighta, float lightb, float deptha, float depthb){
		inter_y=0;
		inter_x=0;
		x=_x;
		e=_e;
		dx=_dx;
		dy=_dy;
		xfrac=0;
		setColors(a,b);
		lights[0]=lighta;
		lights[1]=lightb;
		depths[0]=deptha;
		depths[1]=depthb;
		light=lighta;
		depth=deptha;
	}
	void setColors(Color a, Color b){
		colors[0]=a;
		colors[1]=b;
		color=a;
	}
	void setColors(Color a, Color b,Color c){
		colors[0]=a;
		colors[1]=b;
		color=c;
	}
	n_AET(const n_AET& b){
		inter_y=b.inter_y;
		inter_x=b.inter_x;
		e=b.e;
		x=b.x;
		dx=b.dx;
		dy=b.dy;
		xfrac=b.xfrac;
		lights[0]=b.lights[0];
		lights[1]=b.lights[1];
		depths[0]=b.depths[0];
		depths[1]=b.depths[1];
		light=b.light;
		depth=b.depth;
		setColors(b.colors[0],b.colors[1],b.color);
	}
	bool operator <(const n_AET b)const{
		return x==b.x?e<b.e:x<b.x;
	}
	void updateColor(){//a*x+b*y / a+b=1
		int sizeH=dx*dx+dy*dy;
		int sizeh=inter_x*inter_x+inter_y*inter_y;
		float rel=(float)sizeh/sizeH;
		for (int i=0;i<3;i++){
			color.set(i,colors[0].get(i)*(1-rel)+colors[1].get(i)*rel);
		}
		light=lights[0]*(1-rel)+lights[1]*rel;
		depth=depths[0]*(1-rel)+depths[1]*rel;
	}
	n_AET& operator ++(){
		inter_y++;
		xfrac+=dx;
		while(abs(xfrac)>=dy){
			if(dx>0){
				xfrac-=dy;
				x++;
				inter_x++;
			} else {
				xfrac+=dy;
				x--;
				inter_x++;
			}
		}
		if(color.color)
			updateColor();
		return *this;
	}
};

struct ScanLine{
	std::set<n_AET> AET;
	std::set<n_AET>::iterator first,second;
	int m_n;
	Line *m_ET;
	vector<3> original_z;
	ScanLine(){
		m_n=0;
		m_ET=0;
	}
	ScanLine(int n, Line * ET){
		m_n=n;
		if(n>0){
			m_ET=new Line[n];
			for (int i=0;i<n;i++){
				m_ET[i]=ET[i];
			}
		}
	}
	void draw_points(std::vector<Point>& scan, bool interpolation=false){
		draw_points(m_n,m_ET,scan, interpolation);
	}
	void draw_points(int n, Line * ET,std::vector<Point>& scan, bool interpolation=false){
		if(n==0){
			return;
		}
		std::sort(ET, ET+n);
		AET.clear();
		int pos=0;
		for (int y=ET[0].F.y;y<ET[n-1].S.y;y++){
			while (pos<n && ET[pos].F.y==y){
				if(ET[pos].S.y>y){
					if(interpolation){//coloring
						AET.insert(n_AET(pos,ET[pos].F.x,ET[pos].dx,ET[pos].dy,ET[pos].F.color,ET[pos].S.color,ET[pos].F.light,ET[pos].S.light,ET[pos].F.z,ET[pos].S.z));
					} else {
						AET.insert(n_AET(pos,ET[pos].F.x,ET[pos].dx,ET[pos].dy));
					}
				}
				pos++;
			}
			std::set<n_AET> new_AET;
			first=AET.begin();
			if(first!=AET.end()){
				if(y+1!=ET[first->e].S.y){
					n_AET nxt(*first);
					++nxt;
					new_AET.insert(nxt);
				}
				second=AET.begin();
				second++;
				int x=first->x;;
				bool draw=true;
				while(second!=AET.end()){
					if(draw){
						//x=first->x;
						if(x==second->x){
							if(interpolation){
								Color color;
								for (int i=0;i<3;i++){
									color.set(i,(first->color).get(i)*first->light);
								}
								scan.push_back(Point(x,y,color));
							} else {
								scan.push_back(Point(x,y));
							}
						} else {
							//if(first->xfrac==0){
								if(interpolation){
									Color color;
									for (int i=0;i<3;i++){
										color.set(i,(first->color).get(i)*first->light);
									}
									scan.push_back(Point(x,y,first->depth,color));
								} else {
									scan.push_back(Point(x,y));
								}
							//}
							while(++x<second->x){
								if(interpolation){
									Color color;
									float rel=(float)(x-first->x)/(second->x-first->x);
									float ilight= first->light*(1-rel)+second->light*rel;
									for (int i=0;i<3;i++){
										color.set(i,((first->color).get(i)*(1-rel)+(second->color).get(i)*(rel))*ilight);
									}
									scan.push_back(Point(x,y,(first->depth)*(1-rel)+(second->depth)*rel,color));
								} else {	
									scan.push_back(Point(x,y));
								}
							}
							if(second->xfrac==0){
								if(interpolation){
									Color color;
									for (int i=0;i<3;i++){
										color.set(i,(second->color).get(i)*second->light);
									}
									scan.push_back(Point(x,y,(second->depth),color));
								} else {
									scan.push_back(Point(x,y));
								}
							}
						}
					}
					first=second;
					if(y+1!=ET[first->e].S.y){
						n_AET nxt=*first;
						++nxt;
						new_AET.insert(nxt);
					}
					second++;
					draw=!draw;
				}
			}
			AET=new_AET;
		}
	}
};

struct Transformations{
	vector<3> reflection(vector<3> normal,vector<3>light){
		float scal=2*(normal*light);
		normal=normal*scal;
		return light-normal;
	}
	matrix<4,4> MatrixPerspective(us Width, us Height,
		us zNear, us zFar, us FOV, double dist){
		us ar= Width / Height;
		us zRange = zNear - zFar;
		double tanHalfFOV=1/tanf((FOV/2.0)*3.1416/180);
		matrix<4,4> mPers;
		mPers[0][0] = 1.0f / (tanHalfFOV * ar);
		mPers[0][1] = 0.0f;
		mPers[0][2] = 0.0f;
		mPers[0][3] = 0.0f;
		mPers[1][0] = 0.0f;
		mPers[1][1] = 1.0f/tanHalfFOV;
		mPers[1][2] = 0.0f;
		mPers[1][3] = 0.0f;
		mPers[2][0] = 0.0f;
		mPers[2][1] = 0.0f;
		mPers[2][2] = (-zNear - zFar) / zRange;
		mPers[2][3] = 2.0f * zFar * zNear / zRange;
		mPers[3][0] = 0.0f;
		mPers[3][1] = 0.0f;
		mPers[3][2] = 1.0f;
		mPers[3][3] = 0.0f;
		return mPers;	
	}
};
