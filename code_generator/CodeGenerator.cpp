//
// Created by M on 19.01.2025.
//

#include "CodeGenerator.h"

#include <utility>

void CodeGenerator::init_target_machine(){
    // initialize LLVM
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
    // use default target triple of host
    std::string triple = sys::getDefaultTargetTriple();
    // set up target
    std::string error;
    auto target = TargetRegistry::lookupTarget(triple, error);
    if (!target) {
        std::cerr << error << std::endl;
    } else {
        // set up target machine to match host
        std::string cpu = "generic";
        std::string features;
        TargetOptions opt;

        #ifdef _LLVM_LEGACY
        auto model = llvm::Optional<Reloc::Model>();
        #else
        auto model = std::optional<Reloc::Model>();
        #endif

        target_ = target->createTargetMachine(triple, cpu, features, opt, model);
    }

}

void CodeGenerator::init_builder() {
    init_target_machine();

    // set up a builder to generate the LLVM intermediate representation
    LLVMContext ctx;
    builder_ = new IRBuilder<>(ctx);

    if(!target_){
        return;
    }

    // set up LLVM module
    module_ = new Module(filename_, ctx);
    module_->setDataLayout(target_->createDataLayout());
    module_->setTargetTriple(target_->getTargetTriple().getTriple());


}

CodeGenerator::CodeGenerator(string filename, OutputFileType output_type) : filename_(std::move(filename)), output_type_(output_type) {
    init_target_machine();
    init_builder();
}
