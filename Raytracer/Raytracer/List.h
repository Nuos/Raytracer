#ifndef _LIST_H
#define _LIST_H

template <class Type>

class List
{
public:
	List( int initialCapacity = 256 )
	{
		length = 0;
		capacity = initialCapacity;
		list = new Type[capacity];
	}

	~List()
	{
		length = 0;

		delete[] list;
		list = 0;
	}

	Type& get(int index)
	{
		return list[index];
	}

	void add(Type &item)
	{
		if ( length > capacity - 1 )
		{
			capacity *= 2;
			//std::cout << "fuuuuu ";
			Type *newList = new Type[capacity];

			for ( int i = 0; i < length; i++)
				newList[i] = list[i];

			delete[] list;

			list = newList;
		}

		list[length++] = item;
	}

	bool remove(Type item)
	{
		int index = -1;

		for( int i = 0; i < length; i++ )
		{
			if( item == list[i] )
			{
				index = i;
				length--;
				break;
			}
		}

		if( index > -1 )
		{
			for( int i = index; i < length; i++ )
			{
				list[i] = list[i+1];
			}

			return true;
		}

		return false;
	}

	void removeAt(int index)
	{
	}

	int size()
	{
		return length;
	}

	Type* list;
private:
	int capacity;
	int length;
	
};

#endif
