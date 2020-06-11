#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

#include <memory>

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

#include "Cache.tpp"

#endif
