#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/StructType.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/HelloWorld.h"

using namespace llvm;

namespace {
struct ReorderStructPass : public ModulePass {
  static char ID;
  ReorderStructPass() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    bool modified = false;

    for (StructType *ST : M.getIdentifiedStructTypes()) {
      if (ST->getNumElements() == 3) {
        auto *LongTy = Type::getInt64Ty(M.getContext());
        auto *IntTy = Type::getInt32Ty(M.getContext());
        auto *CharTy = Type::getInt8Ty(M.getContext());

        // Track indices of the required types
        int charIndex = -1, intIndex = -1, longIndex = -1;

        // Find the indices of char, int, and long in the struct
        for (unsigned i = 0; i < 3; ++i) {
          Type *ElementType = ST->getElementType(i);
          if (ElementType == CharTy) charIndex = i;
          else if (ElementType == IntTy) intIndex = i;
          else if (ElementType == LongTy) longIndex = i;
        }

        // Check if all types are found and need reordering
        if (charIndex != -1 && intIndex != -1 && longIndex != -1) {
          // If not already in the order char, int, long, reorder
          if (!(charIndex == 0 && intIndex == 1 && longIndex == 2)) {
            std::vector<Type *> ReorderedFields = {CharTy, IntTy, LongTy};
            ST->setBody(ReorderedFields, false);

            errs() << "Reordered struct: " << ST->getName() << "\n";
            modified = true;
          }
        }
      }
    }

    // Print whether the module was modified
    errs() << "Modified: " << (modified ? "true" : "false") << "\n";
    return modified;
  } //closes runOnMod
}; //closes Pass
} // closes namespace

char ReorderStructPass::ID = 0;
static RegisterPass<ReorderStructPass> X("reorder-struct",
                                         "Reorder struct fields to char, int, long",
                                         false, false);

