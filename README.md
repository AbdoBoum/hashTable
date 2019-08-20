# Implementation of a HashTable

### HashTable 
A HashTable is a key -> value lookup. We can think of it as an array of "buckets", each of which  store a key-value pairs. To locate the "bucket" where a key-value pair is stored, the key is passed throught a **hashing function** which returns ans integers which represents the index of the pairs in the array of "buckets".

### Functions
- ht_hash_table* ht_new(void): It creates an empty hash table having the initial default capacity.
- ht_hash_table* ht_new_sized(const int base_size): It creates an empty hashtable of a certain size/capacity.
- void ht_insert(ht_hash_table*, const char*, const char*): It inserts the specified key-value in the hash table.
- void ht_insert_item(ht_hash_table*, ht_item*): It inserts the specified item in the hash table.
- char* ht_get(ht_hash_table*, const char*): Returns the string that contains the value associated with the key.
- void ht_delete(ht_hash_table*, const char*): It removes the specified values which is associated whith specified key from the hash table.
- bool ht_isEmpty(ht_hash_table*): Returns a boolean indicating weather the hash table is empty or not.
- bool ht_containsKey(ht_hash_table*, const char*): Check if the hash table contains some key equal to specified key.
- int ht_size(ht_hash_table*): Returns the size of the hash table.
- void ht_replace(ht_hash_table*, const char*, const char*): Replaces the old value with the new value for a specified key.

### The Structures
To store our key-value pairs we'll create a new structure:

```
typedef struct {
   char* key;
   char* value;
 } ht_item;
```
and the structure of the hash table will store the size of the table, the base size, the count of elements in the table and an array of pointers to items:

```
typedef struct {
  int base_size;
  int size;
  int count;
  ht_item** items;
} ht_hash_table;
```

Before start the real work we have to implement some basics function for creating new items, deleting items... Then we have to create our hash function which must returns an even distribution of indexes to avoid leading to a higher rate of collisions. So, our hash function take a string as input and output an integer between 0 and the size of the hash table.

The algorithm for the used hash function is show bellow: 

```
function hash(String string, int a, int size):
    hash = 0
    string_len = length(string)
    for i = 0, 1, ..., string_len:
        hash += (a ** (string_len - (i+1))) * char_code(string[i])
    hash = hash % size
    return hash
```

We take hash % size to reduce the size of the integer to the fixed range. When 2 differents key lead to a same index, it causes a collision. So we must deal with those collisions. In our case we will use [Double hashing](https://www.geeksforgeeks.org/double-hashing/) to calculate the new index. There is other solution to deal with collisions: 

- [Separate chaining](https://www.geeksforgeeks.org/hashing-set-2-separate-chaining/) : each bucket contains a linked list. When items collide, they are added to the list.
- [Linear probing](https://www.geeksforgeeks.org/hashing-set-3-open-addressing/): When a collision happen, we continue incrementing the index by 1 until we found an empty bucket to store our item.
- [Quadratic  probing](https://www.geeksforgeeks.org/hashing-set-3-open-addressing/): When a collision happen, we continue incrementing the index by i² until we found an empty bucket to store our item. i = 0,1,...
- Double hashing : We'll make use of a second hash function which should not return 0. Double hashing is a common method of collision management in production hash tables. ``` index = (hash_a(string) + i * (hash_b(string) + 1)) % num_buckets ```.

### Resizing the hash table
When first our hash table is created it have an initial default capacity. As we add more items the table starts to fill up ==> higher rates of collisions. Also we can only store a fixed number of items, and so if we try to add more items the ht_insert will fail.

To deal with that we have to increase the size of our hash table before it gets full. We'll resize the table depending on the load factor (count / size), ratio of filled buckets to total buckets. We'll double the size of the hash table if load > 0.7, and decrease it to half if load < 0.1.

To resize, we create a new hash table roughly half or twice as big as the current, and insert all non-deleted items into it.

Our new array size should be a prime number roughly double or half the current size. Finding the new array size isn't trivial. To do so, we store a base size, which we want the array to be, and then define the actual size as the first prime larger than the base size. To resize up, we double the base size, and find the first larger prime, and to resize down, we halve the size and find the next larger prime.

Credits to : [jamesroutley](https://github.com/jamesroutley/write-a-hash-table).

