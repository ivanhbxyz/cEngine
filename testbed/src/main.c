#include <core/logger.h>
#include <core/asserts.h>

int main(void) {
    int val = 3;
    int valu = 19;

    KFATAL("A test message: %f", 3.14f);
    KERROR("A test message: %f", 3.14f);
    KWARN("A test message: %f", 3.14f);
    KINFO("A test message: %f", 3.14f);
    KDEBUG("A test message: %f", 3.14f);
    KTRACE("A test message: %f", 3.14f);
    valu = valu * 3;
    KASSERT(1 == 0);

    return 0;
}