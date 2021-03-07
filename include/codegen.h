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

    /*!
     * Create a new variable state;
     * here we only use sting as identifier.
     * @return A new variable name.
     */
    State create_state();

    /*!
     * Generate code.
     * @param tree root node of the code generation
     * @param function function node in the IR
     * @param read read function node.
     * @param write write function node.
     * @return A new variable represents the value.
     */
    State codegen(const std::shared_ptr <parser::ParseTree> &tree,
                  vmips::Function &function,
                  symtable::SymTable <std::shared_ptr<vmips::VirtReg>> &,
                  const std::shared_ptr <vmips::Function> &read,
                  const std::shared_ptr <vmips::Function> &write
    );
}
#endif //MICRO_COMPILER_CODEGEN_H
