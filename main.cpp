#include <iostream>
#include <fstream>
#include <grammar.h>
#include <builtin.h>
#include <codegen.h>

const static char HELP[] =
        "USAGE:\n"
        "    compiler <input> [OPTIONS]\n"
        "OPTIONS:\n"
        "    -h,--help           print this help message can return\n"
        "    -o,--output <path>  set output position, use stdout by default\n"
        "    -i,--ir             display/output IR instead of assembly\n"
        "    -a,--ast            display/output AST structure instead of assembly\n"
        "NOTICE:\n"
        "    - IR and AST mode can be set in same time\n"
        "    - if no input file provided, the compiler will use stdin as the input\n";


struct Config {
    bool ir = false;
    bool ast = false;
    bool help = false;
    bool error = false;
    bool has_path = false;
    bool use_stdin = true;
    std::string path;
    std::string input;
    void parse_arguments(int argc, const char **args) {
        bool expect_path = false;
        bool expect_input = true;
        for (int i = 1; i < argc; i += 1) {
            if (!std::strcmp(args[i], "-h") || !std::strcmp(args[i], "--help")) {
                help = true;
            } else if (!std::strcmp(args[i], "-i") || !std::strcmp(args[i], "--ir")) {
                ir = true;
            } else if (!std::strcmp(args[i], "-a") || !std::strcmp(args[i], "--ast")) {
                ast = true;
            } else if (!std::strcmp(args[i], "-o") || !std::strcmp(args[i], "--output")) {
                expect_path = true;
            } else if (expect_path) {
                expect_path = false;
                has_path = true;
                path = args[i];
            } else if (expect_input) {
                input = args[i];
                expect_input = false;
                use_stdin = false;
            }
            else {
                error = true;
            }
        }
        if (expect_path) error = true;
    }

    void handle() {
        std::ostream *out = &std::cout;
        std::istream *in = &std::cin;
        if (error) {
            std::cout << "Invalid Arguments. Please Check Help with -h/--help.\n";
            std::exit(-EINVAL);
        }
        if (help) {
            std::cout << HELP;
            std::exit(0);
        }
        if (has_path) {
            out = new std::ofstream{path};
        }
        if (!use_stdin) {
            in = new std::ifstream {input};
        }
        std::stringstream text{};
        std::string buffer {};
        while (std::getline(*in, buffer)) {
            text << buffer << std::endl;
        }
        auto content = text.str();
        auto tree = grammar::Toplevel().match({
                                                      std::make_shared<parser::MemoTable>(),
                                                      content,
                                                      0,
                                                      0
                                              });
        if (!tree) {
            std::cerr << "syntax error" << std::endl;
            std::exit(1);
        }
        tree = tree->compress<grammar::SelectRule>()[0];
        if (ast) tree->display(*out);
        auto m = vmips::Module("main");
        auto read = builtin::add_read(m);
        auto write = builtin::add_write(m);
        auto func = m.create_function("main", 3);
        symtable::SymTable<std::shared_ptr<vmips::VirtReg>> table{};
        table.enter();
        codegen::codegen(tree, *func, table, read, write);
        func->assign_special(vmips::SpecialReg::v0, 0);
        table.escape();
        if (ir) m.output(*out);
        m.finalize();
        if (!ast && !ir) m.output(*out);
        out->flush();
        if (has_path) {
            delete out;
        }
        if (!use_stdin) {
            delete in;
        }
    }
};


int main(int argc, const char** argv) {
    Config conf;
    conf.parse_arguments(argc, argv);
    conf.handle();
    return 0;
}
