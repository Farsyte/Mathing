#pragma once

#include "exception.hh"
#include "str.hh"
#include "normsq.hh"

/** \file
 * \brief Fixed Size Vector
 *
 */
template<int N, typename T>
class Vec {
  public:

    /** Storage for the elements of the vector.
     *
     * As long as all data elements are public, and there are
     * no constructors, Vec can be initialized just as if it
     * were a struct.
     */
    T        _[N];

    /* ================================================================
     *                       Subscripting
     * ============================================================= */

    /** Subscript (zero-based) for const Vec objects.
     *
     * After checking that the subscript is within range, returns a
     * const reference to the element of the const Vec.
     */
    T const &operator()(int i) const
    {
        IndexRange::check(i, 0, N - 1);
        return _[i];
    }

    /** Subscript (zero-based) for mutable Vec objects.
     *
     * After checking that the subscript is within range, returns a
     * mutable reference to the element of the mutable Vec.
     */
    T &operator()(int i)
    {
        IndexRange::check(i, 0, N - 1);
        return _[i];
    }

    /* ================================================================
     *                   Comparison Operators
     * ============================================================= */

    /** True if all correspinding elements compare equal.
     */
    template<typename U>
    bool operator==(Vec<N, U> const &that) const
    {
        for (int i = 0; i < N; ++i)
            if (_[i] != that._[i])
                return false;
        return true;
    }

    /** False if all correspinding elements compare equal.
     */
    template<typename U>
    bool operator!=(Vec<N, U> const &that) const
    {
        for (int i = 0; i < N; ++i)
            if (_[i] != that._[i])
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
    Vec &operator=(Vec<N, U> const &that)
    {
        for (int i = 0; i < N; ++i)
            _[i] = that._[i];
        return *this;
    }

    /** Add Assignment.
     *
     * Increment each element by the corresponding element
     * of the left hand side.
     */
    template<typename U>
    Vec &operator+=(Vec<N, U> const &that)
    {
        for (int i = 0; i < N; ++i)
            _[i] += that._[i];
        return *this;
    }

    /** Subtract Assignment.
     *
     * Decrement each element by the corresponding element
     * of the left hand side.
     */
    template<typename U>
    Vec &operator-=(Vec<N, U> const &that)
    {
        for (int i = 0; i < N; ++i)
            _[i] -= that._[i];
        return *this;
    }

    /** Multiply Assignment.
     *
     * Scale each element up by the scalar value on the
     * right hand side.
     */
    template<typename U>
    Vec &operator*=(U const &that)
    {
        for (int i = 0; i < N; ++i)
            _[i] *= that;
        return *this;
    }

    /** Divide Assignment.
     *
     * Scale each element down by the scalar value on the
     * right hand side.
     */
    template<typename U>
    Vec &operator/=(U const &that)
    {
        for (int i = 0; i < N; ++i)
            _[i] /= that;
        return *this;
    }

    /* ================================================================
     *                     Unary Operators
     * ============================================================= */

    /** Negation.
     *
     * Return a Vec where each element is the negative of each
     * corresponding element of this Vec.
     */
    Vec<N, T> operator-() const
    {
        Vec<N, T> result;
        for (int i = 0; i < N; ++i)
            result._[i] = -_[i];
        return result;
    }

    /* ================================================================
     *                     Binary Operators
     * ============================================================= */

    /** Addition.
     *
     * Return a Vec with each element set to the sum of the
     * corresponding elements of this Vec and the Vec value
     * on the right hand side.
     */
    template<typename U>
    Vec operator+(Vec<N, U> const &that) const
    {
        Vec result;
        for (int i = 0; i < N; ++i)
            result._[i] = _[i] + that._[i];
        return result;
    }

    /** Subtraction.
     *
     * Return a Vec with each element set to the difference of the
     * corresponding elements of this Vec and the Vec value on the
     * right hand side.
     */
    template<typename U>
    Vec operator-(Vec<N, U> const &that) const
    {
        Vec result;
        for (int i = 0; i < N; ++i)
            result._[i] = _[i] - that._[i];
        return result;
    }

    /** Scalar Multiply.
     *
     * Return a Vec with each element set to the product of the
     * corresponding element of this vector and the scalar value
     * on the right hand side.
     */
    template<typename U>
    Vec operator*(U const &that) const
    {
        Vec result;
        for (int i = 0; i < N; ++i)
            result._[i] = _[i] * that;
        return result;
    }

    /** Scalar Divide
     *
     * Return a Vec with each element set to the corresponding element
     * of this vector divided by the scalar value on the right hand
     * side.
     */
    template<typename U>
    Vec operator/(U const &that) const
    {
        Vec result;
        for (int i = 0; i < N; ++i)
            result._[i] = _[i] / that;
        return result;
    }

    /** Vector Product.
     *
     * Return the dot product of the two vectors.
     */
    T operator*(Vec const &that) const
    {
        T result = T();
        for (int i = 0; i < N; ++i)
            result += _[i] * that._[i];
        return result;
    }

    /* ================================================================
     *                     special patterns
     * ============================================================= */

    void fill(T const &v = T())
    {
        for (int i = 0; i < N; ++i)
            _[i] = v;
    }
};

/** Vector Cross Product.
 */
template<typename T>
Vec<3, T> cross(Vec<3, T> const &lhs, Vec<3, T> const &rhs)
{
    Vec<3, T> result;
    result._[0] = lhs._[1] * rhs._[2] - lhs._[2] * rhs._[1];
    result._[1] = lhs._[2] * rhs._[0] - lhs._[0] * rhs._[2];
    result._[2] = lhs._[0] * rhs._[1] - lhs._[1] * rhs._[0];
    return result;
}

/** Forvec Vec value into a String.
 */
template<int N, typename T>
std::string str(                                            //
        const Vec<N, T> &m,                                 //
        int              w = 8, int p = 3,                  //
        std::ios_base::fmtflags f = std::ios_base::fixed)   //
{
    std::ostringstream ss;
    ss << "{ ";
    for (int i = 0; i < N; ++i) {
        if (i)
            ss << ", ";
        ss << str(m(i), w, p, f);
    }
    ss << " }";

    return ss.str();
}

/** Output Vec onto an output stream.
 */
template<typename S, int N, typename T>
S &operator<<(S &s, Vec<N, T> const &m)
{
    return s << str(m, s);
}

/** Compute the squared norm of the vector.
 */
template<int N, typename T>
T normsq(Vec<N, T> const &m)
{
    T result = T();
    for (int i = 0; i < N; ++i)
        result += normsq(m._[i]);
    return result;
}

/** Vector Left-Multiply by Scalar.
 *
 * Return a new Vec of the same type as the right hand side,
 * where the value is the product of the left hand side and
 * the corresponding element of the right hand side.
 */
template<int N, typename T, typename U>
Vec<N, T> operator*(U const &lhs, Vec<N, T> const &rhs)
{
    Vec<N, T> result;
    for (int i = 0; i < N; ++i)
        result._[i] = lhs * rhs._[i];
    return result;
}
