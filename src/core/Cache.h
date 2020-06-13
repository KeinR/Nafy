#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include <memory>

/*
* Keeps a weak pointer to some data.
* If that data has expired (all the shared poitners dead),
* Then it calls a given function `gen` to create a new value.
* This effectively allows for retrieving of data when it's needed,
* and deallocation when it's not in use.
*/

namespace nafy {
    template<class T>
    class Cache {
    public:
        typedef std::shared_ptr<T> data_t;
        typedef data_t(*generate_t)();
    private:
        std::weak_ptr<T> data;
        generate_t gen;
    public:
        // `gen` is called to regenerate the cache
        Cache(generate_t gen);
        data_t get();
    };
}

// Dev note: since it's not at the top dep doesn't register Cache.tpp it as a dependency of Cache.h.
// May want to figure out a fix to that. 
#include "Cache.tpp"

#endif
