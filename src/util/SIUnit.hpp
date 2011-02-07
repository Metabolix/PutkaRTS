/*
 * Namespace and class template for SI units.
 *
 * Copyright 2011 Lauri Kenttä
 *
 * This file is part of PutkaRTS.
 *
 * PutkaRTS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PutkaRTS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SIUNIT_HPP
#define _SIUNIT_HPP

namespace SIUnit {
	// Define Unit template.
	template <int _m = 0, int _kg = 0, int _s = 0, int _A = 0, int _K = 0, int _mol = 0, int _cd = 0, bool _error = false>
	struct Unit {
		typedef Unit<_m, _kg, _s, _A, _K, _mol, _cd, _error> Self;
		typedef Unit<-_m, -_kg, -_s, -_A, -_K, -_mol, -_cd, _error> Inverse;
		static const int error = _error;
		static const int m   = _m;
		static const int kg  = _kg;
		static const int s   = _s;
		static const int A   = _A;
		static const int K   = _K;
		static const int mol = _mol;
		static const int cd  = _cd;

		template <typename T> struct Add {
			typedef Unit<_m + T::m, _kg + T::kg, _s + T::s, _A + T::A, _K + T::K, _mol + T::mol, _cd + T::cd, _error || T::error> Self;
		};
		template <typename T> struct Subtract: Add<typename T::Inverse> {};
		template <int n> struct Multiply {
			typedef Unit<_m * n, _kg * n, _s * n, _A * n, _K * n, _mol * n, _cd * n, _error> Self;
		};
		template <int n> struct Divide {
			#define div_error ((_m % n) || (_kg % n) || (_s % n) || (_A % n) || (_K % n) || (_mol % n) || (_cd % n))
			typedef Unit<_m / n, _kg / n, _s / n, _A / n, _K / n, _mol / n, _cd / n, _error || div_error> Self;
			#undef div_error
		};
	};

	// Using SI::add and others is a bit nasty; define shorter alternatives.
	template <typename T, typename U> struct UnitAdd:
		Unit<T::m + U::m, T::kg + U::kg, T::s + U::s, T::A + U::A, T::K + U::K, T::mol + U::mol, T::cd + U::cd, T::error || U::error> {};

	template <typename T, typename U> struct UnitSub:
		UnitAdd<T, typename U::Inverse> {};

	template <typename T, int n> struct UnitMultiply:
		Unit<T::m * n, T::kg * n, T::s * n, T::A * n, T::K * n, T::mol * n, T::cd * n, T::error> {};

	//#define SIUnitAdd(T, U) typename SIUnit::UnitAdd<T, U>::Self
	//#define SIUnitSub(T, U) typename SIUnit::UnitSub<T, U>::Self
	#define SIUnitAdd(T, U) typename T::Self::template Add<typename U::Self>::Self
	#define SIUnitSub(T, U) typename T::Self::template Subtract<typename U::Self>::Self
	#define SIUnitMul(T, n) typename T::Self::template Multiply<n>::Self
	#define SIUnitDiv(T, n) typename T::Self::template Divide<n>::Self

	// ===================================================================
	// Define simple units.
	template <int n = 1> struct m   : public Unit<n, 0, 0, 0, 0, 0, 0> {};
	template <int n = 1> struct kg  : public Unit<0, n, 0, 0, 0, 0, 0> {};
	template <int n = 1> struct s   : public Unit<0, 0, n, 0, 0, 0, 0> {};
	template <int n = 1> struct A   : public Unit<0, 0, 0, n, 0, 0, 0> {};
	template <int n = 1> struct K   : public Unit<0, 0, 0, 0, n, 0, 0> {};
	template <int n = 1> struct mol : public Unit<0, 0, 0, 0, 0, n, 0> {};
	template <int n = 1> struct cd  : public Unit<0, 0, 0, 0, 0, 0, n> {};

	typedef Unit<> Angle;

	typedef m<1>::Self Length;
	typedef m<2>::Self Area;
	typedef m<3>::Self Volume;
	typedef m<1>::Self Position;

	typedef s<1>::Self Time;
	typedef kg<1>::Self Mass;
	typedef K<1>::Self Temperature;
	typedef A<1>::Self Current;
	typedef mol<1>::Self Amount;

	typedef UnitSub<Length, Time>::Self Velocity;
	typedef UnitAdd<Velocity, Mass>::Self Momentum;
	typedef UnitSub<Velocity, Time>::Self Acceleration;
	typedef UnitAdd<Acceleration, Mass>::Self Force;
	typedef UnitAdd<Force, Length>::Self Energy;
	typedef UnitSub<Energy, Time>::Self Power;
	typedef UnitAdd<Mass, m<2>::Self>::Self MomentOfInertia;
	typedef s<-1>::Self AngularVelocity;
	typedef s<-2>::Self AngularAcceleration;
	typedef UnitAdd<Force, Length>::Self Torque;
	typedef UnitAdd<MomentOfInertia, AngularVelocity>::Self AngularMomentum;
}

#endif // _SIUNIT_HPP
