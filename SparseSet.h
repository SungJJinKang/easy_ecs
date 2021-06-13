#pragma once

#include <cassert>
#include <memory>
#include <vector>

namespace ecs
{

	template <typename T, typename Allocator = std::allocator<T>>
	class SparseSet
	{
	private:

		std::vector<size_t> mSparse;
		std::vector<T, Allocator> mDense;
		

		size_t mSize;
		size_t mCapacity;

		void _reserve(size_t size)
		{
			Allocator::
			this->mDense.reserve(size);
			this->mSparse.reserve(size);

			this->mCapacity = size;
		}

		void _swap(size_t i, size_t j)
		{
			size_t tempIndex = this->mSparse[i];
			this->mSparse[i] = this->mSparse[j];
			this->mSparse[j] = tempIndex;

			T tempValue = std::move(this->mDense[i]);
			this->mDense[i] = std::move(this->mDense[j]);
			this->mDense[j] = std::move(tempValue);
		}

	public:

		SparseSet()
			:mSize(0), mCapacity(0), mSparse(), mDense()
		{

		}

		bool empty()
		{
			return this->mSize == 0;
		}

		size_t size()
		{
			return this->mSize;
		}

		void reserve(size_t size)
		{
			this->_reserve(size);
		}

		bool has(const T& value)
		{
			for (size_t i = 0; i < this.mSize; i++)
			{
				if (mDense[i] == value)
				{
					return true;
				}
			}

			return false;
		}

		size_t insert(const T& value)
		{
			if (this->mCapacity < this->mSize + 1)
			{
				this->_reserve(this->mCapacity * 2 == 0 ? 1 : this->mCapacity * 2);
			}

			if (this->mDense.size() < this->mSize)
			{
				this->mDense.push_back(value);
			}
			else
			{
				this->mDense.push_back(value);
			}
			this->mDense[mSize] = value;
			this->mSparse[mSize] = this->mSize;

			mSize++;
			return this->mSize - 1;
		}

		size_t insert(T&& value)
		{
			if (this->mCapacity < this->mSize + 1)
			{
				this->_reserve(this->mCapacity * 2 == 0 ? 1 : this->mCapacity * 2);
			}

			this->mDense[mSize] = std::move(value);
			this->mSparse[mSize] = this->mSize;

			this->mSize++;
			return this->mSize - 1;
		}

		void erase(const T& value)
		{
			assert(this->mSize > 0);
			for (size_t i = 0; i < this->mSize; i++)
			{
				if (this->mDense[this->mSparse[i]] == value)
				{
					if (i != this->mSize - 1)
					{
						//swap and pop_back trick
						this->_swap(i, this->mSize - 1);
					}
					this->mSize--;
					return;
				}
			}
			
		}

		T operator[](size_t i)
		{
			assert(i >= 0 && i < this->mSize);
			return this->mDense[this->mSparse[i]];
		}

		const T& operator[](size_t i) const
		{
			assert(i >= 0 && i < this->mSize);
			return this->mDense[this->mSparse[i]];
		}

	};

	void test()
	{
		SparseSet<int> gg{};

		for (int i = 10; i <= 20; i++)
		{
			gg.insert(i);
		}

		for (int i = 20; i >= 10; i--)
		{
			gg.erase(i);
		}

		for (int i = 50; i <= 60; i++)
		{
			gg.insert(i);
		}

		for (int i = 50; i <= 55; i++)
		{
			gg.erase(i);
		}
		
	}

	
}

