#include <iostream>
#include <grammar.h>
#include <builtin.h>
#include <codegen.h>

auto test = "begin\n"
            "   write(1 + ( 3 - 6));\n"
            "   read(a, b);\n"
            "   a0 := a + 1;\n"
            "   a1 := a + 1;\n"
            "   a2 := a + 1;\n"
            "   a3 := a + 1;\n"
            "   a4 := a + 1;\n"
            "   a5 := a + 1;\n"
            "   a6 := a + 1;\n"
            "   a7 := a + 1;\n"
            "   a8 := a + 1;\n"
            "   a9 := a + 1;\n"
            "   a10 := a + 1;\n"
            "   a11 := a + 1;\n"
            "   a12 := a + 1;\n"
            "   a13 := a + 1;\n"
            "   a14 := a + 1;\n"
            "   a15 := a + 1;\n"
            "   a16 := a + 1;\n"
            "   a17 := a + 1;\n"
            "   a18 := a + 1;\n"
            "   write(a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + 1);\n"
            "end\n";

int main() {
    auto tree = grammar::Toplevel().match({
        std::make_shared<parser::MemoTable>(),
                test,
                0,
                0
    });
    if (!tree) {
        std::cerr << "syntax error" << std::endl;
        std::abort();
    }
    tree = tree->compress<grammar::SelectRule>()[0];
    auto m = vmips::Module("main");
    auto read = builtin::add_read(m);
    auto write = builtin::add_write(m);
    auto func = m.create_function("main", 3);
    symtable::SymTable<std::shared_ptr<vmips::VirtReg>> table {};
    table.enter();
    codegen::codegen(tree, *func, table, read, write);
    table.escape();
    m.output(std::cout);
    m.finalize();
    m.output(std::cout);
    return 0;
}
