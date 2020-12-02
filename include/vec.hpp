#ifndef VEC_H
#define VEC_H

class vec3d {
    public:
        double x;
        double y;
        double z;

        //size of vector
        const static int n = 3;

        //default constructor
        vec3d();
        //constructor: initializes to constant array with double e
        vec3d(const double e);
        //constructor: intializes to given values
        vec3d(const double x, const double y, const double z);
        //copy constructor
        vec3d(const vec3d& v);
        //destructor
        ~vec3d();

        // printer method
		friend std::ostream& operator<<(std::ostream& os, const vec3d& v);

        // //functions
        // double norm() const;
        // void normalize();

        //copy assignment to vector
        void operator=(const vec3d& v);
        //copy assignment to array
        void operator=(const double v[3]);

        //vector equality
        bool operator==(const vec3d& v) const;
        //vector inequality
        bool operator!=(const vec3d& v) const;

        //vector addition
        vec3d operator+(const vec3d& v) const;
        //in place vector addition
        void operator+=(const vec3d& v);

        //vector subtraction
        vec3d operator-(const vec3d& v) const;
        //in place vector subtraction
        void operator-=(const vec3d& v);

        //vector scalar product
        vec3d operator*(const double s) const;
        friend vec3d operator*(double s, const vec3d& v);
        //vector scalar division
        vec3d operator/(const double s) const;

        //vector inner product
        double operator*(const vec3d& v) const;
        //vector outer product
        vec3d operator&(const vec3d& v) const;
        //in place vector outer product
        void operator&=(const vec3d& v);
};

#endif