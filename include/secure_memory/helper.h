#ifndef SECUREMEMORY_HELPER_H
#define SECUREMEMORY_HELPER_H

#include <cstring>
#include <functional>
#include "conversions.h"

inline bool comparisonHelper(const void *one, const void *two, uint32_t size) {
    const char *cthis = static_cast<const char *>(one),
            *cother = static_cast<const char *>(two);

    // loop breaks on different character or if size() elements have been checked
    for (; size != 0 && *cthis == *cother; cthis++, cother++, size--);

    return size == 0;       // if they equal, iteration count equals size
}

inline size_t strlen_s(const char *str) {
    if (str == nullptr)
        return 0;
    return strlen(str);
}

namespace secure_memory {
    /**
     * Shortcut template to eliminate need for "using std::swap" before every swap call.
     * @param one
     * @param two
     */
    template<typename T>
    void swap(T &one, T &two) {
        using std::swap;
        swap(one, two);
    }
}


#endif //SECUREMEMORY_HELPER_H
