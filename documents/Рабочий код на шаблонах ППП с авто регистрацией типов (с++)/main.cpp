#include <QCoreApplication>

#include <cmath>
#include <iostream>

using namespace std;

//==============================================================================
struct Rectangle
{
    float a;
    float b;
};

void Out(Rectangle& t)
{
    cout <<"Rectangle"<<endl;
}

double Square(Rectangle *item)
{
    auto s = item->a * item->b;
    return s;
}

double Perimeter(Rectangle *item)
{
    auto perimeter = 2*(item->a * item->b);
    return perimeter;
}

//==============================================================================
struct Triangle
{
    float a;
    float b;
    float c;
};

void Out(Triangle& t)
{
    cout <<"Triangle"<<endl;
}

double Square(Triangle* item)
{
    auto p = static_cast<float>(item->a + item->b + item->c)/2.0;
    auto s = sqrt(p*(p-item->a)*(p-item->b)*(p-item->c));
    return s;
}

double Perimeter(Triangle* item)
{
    auto perimeter = item->a + item->b + item->c;
    return perimeter;
}

//==============================================================================
struct Circl
{
    int r;
};

void Out(Circl& t)
{
    cout <<"Circl"<<endl;
}

double Square(Circl *item)
{
    auto s = 3.14*item->r;
    return s;
}

double Perimeter(Circl *item)
{
    auto perimeter = 2*3.14*item->r;
    return perimeter;
}



//=============================================================================
namespace  {
    vector<string> regType{};
}

template <typename T>
int GetSpecNumAndIncrement(T t)
{
    string type = typeid(t).name();
    int specNumber = 0;
    auto findItem  = std::find(regType.begin(),regType.end(),type);

    if(findItem == regType.end())
    {
       regType.push_back(type);
       specNumber = static_cast<int>(regType.size()-1);
    }
    else
    {
        specNumber = std::distance(regType.begin(), findItem);
    }

    return specNumber;
}


//==============================================================================
template<typename T>
struct Union {
    Union(int mark):mark(mark){}
    int GetMark(){return mark;}
    T* GetValue() {return &value;}
    int mark;
    T value;
};

struct Figure {
    virtual void Out() = 0;
    virtual void GetName() = 0;
    static int size;

    int index;
};


namespace  {
    double (*Square_figure[])(Figure* f){nullptr};
    double (*Perimetr_figure[])(Figure* f){nullptr};
}


template<typename F, typename T>
double SquareFigure(F* f) {
    return Square(dynamic_cast<T*>(f)->GetValue());
}

template<typename F, typename T>
double PerimetrFigure(F* f) {
    return Perimeter(dynamic_cast<T*>(f)->GetValue());
}


//Класс, регистрирующий специализацию прямоугольника
struct FigureRectangle: Figure, Union<Rectangle> {
    FigureRectangle():Union(getRank()){
        index = mark;
        Square_figure[mark] = &SquareFigure<Figure, FigureRectangle>;
        Perimetr_figure[mark]= &PerimetrFigure<Figure, FigureRectangle>;

    }
    int getRank() {GetSpecNumAndIncrement(this);}

    virtual void Out() {cout << "Registration of FigureRectangle, mark = " << mark << endl;}
    virtual void GetName(){cout << "Rectangle : ";}
};



//Класс, регистрирующий специализацию треугольника
struct FigureTriangle: Figure, Union<Triangle> {
    FigureTriangle():Union(getRank()){
         index = mark;
         Square_figure[mark] = &SquareFigure<Figure, FigureTriangle>;
         Perimetr_figure[mark]= &PerimetrFigure<Figure, FigureTriangle>;
    }
    int getRank() {GetSpecNumAndIncrement(this);}

    virtual void Out() {cout << "Registration of FigureTriangle, mark = " << mark << endl;}
    virtual void GetName(){cout << "Triangle : ";}
};


//Класс, регистрирующий специализацию треугольника
struct FigureCircl: Figure, Union<Circl> {
    FigureCircl():Union(getRank()){
         index = mark;
         Square_figure[mark] = &SquareFigure<Figure, FigureCircl>;
         Perimetr_figure[mark]= &PerimetrFigure<Figure, FigureCircl>;


    }

    int getRank() {GetSpecNumAndIncrement(this);}

    virtual void Out() {cout << "Registration of FigureCircl, mark = " << mark << endl;}
    virtual void GetName(){cout << "Circl : ";}
};


//=============================================================================
double Square(Figure* f)
{
    return Square_figure[f->index](f);
}

double Perimeter(Figure* f)
{
    return Perimetr_figure[f->index](f);
}

//=============================================================================
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    FigureRectangle rectangle;
    rectangle.value.a = 2;
    rectangle.value.b = 5;
    Figure* pf_rec = &rectangle;
    pf_rec->Out();
    pf_rec->GetName();
    cout << "Square = "<< Square(pf_rec) << endl;
    pf_rec->GetName();
    cout << "Perimetr = "<< Perimeter(pf_rec) << endl;

    FigureTriangle*  triangle = new FigureTriangle;
    triangle->value.a = 2;
    triangle->value.b = 2;
    triangle->value.c = 2;
    Figure* pf_triangle = triangle;
    pf_triangle->Out();
    pf_triangle->GetName();
    cout << "Square = "<< Square(pf_triangle) << endl;
    pf_triangle->GetName();
    cout << "Perimetr = "<< Perimeter(pf_triangle) << endl;

    FigureRectangle  rectangle2;
    rectangle2.value.a = 2;
    rectangle2.value.b = 5;
    Figure* pf_rectangle2 = &rectangle2;
    pf_rectangle2->Out();
    pf_rectangle2->GetName();
    cout  << "Square = "<< Square(pf_rectangle2) << endl;
    pf_triangle->GetName();
    cout  << "Perimetr = "<< Perimeter(pf_rectangle2) << endl;

    FigureCircl  circl;
    circl.value.r = 2;
    Figure* pf_circl = &circl;
    pf_circl->Out();
    pf_circl->GetName();
    cout << "Square = "<< Square(pf_circl) << endl;
    pf_circl->GetName();
    cout << "Perimetr = "<< Perimeter(pf_circl) << endl;

    FigureCircl*  circl2 = new FigureCircl;
    circl2->value.r = 2;
    Figure* pf_circl2 = circl2;
    pf_circl->Out();
    pf_circl->GetName();
    cout << "Square = "<< Square(pf_circl2) << endl;
    pf_circl->GetName();
    cout << "Perimetr = "<< Perimeter(pf_circl2) << endl;


    cout << "================="<<endl;
    cout << "All Registered Types ("<<regType.size()<<"):"<<endl<<endl;
    for(int i=0; i < regType.size();i++)
    {
        cout <<"[ "<<i<<" ] "<< regType[i]<<endl;
    }
    cout << "================="<<endl;


    return a.exec();
}
