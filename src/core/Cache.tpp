#include "Cache.h"

template<class T>
nafy::Cache<T>::Cache(generate_t gen): gen(gen) {
}
template<class T>
typename
nafy::Cache<T>::data_t nafy::Cache<T>::get() {
    // Need to do it like this instead of checking expired(),
    // because if a weak pointer is set to a r-value shared pointer,
    // the value expires because the r-value shared pointer destructs
    // and weak pointers don't count towards reference counters.
    data_t ptr = data.lock();
    if (!ptr) {
        ptr = gen();
        data = ptr;
    }
    return ptr;
}
