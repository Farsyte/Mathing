#pragma once

#include "exception.hh"
#include "str.hh"
#include "normsq.hh"

/** \file
 * \brief Fixed Size Matrix
 *
 * TODO: doxygen comments for everyone.
 *
 *
 * TLDR: for dimensions R,I,C and type T, we have:
 *
 * 
 */

#if 0
template<int R, int I, int C, typename T>
void demo()
{
    // constructors with initializers
    Mat<R,I,T> ml = {};
    Mat<I,C,T> mr = {};
    Mat<R,C,T> x = {};
    Mat<R,C,T> y = {};
    Mat<R,C,T> z = {};
    Mat<1,C,T> rv = {};
    Mat<R,1,T> cv = {};

    x = ml * mr;
}
#endif

template<int R, int C, typename T>
class Mat {

  public:

    int      nr() const { return R; }
    int      nc() const { return C; }

    T const &checked(int r, int c) const
    {
        IndexRange::check(r, 0, R - 1);
        IndexRange::check(c, 0, C - 1);
        return unchecked(r, c);
    }

    T &checked(int r, int c)
    {
        IndexRange::check(r, 0, R - 1);
        IndexRange::check(c, 0, C - 1);
        return unchecked(r, c);
    }

    T const &checked(int i) const
    {
        if (R == 1)
            return checked(0, i);
        if (C == 1)
            return checked(i, 0);
        IndexRange::check(i, 0, R * C - 1);
        return unchecked(i / C, i % C);
    }

    T &checked(int i)
    {
        if (R == 1)
            return checked(0, i);
        if (C == 1)
            return checked(i, 0);
        IndexRange::check(i, 0, R * C - 1);
        return unchecked(i / C, i % C);
    }

    template<typename U>
    void copyout(U const that[R][C]) const
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                that[r][c] = unchecked(r, c);
    }

    template<typename U>
    void copyout(U const that[R * C]) const
    {
        int i = 0;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                that[i++] = unchecked(r, c);
    }

    template<typename U>
    void diag(U const &d)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = (r == c) ? d : U();
    }

    template<typename U, typename V>
    void diag(U const &d, V const &o)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = (r == c) ? d : o;
    }

    void fill()
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = T();
    }

    template<typename U>
    void fill(U const &v)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = v;
    }

    template<typename U>
    void copy(Mat<R, C, U> const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = that.unchecked(r, c);
    }

    template<typename U>
    void copy(U const that[R * C])
    {
        int i = 0;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = that[i++];
    }

    template<typename U>
    void copy(U const that[R][C])
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = that[r][c];
    }

    /* ================================================================
     *                        Operators                                
     * ============================================================= */

    T const &operator()(int r, int c) const { return checked(r, c); }
    T       &operator()(int r, int c) { return checked(r, c); }
    T const &operator()(int i) const { return checked(i); }
    T       &operator()(int i) { return checked(i); }

    template<typename U>
    Mat &operator=(Mat<R, C, U> const &that)
    {
        for (int i = 0; i < R * C; ++i)
            unchecked(i) = that.unchecked(i);
        return *this;
    }

    template<typename U>
    Mat &operator=(U const that[R * C])
    {
        for (int i = 0; i < R * C; ++i)
            unchecked(i) = that[i];
        return *this;
    }

    template<typename U>
    Mat &operator=(U const that[R][C])
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) = that[r][c];
        return *this;
    }

    template<typename U>
    bool operator==(Mat<R, C, U> const &that) const
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                if (unchecked(r, c) != that.unchecked(r, c))
                    return false;
        return true;
    }

    template<typename U>
    bool operator!=(Mat<R, C, U> const &that) const
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                if (unchecked(r, c) != that.unchecked(r, c))
                    return true;
        return false;
    }

    template<typename U>
    Mat &operator+=(Mat<R, C, U> const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) += that.unchecked(r, c);
        return *this;
    }

    template<typename U>
    Mat operator+(Mat<R, C, U> const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result.unchecked(r, c) =
                        unchecked(r, c) + that.unchecked(r, c);
        return result;
    }

    template<typename U>
    Mat &operator-=(Mat<R, C, U> const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) -= that.unchecked(r, c);
        return *this;
    }

    template<typename U>
    Mat operator-(Mat<R, C, U> const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result.unchecked(r, c) =
                        unchecked(r, c) - that.unchecked(r, c);
        return result;
    }

    template<typename U>
    Mat &operator*=(U const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) *= that;
        return *this;
    }

    template<typename U>
    Mat operator*(U const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result.unchecked(r, c) = unchecked(r, c) * that;
        return result;
    }

    template<typename U>
    Mat &operator/=(U const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                unchecked(r, c) /= that;
        return *this;
    }

    template<typename U>
    Mat operator/(U const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result.unchecked(r, c) = unchecked(r, c) / that;
        return result;
    }

    template<int N>
    Mat<R, N, T> operator*(Mat<C, N, T> const &that) const
    {
        Mat<R, N, T> result;
        for (int r = 0; r < R; ++r)
            for (int n = 0; n < N; ++n) {
                T sum = unchecked(r, 0) * that.unchecked(0, n);
                for (int c = 1; c < C; ++c)
                    sum += unchecked(r, c) * that.unchecked(c, n);
                result.unchecked(r, n) = sum;
            }

        return result;
    }

    Mat<C, R, T> operator~() const
    {
        Mat<C, R, T> result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result.unchecked(c, r) = unchecked(r, c);
        return result;
    }

    T const &unchecked(int r, int c) const { return _[r][c]; }
    T       &unchecked(int r, int c) { return _[r][c]; }

    T const &unchecked(int i) const
    {
        if (R == 1)
            return _[0][i];
        if (C == 1)
            return _[i][0];
        return _[i / C][i % C];
    }

    T &unchecked(int i)
    {
        if (R == 1)
            return _[0][i];
        if (C == 1)
            return _[i][0];
        return _[i / C][i % C];
    }

    T _[R][C];
};

template<int R, int C, typename T>
std::string str(                                            //
        const Mat<R, C, T> &m,                              //
        int                 w = 8, int p = 3,               //
        std::ios_base::fmtflags f = std::ios_base::fixed)   //
{
    std::ostringstream ss;
    ss << "\n{";
    for (int r = 0; r < R; ++r) {
        if (r)
            ss << ",\n ";
        ss << "{ ";
        for (int c = 0; c < C; ++c) {
            if (c)
                ss << ", ";
            ss << str(m.unchecked(r, c), w, p, f);
        }
        ss << " }";
    }
    ss << "}";

    return ss.str();
}

template<typename S, int R, int C, typename T>
S &operator<<(S &s, Mat<R, C, T> const &m)
{
    return s << str(m, s);
}

template<int R, int C, typename T>
T normsq(Mat<R, C, T> const &m)
{
    T result = T();
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            result += normsq(m.unchecked(r, c));
    return result;
}
