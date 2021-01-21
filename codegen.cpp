//
// Created by schrodinger on 1/22/21.
//

#include "codegen.h"
#include <iostream>
using namespace vmips;
using namespace parser;
using namespace grammar;


#define CASE(TYPE) \
    if (tree->instance == typeid(TYPE))

codegen::State codegen::codegen(const std::shared_ptr<ParseTree> &tree,
                                          vmips::Function &function,
                                          symtable::SymTable<std::shared_ptr<vmips::VirtReg>> &table,
                                          const std::shared_ptr<vmips::Function>& read,
                                          const std::shared_ptr<vmips::Function>& write
) {
    CASE(Toplevel) {
        for (auto &i : tree->subtrees) {
            codegen(i, function, table, read, write);
        }
    }
    CASE(ReadStmt) {
        for (auto &i : tree->subtrees) {
            auto state = codegen(i, function, table, read, write);
            auto val = function.call(read);
            if(!table.update(state, val)) {
                std::cerr << "undefined variable: " << state << std::endl;
                std::abort();
            }
        }
    }
    CASE(WriteStmt) {
        for (auto &i : tree->subtrees) {
            auto state = codegen(i, function, table, read, write);
            auto reg = table(state);
            if (reg) {
                function.call_void(write, *reg);
            } else {
                std::cerr << "undefined variable: " << state << std::endl;
                std::abort();
            }
        }
    }
    CASE(Expr) {
        auto head = codegen(tree->subtrees[0], function, table, read, write);
        if (tree->subtrees.size() > 1) {
            auto value = table(head);
            for(size_t i = 2; i < tree->subtrees.size(); ++i) {
                auto target = codegen(tree->subtrees[i], function, table, read, write);
                auto tmp = table(target);
                switch (tree->subtrees[i - 1]->parsed_region[0]) {
                    case '-':
                        value = function.append<sub>(*value, *tmp);
                        break;
                    case '+':
                        value = function.append<add>(*value, *tmp);
                        break;
                    default:
                        std::cerr << "invalid operator" << std::endl;
                        std::abort();
                }
            }
            auto name = create_state();
            table.define(name, *value);

        } else {
            return head;
        }
    }

    return {};
}
