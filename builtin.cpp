//
// Created by schrodinger on 1/21/21.
//

#include <builtin.h>

std::shared_ptr<vmips::Function> builtin::add_read(vmips::Module &module) {
    auto scanf = module.create_extern("scanf", 3);
    auto read = module.create_function("__micro_read", 0);
    auto format = read->create_data<asciiz>(true, "%d");
    auto data = read->new_memory(4);
    auto offset = read->append<address>(data);
    auto address = read->append<add>(offset, get_special(SpecialReg::s8));
    auto faddr = read->append<la>(format);
    read->call_void(scanf, faddr, address);
    auto result = read->append<lw>(data);
    read->assign_special(SpecialReg::v0, result);
    return read;
}

std::shared_ptr<vmips::Function> builtin::add_write(vmips::Module &module) {
    auto printf = module.create_extern("printf", 3);
    auto write = module.create_function("__micro_write", 1);
    auto format = write->create_data<asciiz>(true, "%d\n");
    auto faddr = write->append<la>(format);
    write->call_void(printf, faddr, get_special(SpecialReg::a0));
    return write;
}
