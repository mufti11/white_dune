/* https://github.com/skaslev/catmull-clark/buf.h
The MIT License

Copyright (c) 2009-2017 Slavomir Kaslev, https://github.com/skaslev/catmull-clark

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#ifndef BUF_H
#define BUF_H

#include <stdlib.h>

/* Based on Sean Barrett's stretchy buffer at http://www.nothings.org/stb/stretchy_buffer.txt
 * init: NULL, free: buf_free(), push_back: buf_push(), size: buf_len()
 */
#define buf_len(a)         ((a) ? buf_n_(a) : 0)
#define buf_push(a, v)     (buf_maybegrow1(a), (a)[buf_n_(a)++] = (v))
#define buf_last(a)        ((a)[buf_n_(a) - 1])
#define buf_resize(a, n)   (buf_maybegrow(a, n), (a) ? buf_n_(a) = (n) : 0)
#define buf_reserve(a, n)  (buf_maybegrow(a, n))
#define buf_free(a)        ((a) ? free(buf_raw_(a)) : (void) 0)
#define buf_foreach(it, a) for ((it) = (a); (it) < (a) + buf_len(a); (it)++)

/* Private */
#define buf_raw_(a)        ((size_t *) (a) - 2)
#define buf_m_(a)          (buf_raw_(a)[0])
#define buf_n_(a)          (buf_raw_(a)[1])

#define buf_maybegrow(a, n)    (((n) > 0) && (!(a) || (n) >= buf_m_(a)) ? buf_realloc(a, n) : (void) 0)
#define buf_maybegrow1(a)      (!(a) || buf_m_(a) == 0 ? buf_realloc(a, 8) : \
                               buf_n_(a) == buf_m_(a) ? buf_realloc(a, 3 * buf_m_(a) / 2) : (void) 0)
#define buf_realloc(a, n)      buf_do_realloc((void **) &(a), n, sizeof(*(a)))

void buf_do_realloc(void **a, size_t nr, size_t sz);

#endif
