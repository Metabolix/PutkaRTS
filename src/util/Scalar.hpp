#ifndef PUTKARTS_Scalar_HPP
#define PUTKARTS_Scalar_HPP

#include <cmath>
#include <limits>

#include "Math.hpp"
#include "SIUnit.hpp"

template <typename U1 = SIUnit::Unit<> >
struct Scalar {
	typedef double T;
	typedef U1 Unit;
	typedef Scalar<U1> Type;

	T value;

	// Default constructor; init to zero
	Scalar() : value(0) {}

	// Construct from a value
	Scalar(T _val) : value(_val) {}

	// Copy constructor
	Scalar(Type const& t) : value(t.value) {}

	// Accessing the raw value
	double getDouble() const {
		return value;
	}

	// Assignment without type checks
	template <typename U2>
	void unsafeAssign(Scalar<U2> const& t) {
		value = t.value;
	}

	// Conversion without type checks
	template <typename U2>
	Scalar<U2> unsafeConvert() const {
		return Scalar<U2>(value);
	}

	// Special values
	static Type zero() { return Type(0); }
	static Type nan() { return Type(std::numeric_limits<T>::quiet_NaN()); }
	static Type infP() { return Type(std::numeric_limits<T>::infinity()); }
	static Type infN() { return Type(-std::numeric_limits<T>::infinity()); }
	static Type inf() { return infP(); }
	static Type inf(Type dir) {
		return dir.value < 0 ? infN() : infP();
	}
	static Type pi() {
		return (T) Math::pi;
	}

	// Checks for special cases
	bool isNaN() const {
		//return isnan(value);
		return value != value;
	}
	bool isInf() const {
		return value == infP().value || value == infN().value;
	}
	bool isZero() const {
		return value == 0;
	}
	bool isPositive() const {
		return value > 0;
	}
	bool isNegative() const {
		return value < 0;
	}

	// Strip unit
	Scalar<> strip() const {
		return value;
	}

	// Opposite value
	Type operator - () const {
		return -value;
	}

	// Allow usage in conditions
	typedef bool (Type::*BoolDummyType)();
	operator BoolDummyType () const {
		return value ? (BoolDummyType) &Type::isZero : 0;
	}
	bool operator ! () const { return !value; }

	// Comparisons
	bool operator < (Type const& t) const { return value < t.value; }
	bool operator > (Type const& t) const { return value > t.value; }
	bool operator <= (Type const& t) const { return value <= t.value; }
	bool operator >= (Type const& t) const { return value >= t.value; }
	bool operator == (Type const& t) const { return value == t.value; }
	bool operator != (Type const& t) const { return value != t.value; }

	// Addition
	Type operator + (Type const& t) const {
		return value + t.value;
	}

	// Subtraction
	Type operator - (Type const& t) const {
		return value - t.value;
	}

	// Multiplication
	template <typename U2>
	Scalar<SIUnitAdd(U1, U2)> operator * (Scalar<U2> const& t) const {
		return value * t.value;
	}

	// Division
	template <typename U2>
	Scalar<SIUnitSub(U1, U2)> operator / (Scalar<U2> const& t) const {
		return value / t.value;
	}

	// Assignment shorthands
	Type const& operator += (Type const& t) {
		*this = *this + t;
		return *this;
	}
	Type const& operator -= (Type const& t) {
		*this = *this - t;
		return *this;
	}
	Type const& operator /= (Scalar<> const& t) {
		*this = *this / t;
		return *this;
	}
	Type const& operator *= (Scalar<> const& t) {
		*this = *this * t;
		return *this;
	}

	// Square
	Scalar<SIUnitAdd(U1, U1)> pow2() const {
		return (*this) * (*this);
	}

	// Absolute value
	Type abs() const {
		return Type(value < 0 ? -value : value);
	}

	// Modulo
	Type mod(Type divisor) const {
		return Type(fmod(value, divisor.value));
	}
};

// Square root; the template magic requires a helper.
template <typename U1>
inline Scalar<U1> _sqrtHelper(Scalar<SIUnitMul(U1, 2)> const& x) {
	return std::sqrt(x.value);
}
template <typename U1>
inline Scalar<SIUnitDiv(U1, 2)> sqrt(Scalar<U1> const& x) {
	return _sqrtHelper<SIUnitDiv(U1, 2)>(x);
}

// Power
inline Scalar<> pow(Scalar<> const& x, Scalar<> const& y) {
	return std::pow(x.value, y.value);
}

// Exponent function
inline Scalar<> exp(Scalar<> const& x) {
	return std::exp(x.value);
}

// Logarithm
inline Scalar<> log(Scalar<> const& x) {
	return std::log(x.value);
}

// Trigonometry
inline Scalar<> cos(Scalar<> const& a) {
	return std::cos(a.value);
}
inline Scalar<> sin(Scalar<> const& a) {
	return std::sin(a.value);
}
inline Scalar<> tan(Scalar<> const& a) {
	return std::tan(a.value);
}
template <typename U1>
inline Scalar<> atan2(Scalar<U1> const& y, Scalar<U1> const& x) {
	return std::atan2(y.value, x.value);
}

#endif
