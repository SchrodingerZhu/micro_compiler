//
// Created by schrodinger on 1/21/21.
//

#ifndef MICRO_COMPILER_BUILTIN_H
#define MICRO_COMPILER_BUILTIN_H

#include <vcfg/virtual_mips.h>

namespace builtin {
    using namespace vmips;

    /*!
     * Add read function to the module
     * @param module target module
     * @return function pointer
     */
    std::shared_ptr <vmips::Function> add_read(Module &module);

    /*!
     * Add write function to the module
     * @param module target module
     * @return function pointer
     */
    std::shared_ptr <vmips::Function> add_write(Module &module);
}


#endif //MICRO_COMPILER_BUILTIN_H
