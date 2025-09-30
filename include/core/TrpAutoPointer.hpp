#ifndef AUTOPOINTER_HPP
#define AUTOPOINTER_HPP

#include <cstddef>

// RAII logic for something similar to smartpointer **c++98 is pain**
template <typename T>
class AutoPointer {
    private:
        T* ptr;

        // we do not need copy constructor and copy assignment
        AutoPointer(const AutoPointer& _other);
        AutoPointer& operator=(const AutoPointer& _other);

    public:
        AutoPointer(T* _ptr = NULL) : ptr(_ptr) {
            // nothing goes here for now let see if i can expand this class for future uses
        }

        ~AutoPointer(void) {
            delete ptr;
        }

        bool isNULL(void) const {
            return (ptr == NULL);
        }

        T* release(void) {
            if (!isNULL()) {
                T* tmp = ptr;
                ptr = NULL;
                return tmp;
            }
            return NULL;
        }

        T* get() const {
            return ptr;
        }

        void reset(T* _ptr) {
            if (!isNULL()) {
                delete ptr;
                ptr = _ptr;
            }
            ptr = _ptr;
        }

        T& operator*() const {
            return *ptr;
        }

        T* operator->() const {
            return ptr;
        }
};

// * somehow for static members aka class members you need to do this stupid definition after declaration in class

// !
//! template <typename T>
//! type ClassName<T> = defualt

// * which is nasty

#endif // AUTOPOINTER_HPP