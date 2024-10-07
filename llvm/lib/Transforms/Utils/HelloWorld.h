#ifndef REORDER_STRUCT_PASS_H
#define REORDER_STRUCT_PASS_H

#include "llvm/IR/Module.h"
#include "llvm/IR/StructType.h"
#include "llvm/Pass.h"

using namespace llvm;

namespace {
class ReorderStructPass : public ModulePass {
public:
    static char ID; // Pass identification

    // Constructor
    ReorderStructPass() : ModulePass(ID) {}

    // Run the pass on the module
    bool runOnModule(Module &M) override;
};

} // end anonymous namespace

// Register the pass in the LLVM framework
void initializeReorderStructPass(PassRegistry &Registry);

#endif // REORDER_STRUCT_PASS_H

