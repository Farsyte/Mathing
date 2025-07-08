#pragma once

#include "exception.hh"
#include "str.hh"
#include "normsq.hh"

/** \file
 * \brief Fixed Size Matrix
 *
 */
template<int R, int C, typename T>
class Mat {
  public:

    /** Storage for the elements of the array.
     *
     * As long as all data elements are public, and there are
     * no constructors, Mat can be initialized just as if it
     * were a struct.
     */
    T        _[R][C];

    /* ================================================================
     *                       Subscripting
     * ============================================================= */

    /** Double Subscript (zero-based) for const Mat objects.
     *
     * After checking that the subscripts are within range, returns a
     * const reference to the element of the const Mat.
     */
    T const &operator()(int r, int c) const
    {
        IndexRange::check(r, 0, R - 1);
        IndexRange::check(c, 0, C - 1);
        return _[r][c];
    }

    /** Double Subscript (zero-based) for mutable Mat objects.
     *
     * After checking that the subscripts are within range, returns a
     * mutable reference to the element of the mutable Mat.
     */
    T &operator()(int r, int c)
    {
        IndexRange::check(r, 0, R - 1);
        IndexRange::check(c, 0, C - 1);
        return _[r][c];
    }

    /** Single Subscript (zero-based) for const Mat objects.
     *
     * Intended for use with matrices with one row or one column
     * allowing code to treat them as column or row vectors, but
     * otherwise, allows sequential access to all elements.
     *
     * After checking that the subscript is within range, returns a
     * const reference to the element of the const Mat.
     */
    T const &operator()(int i) const
    {
        if (R == 1) {
            IndexRange::check(i, 0, C - 1);
            return _[0][i];
        }
        if (C == 1) {
            IndexRange::check(i, 0, R - 1);
            return _[i][0];
        }
        IndexRange::check(i, 0, R * C - 1);
        return _[i / C][i % C];
    }

    /** Single Subscript (zero-based) for mutable Mat objects.
     *
     * Intended for use with matrices with one row or one column
     * allowing code to treat them as column or row vectors, but
     * otherwise, allows sequential access to all elements.
     *
     * After checking that the subscript is within range, returns a
     * mutable reference to the element of the mutable Mat.
     */
    T &operator()(int i)
    {
        if (R == 1) {
            IndexRange::check(i, 0, C - 1);
            return _[0][i];
        }
        if (C == 1) {
            IndexRange::check(i, 0, R - 1);
            return _[i][0];
        }
        IndexRange::check(i, 0, R * C - 1);
        return _[i / C][i % C];
    }

    /* ================================================================
     *                   Comparison Operators
     * ============================================================= */

    /** True if all correspinding elements compare equal.
     */
    template<typename U>
    bool operator==(Mat<R, C, U> const &that) const
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                if (_[r][c] != that._[r][c])
                    return false;
        return true;
    }

    /** False if all correspinding elements compare equal.
     */
    template<typename U>
    bool operator!=(Mat<R, C, U> const &that) const
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                if (_[r][c] != that._[r][c])
                    return true;
        return false;
    }

    /* ================================================================
     *                   Assignment Operators
     * ============================================================= */

    /** Assignment.
     *
     * Copy each element of the right hand side to the left.
     */
    template<typename U>
    Mat &operator=(Mat<R, C, U> const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] = that._[r][c];
        return *this;
    }

    /** Add Assignment.
     *
     * Increment each element by the corresponding element
     * of the left hand side.
     */
    template<typename U>
    Mat &operator+=(Mat<R, C, U> const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] += that._[r][c];
        return *this;
    }

    /** Subtract Assignment.
     *
     * Decrement each element by the corresponding element
     * of the left hand side.
     */
    template<typename U>
    Mat &operator-=(Mat<R, C, U> const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] -= that._[r][c];
        return *this;
    }

    /** Multiply Assignment.
     *
     * Scale each element up by the scalar value on the
     * right hand side.
     */
    template<typename U>
    Mat &operator*=(U const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] *= that;
        return *this;
    }

    /** Divide Assignment.
     *
     * Scale each element down by the scalar value on the
     * right hand side.
     */
    template<typename U>
    Mat &operator/=(U const &that)
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] /= that;
        return *this;
    }

    /* ================================================================
     *                     Unary Operators
     * ============================================================= */

    /** Negation.
     *
     * Return a Mat where each element is the negative of each
     * corresponding element of this Mat.
     */
    Mat<R, C, T> operator-() const
    {
        Mat<R, C, T> result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result._[r][c] = -_[r][c];
        return result;
    }

    /** Transpose.
     *
     * Return a Mat where each row of the result is the corresponding
     * column of this Mat.
     */
    Mat<C, R, T> operator~() const
    {
        Mat<C, R, T> result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result._[c][r] = _[r][c];
        return result;
    }

    /* ================================================================
     *                     Binary Operators
     * ============================================================= */

    /** Addition.
     *
     * Return a Mat with each element set to the sum of the
     * corresponding elements of this Mat and the Mat value
     * on the right hand side.
     */
    template<typename U>
    Mat operator+(Mat<R, C, U> const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result._[r][c] = _[r][c] + that._[r][c];
        return result;
    }

    /** Subtraction.
     *
     * Return a Mat with each element set to the difference of the
     * corresponding elements of this Mat and the Mat value on the
     * right hand side.
     */
    template<typename U>
    Mat operator-(Mat<R, C, U> const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result._[r][c] = _[r][c] - that._[r][c];
        return result;
    }

    /** Scalar Multiply.
     *
     * Return a Mat with each element set to the product of the
     * corresponding element of this matrix and the scalar value
     * on the right hand side.
     */
    template<typename U>
    Mat operator*(U const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result._[r][c] = _[r][c] * that;
        return result;
    }

    /** Scalar Divide
     *
     * Return a Mat with each element set to the corresponding element
     * of this matrix divided by the scalar value on the right hand
     * side.
     */
    template<typename U>
    Mat operator/(U const &that) const
    {
        Mat result;
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                result._[r][c] = _[r][c] / that;
        return result;
    }

    /** Matrix Product.
     *
     * Return a Mat that is the result of performing a matrix multiply
     * of this matrix and the matrix on the right hand side.
     */
    template<int K, typename U>
    Mat<R, K, T> operator*(Mat<C, K, U> const &that) const
    {
        Mat<R, K, T> result;
        for (int r = 0; r < R; r++) {
            for (int k = 0; k < K; k++) {
                T sum = T();
                for (int c = 0; c < C; c++) {
                    sum += _[r][c] * that._[c][k];
                }
                result._[r][k] = sum;
            }
        }
        return result;
    }

    /* ================================================================
     *                     special patterns
     * ============================================================= */

    void diag(T const &d = T(), T const &o = T())
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] = (r == c) ? d : o;
    }

    void fill(T const &v = T())
    {
        for (int r = 0; r < R; ++r)
            for (int c = 0; c < C; ++c)
                _[r][c] = v;
    }
};

/** Format Mat value into a String.
 */
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
            ss << str(m(r, c), w, p, f);
        }
        ss << " }";
    }
    ss << "}";

    return ss.str();
}

/** Output Mat onto an output stream.
 */
template<typename S, int R, int C, typename T>
S &operator<<(S &s, Mat<R, C, T> const &m)
{
    return s << str(m, s);
}

/** Compute the squared norm of the matrix.
 */
template<int R, int C, typename T>
T normsq(Mat<R, C, T> const &m)
{
    T result = T();
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            result += normsq(m._[r][c]);
    return result;
}

/** Matrix Multiply.
 *
 * Return a new Mat of the same type as the right hand side,
 * where the value is the product of the left hand side and
 * the corresponding element of the right hand side.
 */
template<int R, int C, typename T, typename U>
Mat<R, C, T> operator*(U const &lhs, Mat<R, C, T> const &rhs)
{
    Mat<R, C, T> result;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            result(r, c) = lhs * rhs(r, c);
    return result;
}
