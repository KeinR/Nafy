#include "Cache.h"

template<class T>
nafy::Cache<T>::Cache(generate_t gen): gen(gen) {
}
template<class T>
typename
nafy::Cache<T>::data_t nafy::Cache<T>::get() {
    data_t ptr = data.lock();
    if (!ptr) {
        ptr = gen();
        data = ptr;
    }
    return ptr;
}
