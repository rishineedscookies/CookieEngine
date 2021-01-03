#pragma once

namespace Cookie
{

	typedef unsigned int size_t;

	template<typename T>
	class Vector
	{
	public:

		Vector();
		explicit Vector(size_t i);

		~Vector();

		// Access
		T& operator[](size_t i) const;

		T* At(size_t i) const;

		bool TryAccess(size_t i, T* out);

		T* GetData();

		// Insertion
		void Insert(T t, size_t i);

		void Add(size_t i);

		// Size and Capacity
		bool Empty() const;

		size_t GetCapacity();

		void Reserve(size_t NewCapacity);

		void Reduce(size_t NewCapacity);


	private:

		T* Arr;
		size_t Capacity;

	};

	template<typename T>
	void Vector<T>::Reduce(size_t NewCapacity)
	{
		if (NewCapacity >= Capacity)
			return;

		T* temp = new T[NewCapacity];
		for(size_t i = 0; i < NewCapacity; i++)
		{
			temp[i] = Arr[i];
		}
		delete[] Arr;
		Arr = temp;
		Capacity = NewCapacity;

	}

	template<typename T>
	bool Vector<T>::TryAccess(size_t i, T* out)
	{
		if (i < Capacity && i >= 0)
			return false;

		out = Arr + i;
		return true;
	}

	template<typename T>
	void Vector<T>::Add(size_t i)
	{
		if (Capacity >= i)
		{
			Reserve(2 * i + 1);
		}
		Arr[i] = *(new T());
	}

	template<typename T>
	void Vector<T>::Insert(T t, size_t i)
	{
		if (Capacity <= i)
		{
			Reserve(2 * i + 1);
		}
		Arr[i] = t;
	}

	template<typename T>
	Vector<T>::Vector()
		: Arr(0), Capacity(0)		
	{}

	template<typename T>
	Vector<T>::Vector(size_t i)
		: Arr(new T[i]), Capacity(i)
	{}

	template<typename T>
	Vector<T>::~Vector()
	{
		delete[] Arr;
	}

	template<typename T>
	T& Vector<T>::operator[](size_t i) const
	{
		return Arr[i];
	}

	template<typename T>
	T* Vector<T>::At(size_t i) const
	{
		CK_CORE_ASSERT(i < Capacity && i >= 0, "Trying to access element out of range");
		return Arr + i;
	}

	template<typename T>
	T* Vector<T>::GetData()
	{
		return Arr;
	}

	template<typename T>
	bool Vector<T>::Empty() const
	{
		return Size == 0;
	}

	template<typename T>
	size_t Vector<T>::GetCapacity()
	{
		return Capacity;
	}

	template<typename T>
	void Vector<T>::Reserve(size_t NewCapacity)
	{
		if (NewCapacity <= Capacity)
			return;

		T* temp = new T[NewCapacity];
		for(size_t i = 0; i < Capacity; i++)
		{
			temp[i] = Arr[i];
		}
		delete[] Arr;
		Arr = temp;
		Capacity = NewCapacity;
	}

}
