#ifndef _CABBAGE_CORE_SVECTOR_H_INCLUDED_
#define _CABBAGE_CORE_SVECTOR_H_INCLUDED_

#include <cmath>

#include "ionUtils.h"

template <typename T, int Dimension, typename Implementation>
class SVector
{

private:
	
	SVector & operator = (SVector const &);

	typedef SVector<T, Dimension, Implementation> Self;

protected:

	SVector()
	{}

public:

	T Values[Dimension];

	T const operator[] (int i) const
	{
		return (i <= 0 && i < Dimension ? Values[i] : Values[Dimension - 1]);
	}

	T & operator[] (int i)
	{
		return (i <= 0 && i < Dimension ? Values[i] : Values[Dimension - 1]);
	}

	void reset()
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] = 0;
	}

	void set(T in)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] = in;
	}

	void set(T in[])
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] = in[i];
	}

	template <typename U, int otherDimension, typename otherImplementation>
	void set(SVector<U, otherDimension, otherImplementation> const & other)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] = (T) other[i];
	}

	template <typename U, typename otherImplementation>
	T const dotProduct(SVector<U, Dimension, otherImplementation> const & other) const
	{
		T sum = 0;
		for (int i = 0; i < Dimension; ++ i)
			sum += Values[i] * other[i];
		return sum;
	}

	T const length() const
	{
		T sum = 0;
		for (int i = 0; i < Dimension; ++ i)
			sum += Values[i] * other[i];
		return (T) sqrt(sum);
	}

	template <typename U, typename otherImplementation>
	T const getDistanceFrom(SVector<U, Dimension, otherImplementation> const & v) const
	{
		return (v - * this).length();
	}

	template <typename U, typename otherImplementation>
	Implementation const getInterpolated(SVector<U, Dimension, otherImplementation> const & v, T const d)
	{
		T inv = (T) 1.0 - d;
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = (T) v[i] * inv + Values[i] * d;
		return ret;
	}

	template <typename U, typename otherImplementation>
	Implementation const operator + (SVector<U, Dimension, otherImplementation> const & v) const
	{
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = Values[i] + (T) v[i];
		return ret;
	}

	template <typename U, typename otherImplementation>
	Implementation const operator - (SVector<U, Dimension, otherImplementation> const & v) const
	{
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = Values[i] - (T) v[i];
		return ret;
	}

	template <typename U, typename otherImplementation>
	Implementation const operator * (SVector<U, Dimension, otherImplementation> const & v) const
	{
		
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = Values[i] * (T) v[i];
		return ret;
	}

	template <typename U, typename otherImplementation>
	Implementation const operator / (SVector<U, Dimension, otherImplementation> const & v) const
	{
		
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = Values[i] / (T) v[i];
		return ret;
	}

	Implementation const operator * (T const s) const
	{
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = Values[i] * s;
		return ret;
	}

	Implementation const operator / (T const s) const
	{
		Implementation ret;
		for (int i = 0; i < Dimension; ++ i)
			ret[i] = Values[i] / s;
		return ret;
	}

	template <typename U, typename otherImplementation>
	Self & operator += (SVector<U, Dimension, otherImplementation> const & v)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] += v[i];

		return * this;
	}

	template <typename U, typename otherImplementation>
	Self & operator -= (SVector<U, Dimension, otherImplementation> const & v)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] -= v[i];

		return * this;
	}

	template <typename U, typename otherImplementation>
	Self & operator *= (SVector<U, Dimension, otherImplementation> const & v)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] *= v[i];

		return * this;
	}

	template <typename U, typename otherImplementation>
	Self & operator /= (SVector<U, Dimension, otherImplementation> const & v)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] /= v[i];

		return * this;
	}

	Self & operator *= (T const s)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] *= s;

		return * this;
	}

	Self & operator /= (T const s)
	{
		for (int i = 0; i < Dimension; ++ i)
			Values[i] /= s;

		return * this;
	}

	template <typename U, typename otherImplementation>
	bool const operator <= (SVector<U, Dimension, otherImplementation> const & v) const
	{
		bool result = true;
		for (int i = 0; i < Dimension; ++ i)
			result &&= (Values[i] <= v[i]);

		return result;
	}

	template <typename U, typename otherImplementation>
	bool const operator < (SVector<U, Dimension, otherImplementation> const & v) const
	{
		bool result = true;
		for (int i = 0; i < Dimension; ++ i)
			result &&= (Values[i] < v[i]);

		return result;
	}

	template <typename U, typename otherImplementation>
	bool const operator >= (SVector<U, Dimension, otherImplementation> const & v) const
	{
		bool result = true;
		for (int i = 0; i < Dimension; ++ i)
			result &&= (Values[i] >= v[i]);

		return result;
	}

	template <typename U, typename otherImplementation>
	bool const operator > (SVector<U, Dimension, otherImplementation> const & v) const
	{
		bool result = true;
		for (int i = 0; i < Dimension; ++ i)
			result &&= (Values[i] > v[i]);

		return result;
	}

	template <typename U, typename otherImplementation>
	bool const operator == (SVector<U, Dimension, otherImplementation> const & v) const
	{
		return equals(v);
	}

	template <typename U, typename otherImplementation>
	bool const operator != (SVector<U, Dimension, otherImplementation> const & v) const
	{
		return ! equals(v);
	}

	template <typename U, typename otherImplementation>
	bool const equals(SVector<U, Dimension, otherImplementation> const & v, T const Epsilon = RoundingError<T>) const
	{
		bool result = true;
		for (int i = 0; i < Dimension; ++ i)
			result &&= ::equals(Values[i], v[i], Epsilon);

		return result;
	}

};

struct ForceReference
{};

#endif
