//
// Created by brahan on 23/01/2020.
//

#ifndef CPP_EX3_HASHMAP_HPP
#define CPP_EX3_HASHMAP_HPP

#include <vector>
#include <stdexcept>

using std::vector;
#define INVALID "Invalid input"
#define LOWER_THRESHOLD 0.25
#define UPPER_THRESHOLD 0.75
#define BASIC_TABLE_SIZE 16
#define EMPTY 0
#define INVALID_IDX -1
#define TO_DELETE 0
#define TO_ADD 1
#define MIN_CAPACITY 1
#define RESIZE_FACTOR 2

//class MemoryAllocationException : public std::exception //TODO we dont need it?
//{
//    const char *what() const noexcept override
//    {
//        return "Memory not allocated";
//    }
//} MemoryAllocationException;

/**
 * template for the hash map
 * @tparam KeyT defines the keys in the hashmap
 * @tparam ValueT defines the values in teh hashmap
 */
template<typename KeyT, typename ValueT>
class HashMap
{
    typedef std::pair<KeyT, ValueT> cell;
    typedef std::vector<std::pair<KeyT, ValueT>> bucket;

public:
    /**
     * inner class that represent a iterator
     */
    class iterator
    {
    public:
        // iterator typedef, needed for iterator after cpp14
        typedef cell value_type;

        typedef cell *pointer;

        typedef cell &reference;

        typedef int difference_type;

        typedef std::forward_iterator_tag iterator_category;

        /**
         * default constructor
         */
        iterator() = default;

        /**
         * default destructor
         */
        ~iterator() = default;


        /**
         * iterator that gets map
         * @param map
         */
        explicit iterator(const HashMap<KeyT, ValueT> *map, int curBucket = 0) : _map(map), _curBucket(curBucket)
        {
            if (map != nullptr)
            {
                curIdx = 0;
                if (curIdx == (int) _map->_table[_curBucket].size())
                {
                    _current = nullptr;
                    operator++();
                }
                _current = &(_map->_table[_curBucket][curIdx]);
            }
            else
            {
                curIdx = 0;
                _current = nullptr;
            }

        }

        //operators: //TODO iterator lecture slides 15
        reference operator*() const
        {
            return *_current;
        }

        pointer operator->() const
        {
            return _current;
        }

        /**
         * ++ after
         * @return
         */
        iterator &operator++()
        {
            ++curIdx;
            while (curIdx > (((int) _map->_table[_curBucket].size()) - 1) && _curBucket < ((int) _map->capacity() - 1))
            {
                curIdx = 0;
                _curBucket++;
            }
            if (_curBucket == ((int) _map->capacity() - 1))
            {
                _current = nullptr;
                return (*this);
            }
            _current = &(_map->_table[_curBucket][curIdx]);
            return (*this);
        }

        /**
         * ++ before
         * @return this after the increment
         */
        iterator operator++(int)
        {
            iterator tmp = *this;
            operator++();
            return tmp;
        } //tODO done

        /**
         * comparison operator
         * @param other other iterator
         * @return true if the the current cell of both iterators are equal
         */
        bool operator==(const iterator &other) const
        {
            return _current == other._current;

        }//TODO done
        /**
         * comparison operator
         * @param other other iterator
         * @return true if the the current cell of both iterators are not equal
         */
        bool operator!=(const iterator &other) const
        {
            return !(other == *this);
        } //TODO done

        /**
         * assignment operator for the iterator
         * @param other the other iterator
         * @return the iterator after the assignment
         */
        iterator &operator=(const iterator &other)
        {
            this->map = *other.map;
            this->bucketIdx = other.bucketIdx;
            this->_current = other._current;
            return *this;
        }

    private:
        const HashMap<KeyT, ValueT> *_map;
        int _curBucket, curIdx;
        cell *_current;
    };

private:
    bucket *_table;
    size_t _size, _capacity;

    /**
     *  calculate the key hash value== its bucket
     * @param key the key
     * @return the key hash value
     */
    int _hash(const KeyT &key) const
    {
        return (std::hash<KeyT>{}(key) & (_capacity - 1));
    }

    /**
     * the function with search for a key and return its index in the map
     * @param key the key
     * @return the key place inside its bucket
     */
    int _containsHelper(const KeyT &key) const
    {
        int bucketIdx = _hash(key);
        for (int i = 0; i < (int) _table[bucketIdx].size(); ++i)
        { //iterate once on all the bucket --> O(n)
            if (_table[bucketIdx].at(i).first == key)
            {
                return i;
            }
        }
        return INVALID_IDX;
    }

    /**
     * helper for constructor, operators =\[] insert without checking if the key is inside the table
     * @param k key
     * @param v value
     */
    void _insertWithDupes(KeyT k, ValueT v)
    {
        ++_size;
        int idx = _containsHelper(k);
        int bucketIdx = (std::hash<KeyT>{}(k) & (capacity() - 1));
        if (idx == INVALID_IDX)
        {
            std::pair<KeyT, ValueT> nP(k, v);
            _table[bucketIdx].push_back(nP);
        }
        else
        {
            _table[bucketIdx][idx].second = v;
        }
    }

    /**
     * adds items from other map with dupes
     * @param old old hashmap
     */
    void _addingLoop(HashMap<KeyT, ValueT> &old)
    {
        for (int i = 0; i < old.capacity(); ++i)
        {
            if (!old._table[i].empty())
            {
                for (auto &pair:old._table[i])
                {
                    _insertWithDupes(pair.first, pair.second);
                }
            }
        }
        old.clear();
    }

    /**
     * resizs the hashmap
     * @param rehashBigger 1 if we need to resize to a bigger hashmap, 0 otherwize
     */
    void _resize(int rehashBigger)
    {
        double modifiedLoad = 0;
        if (rehashBigger == TO_ADD)
        {
            modifiedLoad = (double) (_size + 1) / (double) _capacity;
            if (UPPER_THRESHOLD < modifiedLoad)
            {
                HashMap<KeyT, ValueT> old(*this);
                _capacity *= RESIZE_FACTOR;
                _size = 0;
                delete[] _table;
                _table = new bucket[_capacity];
                _addingLoop(old);
            }
        }
        else if (rehashBigger == TO_DELETE)
        {
            modifiedLoad = (double) (_size) / (double) _capacity;
            if (LOWER_THRESHOLD > modifiedLoad && _capacity > MIN_CAPACITY)
            {
                HashMap<KeyT, ValueT> old(*this);
                _capacity /= RESIZE_FACTOR;
                _size = 0;
                delete[] _table;
                _table = new bucket[_capacity];
                _addingLoop(old);
            }
        }
    }

public:

    /**
     * init empty hashmap
     */
    HashMap() : _table(new bucket[BASIC_TABLE_SIZE]), _size(EMPTY), _capacity(BASIC_TABLE_SIZE)
    {

    }

    /**
     * init hashmap from vectors and keys
     * @param keys keys vector
     * @param values values vector
     */
    HashMap(vector<KeyT> keys, vector<ValueT> values) : _size(EMPTY), _capacity(BASIC_TABLE_SIZE)
    {
        if (keys.size() != values.size()) // ||keys.capacity()!=values.capacity()
        {
            throw std::invalid_argument("constructor");
        }
        _table = new std::vector<std::pair<KeyT, ValueT>>[_capacity];
        for (int i = 0; i < (int) keys.size(); ++i)
        {
            KeyT k = keys[i];
            ValueT v = values[i];
            _resize(TO_ADD);
            int place = _containsHelper(k);
            int bucketIdx = (std::hash<KeyT>{}(k) & (_capacity - 1));
            if (place == INVALID_IDX)
            {
                std::pair<KeyT, ValueT> nP(k, v);
                _table[bucketIdx].push_back(nP);
                ++_size;
            }
            else
            {
                _table[bucketIdx][place].second = v;
//                ++_size;
            }
        }
    }

    /**
     * copy constructor
     * @param other the other hashmap
     */
    HashMap(HashMap<KeyT, ValueT> &other) : _size(other._size), _capacity(other._capacity)
    {
        _table = new bucket[other._capacity];
        for (int i = 0; i < (int) other._capacity; ++i)
        {
            _table[i] = other._table[i];
        }
    }

    /**
     * destructor
     */
    ~HashMap()
    {
        delete[]_table;
    }

    /**
     * getter for the number of element in the hashmap
     * @return the hashmap size
     */
    int size() const
    {
        return (int) _size;
    }

    /**
     * getter for the table capacity
     * @return the hashmap capacity
     */
    int capacity() const
    {
        return (int) _capacity;
    }

    /**
     * return true if the hashmap is empty
     * @return true if empty, false otherwise
     */
    bool empty() const
    {
        return (_size == 0);
    }

    /**
     * insert a pair into the hashmap
     * @return true if the value have been succsefuly inserted , false otherwise
     */
    bool insert(const KeyT key, const ValueT value)
    {
        if (_containsHelper(key) == INVALID_IDX)
        {
            _resize(TO_ADD);
            int place = (std::hash<KeyT>{}(key) & (capacity() - 1));
            std::pair<KeyT, ValueT> nP(key, value);
            _table[place].push_back(nP);
            ++_size;
            return true;
        }
        return false;
    }

    /**
     * checks if a key is inside the table
     * @param key the key we want to search for
     * @return true if the key is in the table, false otherwise
     */
    bool containsKey(KeyT key) const
    {
        return (_containsHelper(key) != INVALID_IDX); // On
    }

    /**
     * returns the value of the given key in the table
     * @param key the key we want its value
     * @return the key value
     */
    ValueT &at(KeyT key) const
    {
        int bucketIdx = _hash(key);
        int keyIdx = _containsHelper(key); //On
        if (keyIdx != INVALID_IDX)
        {
            return _table[bucketIdx][keyIdx].second;
        }
        else
        {
            throw std::invalid_argument("the key not in map - at()");
        }
    }

    /**
     * delete the key value from the table
     * @param key the key
     * @return true if the value successfully deleted, false otherwise
     */
    bool erase(KeyT key)
    {
        int bucketIdx = _hash(key);
        int keyIdx = _containsHelper(key); //On
        if (keyIdx != INVALID_IDX)
        {
            for (typename std::vector<std::pair<KeyT, ValueT>>::const_iterator it = _table[bucketIdx].begin();
                 it != _table[bucketIdx].end(); it++)
            {
                if (it->first == key)
                {
                    _table[bucketIdx].erase(it);
                    --_size;
                    _resize(TO_DELETE);
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * return the table load factor
     * @return the table for the load factor
     */
    double getLoadFactor() const
    {
        return (double) _size / (double) _capacity;
    }

    /**
     * gets a key and returns his bucket size
     * @param key the key
     * @return the key bucket size
     */
    int bucketSize(KeyT key) const
    {
        int keyIdx = _containsHelper(key); //On
        if (keyIdx != INVALID_IDX)
        {
            int bucketIdx = _hash(key);
            return _table[bucketIdx].size();
        }
        else
        {
            throw std::invalid_argument("the key not in map - bucketSize()");
        }
    }

    /**
     * gets a key and returns his bucket index
     * @param key the key
     * @return the key bucket index
     */
    int bucketIndex(KeyT key) const
    {
        int keyIdx = _containsHelper(key); //On
        if (keyIdx != INVALID_IDX)
        {
            int bucketIdx = _hash(key);
            return bucketIdx;
        }
        else
        {
            throw std::invalid_argument("the key not in map - at()");
        }
    }

    /**
     * delete all the map items without changing the capacity
     */
    void clear()
    {
        for (int i = 0; i < this->capacity(); ++i)
        {
            _table[i].clear();
        }
        _size = 0;
    }
/// operators:
    /**
     * random access operator for the map
     * @param k the key we want to get its value
     * @return the key value
     */
    ValueT &operator[](const KeyT &k) noexcept
    {
        int bucketIdx = (std::hash<KeyT>{}(k) & (capacity() - 1));
        int place = _containsHelper(k);
        if (place == INVALID_IDX)
        {
            _resize(TO_ADD);
            ValueT v = ValueT();
            _insertWithDupes(k, v);
            place = _containsHelper(k);
            bucketIdx = (std::hash<KeyT>{}(k) & (capacity() - 1));
        }
        return _table[bucketIdx][place].second;

    }

    /**
     * random access operator for the map
     * @param k the key we want to get its value
     * @return the key value
     */
    ValueT &operator[](const KeyT &k) const noexcept
    {
        int place = _containsHelper(k);
        int bucketIdx = (std::hash<KeyT>{}(k) & (capacity() - 1));
        if (place == INVALID_IDX)
        {
            //DO None
        }
        return _table[bucketIdx][place].second;
    }

    /**
     * comparison operator for the map
     * @param other the other map
     * @return true if the maps are equals
     */
    bool operator==(const HashMap<KeyT, ValueT> &other) const
    {
        if (this->size() != other.size())
        {
            return false;
        }
        for (int i = 0; i < capacity(); ++i)
        {
            for (int j = 0; j < (int) _table[i].size(); ++j)
            {
                if (_table[i].at(j) != other._table[i].at(j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * comparison operator for the map
     * @param other the other map
     * @return true if the maps are equals
     */
    bool operator!=(const HashMap<KeyT, ValueT> &other) const
    {
        return !(*this == other);
    }

    /**
     * assignment operator for the map
     * @param other other map
     * @return the map after the assignment
     */
    HashMap<KeyT, ValueT> &operator=(const HashMap<KeyT, ValueT> &other)
    {
        this->_size = other._size;
        this->_capacity = other._capacity;
        _table = new std::vector<std::pair<KeyT, ValueT>>[_capacity];
        for (int i = 0; i < _capacity; ++i)
        {
            _table[i] = other._table[i];
        }
        return *this;
    }

    /**
     * begin
     * @return iterator that point to the first item of the map
     */
    const iterator begin() const
    {
        return (iterator(this, 0));
    }

    /**
     * const version of begin
     * @return iterator that point to the first item of the map
     */
    iterator cbegin() const
    {
        return begin();
    }

    /**
     * end
     * @return iterator that point to the end of the map
     */
    iterator end() const
    {
        return (iterator(nullptr));
    }

    /**
     * const version of end
     * @return iterator that point to the end of the map
     */
    iterator cend() const
    {
        return end();
    }
};

#endif //CPP_EX3_HASHMAP_HPP
