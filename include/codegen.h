//
// Created by schrodinger on 1/22/21.
//

#ifndef MICRO_COMPILER_CODEGEN_H
#define MICRO_COMPILER_CODEGEN_H
#include <vcfg/virtual_mips.h>
#include <grammar.h>
#include <sym_table.h>
#include <sstream>
namespace codegen {
    using State = std::string;
    State create_state() {
        static size_t COUNTER = 0;
        return std::to_string(COUNTER++) + std::string { "#INTERNAL_VAL" } ;
    }
    State codegen(const std::shared_ptr<parser::ParseTree>& tree,
                                            vmips::Function &function,
                                            symtable::SymTable<std::shared_ptr<vmips::VirtReg>>&,
                                            const std::shared_ptr<vmips::Function>& read,
                                            const std::shared_ptr<vmips::Function>& write
                                            );
}
#endif //MICRO_COMPILER_CODEGEN_H
