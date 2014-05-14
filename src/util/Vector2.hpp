#ifndef PUTKARTS_Vector2_HPP
#define PUTKARTS_Vector2_HPP

#include <cmath>

#include "SIUnit.hpp"
#include "Scalar.hpp"

template <typename U1 = SIUnit::Unit<> >
struct Vector2 {
	typedef U1 Unit;
	typedef Vector2<U1> Type;

	Scalar<U1> x, y;

	// Default constructor
	Vector2() {}

	// Constructor from members
	Vector2(Scalar<U1> const& _x, Scalar<U1> const& _y) : x(_x), y(_y) {}

	// Construct from angle
	static Type fromAngle(Scalar<SIUnit::Angle> angle) {
		return Type(cos(angle), sin(angle));
	}

	// Calculate angle
	Scalar<SIUnit::Angle> toAngle() const {
		return atan2(y, x);
	}

	// Special values
	static Type zero() { return Type(Scalar<U1>::zero(), Scalar<U1>::zero()); }
	static Type nan() { return Type(Scalar<U1>::nan(), Scalar<U1>::nan()); }
	static Type inf(Type dir) {
		return Type(Scalar<U1>::inf(dir.x), Scalar<U1>::inf(dir.y));
	}
	static Type infPP() { return Type(Scalar<U1>::infP(), Scalar<U1>::infP()); }
	static Type infPN() { return Type(Scalar<U1>::infP(), Scalar<U1>::infN()); }
	static Type infNP() { return Type(Scalar<U1>::infN(), Scalar<U1>::infP()); }
	static Type infNN() { return Type(Scalar<U1>::infN(), Scalar<U1>::infN()); }
	static Type inf() { return infPP(); }

	// Allow usage in conditions
	typedef bool (Type::*BoolDummyType)();
	operator BoolDummyType () const {
		return (x || y) ? (BoolDummyType) &Type::isZero : 0;
	}
	bool operator ! () const { return !x && !y; }

	// Some meaningful comparisons
	bool operator == (Type const& t) const { return x == t.x && y == t.y; }
	bool operator != (Type const& t) const { return x != t.x || y != t.y; }

	// Checks for special cases
	bool isNaN() const {
		return x.isNaN() || y.isNaN();
	}
	bool isInf() const {
		return x.isInf() || y.isInf();
	}
	bool isZero() const {
		return x.isZero() && y.isZero();
	}

	// Strip unit
	Vector2<> strip() const {
		return Vector2<>(x.strip(), y.strip());
	}

	// Opposite
	Type operator - () const {
		return Type(-x, -y);
	}

	// Addition and subtraction
	Type operator + (Type const& t) const {
		return Type(x + t.x, y + t.y);
	}
	Type operator - (Type const& t) const {
		return Type(x - t.x, y - t.y);
	}

	// Scaling
	template <typename U2>
	Vector2<SIUnitAdd(U1, U2)> operator * (Scalar<U2> const& t) const {
		return Vector2<SIUnitAdd(U1, U2)>(x * t, y * t);
	}
	template <typename U2>
	Vector2<SIUnitSub(U1, U2)> operator / (Scalar<U2> const& t) const {
		return Vector2<SIUnitSub(U1, U2)>(x / t, y / t);
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

	// Dot product
	template <typename U2>
	Scalar<SIUnitAdd(U1, U2)> dot(Vector2<U2> const& t) const {
		return x * t.x + y * t.y;
	}

	// Cross product
	template <typename U2>
	Scalar<SIUnitAdd(U1, U2)> cross(Vector2<U2> const& t) const {
		return x * t.y - t.x * y;
	}

	// Square of length
	Scalar<SIUnitMul(U1, 2)> pow2() const {
		return dot(*this);
	}

	// Length
	Scalar<U1> length() const {
		return sqrt(pow2());
	}

	// Normal
	Type normal() const {
		return *this / length().strip();
	}
};

// Scaling
template <typename U1, typename U2>
inline Vector2<SIUnitAdd(U1, U2)> operator * (Scalar<U1> const& t, Vector2<U2> const& v) {
	return v * t;
}

#endif
