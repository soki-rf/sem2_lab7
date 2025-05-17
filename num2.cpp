#include <iostream>
#include <utility>

class MyPoint {
    double x, y;
public:
    MyPoint(double x = 0, double y = 0) : x(x), y(y) {
        std::cout << x << ", " << y << " created\n";
    }

    ~MyPoint() {
        std::cout << x << ", " << y << " destroyed\n";
    }

    void print() const {
        std::cout << x << ", " << y << "\n";
    }

    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }
};

template<class T>
class MyShared {
    T *ptr = nullptr;
    size_t *count = nullptr;

    void release() {
        if (count && --(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

public:
    MyShared(T *p = nullptr) : ptr(p), count(p ? new size_t(1) : nullptr) {}

    ~MyShared() { release(); }

    MyShared(const MyShared &other) : ptr(other.ptr), count(other.count) {
        if (count) ++(*count);
    }

    MyShared &operator=(const MyShared &other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            count = other.count;
            if (count) ++(*count);
        }
        return *this;
    }

    MyShared(MyShared &&other): ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    MyShared &operator=(MyShared &&other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            count = other.count;
            other.ptr = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    T &operator*() const { return *ptr; }

    T *operator->() const { return ptr; }

    operator bool() const { return ptr != nullptr; }
};

template<class T, class... Args>
MyShared<T> Make_MyShared(Args &&... args) {
    return MyShared<T>(new T(std::forward<Args>(args)...));
}

int main() {

    auto p1 = Make_MyShared<MyPoint>(1.5, 2.5);
    p1->print();

    {
        auto p2 = p1;
        p2->move(1, 1);
        p2->print();
    }

    p1->print();

    auto p3 = std::move(p1);
    if (!p1) std::cout << "p1 is empty after move\n";
    p3->print();
}
