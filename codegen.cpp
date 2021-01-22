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
                table.define(state, val);
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
                std::exit(1);
            }
        }
    }
    CASE(Expr) {
        auto head = codegen(tree->subtrees[0], function, table, read, write);
        if (tree->subtrees.size() > 1) {
            auto value = table(head);
            if (!value) {
                std::cerr << "undefined variable: " << head << std::endl;
                std::exit(1);
            }
            for(size_t i = 2; i < tree->subtrees.size(); i += 2) {
                auto target = codegen(tree->subtrees[i], function, table, read, write);
                auto tmp = table(target);
                if (!tmp) {
                    std::cerr << "undefined variable: " << head << std::endl;
                    std::exit(1);
                }
                switch (tree->subtrees[i - 1]->parsed_region[0]) {
                    case '-':
                        value = function.append<sub>(*value, *tmp);
                        break;
                    case '+':
                        value = function.append<add>(*value, *tmp);
                        break;
                    default:
                        std::cerr << "invalid operator: " << tree->subtrees[i - 1]->parsed_region[0] << std::endl;
                        std::exit(1);
                }
            }
            auto name = create_state();
            table.define(name, *value);
            return name;
        } else {
            return head;
        }
    }

    CASE(Primary) {
        return codegen(tree->subtrees[0], function, table, read, write);
    }

    CASE(Identity) {
        return std::string { tree->parsed_region.begin(), tree->parsed_region.end() };
    }

    CASE(Integer) {
        auto literal = std::stoi ( std::string {tree->parsed_region.begin(), tree->parsed_region.end() });
        auto value = function.append<li>(literal);
        auto name = create_state();
        table.define(name, value);
        return name;
    }

    CASE(Assignment) {
        auto lhs = codegen(tree->subtrees[0], function, table, read, write);
        auto rhs = codegen(tree->subtrees[1], function, table, read, write);
        auto val = table(rhs);
        if (table(lhs)) {
            table.update(lhs, *val);
        } else {
            table.define(lhs, *val);
        }
    }

    return {};
}

codegen::State codegen::create_state() {
    static size_t COUNTER = 0;
    return std::to_string(COUNTER++) + std::string { "#INTERNAL_VAL" } ;
}
