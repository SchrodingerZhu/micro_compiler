#include <iostream>
#include <grammar.h>
#include <builtin.h>


int main() {
    auto m = vmips::Module("test");
    builtin::add_read(m);
    builtin::add_write(m);
    m.finalize();
    m.output(std::cout);
    return 0;
}
