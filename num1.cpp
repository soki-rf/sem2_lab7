#include <iostream>
#include <utility>


template<class T>
class MyUnique {
    T* p = nullptr;

public:
    explicit MyUnique(T* ptr = nullptr) : p(ptr) {}

    ~MyUnique() {
        delete p;
    }

    MyUnique(const MyUnique&) = delete;

    MyUnique& operator=(const MyUnique&) = delete;

    MyUnique(MyUnique&& other) noexcept : p(other.p) {
        other.p = nullptr;
    }

    MyUnique& operator=(MyUnique&& other) {
        if (this != &other) {
            delete p;
            p = other.p;
            other.p = nullptr;
        }
        return *this;
    }

    T& operator*() {
        return *p;
    }

    T* operator->() {
        return p;
    }
    
    operator bool() const {
        return p != nullptr;
    }
};

template<class T, class... Args>
MyUnique<T> Make_MyUnique(Args&&... args) {
    return MyUnique<T>(new T(std::forward<Args>(args)...));
}

class MyPoint {
    double x, y;
public:
    MyPoint(double x, double y) : x(x), y(y) {
        std::cout << "constructed:" << x << ", " << y << "\n";
    }

    ~MyPoint() {
        std::cout << "destroyed: " << x << ", " << y << "\n";
    }

    void print() const {
        std::cout << x << ", " << y << "\n";
    }

    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }
};

int main() {

    auto p1 = Make_MyUnique<MyPoint>(1.0, 2.0);
    p1->print();

    (*p1).move(0.5, -0.5);
    (*p1).print();

    auto p2 = std::move(p1);
    if (!p1) {
        std::cout << "p1 is empty after move\n";
    }
    p2->print();

    MyUnique<MyPoint> p3;
    p3 = std::move(p2);
    if (!p2) {
        std::cout << "p2 is empty after move\n";
    }
    p3->print();
}
