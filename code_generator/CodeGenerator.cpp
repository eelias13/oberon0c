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

void CodeGenerator::visit(ExpressionNode& node) {
    switch(node.getNodeType()){
        case NodeType::unary_expression:
            visit(dynamic_cast<UnaryExpressionNode&>(node));
            break;
        case NodeType::binary_expression:
            visit(dynamic_cast<BinaryExpressionNode&>(node));
            break;
        case NodeType::ident_selector_expression:
            visit(dynamic_cast<IdentSelectorExpressionNode&>(node));
            break;
        case NodeType::integer:
            visit(dynamic_cast<IntNode&>(node));
            break;
        default:
            return;
    }
}

void CodeGenerator::visit(BinaryExpressionNode &) {

}

void CodeGenerator::visit(UnaryExpressionNode &) {

}

void CodeGenerator::visit(IdentSelectorExpressionNode &) {

}

void CodeGenerator::visit(IdentNode &) {

}

void CodeGenerator::visit(IntNode &) {

}

void CodeGenerator::visit(SelectorNode &) {

}

void CodeGenerator::visit(TypeNode &node) {
    switch (node.getNodeType()) {
        case NodeType::ident:
            visit(dynamic_cast<IdentNode&>(node));
            break;
        case NodeType::array_type:
            visit(dynamic_cast<ArrayTypeNode&>(node));
            break;
        case NodeType::record_type:
            visit(dynamic_cast<UnaryExpressionNode&>(node));
            break;
        default:
            return;

    }
}

void CodeGenerator::visit(ArrayTypeNode &) {

}

void CodeGenerator::visit(DeclarationsNode &) {

}

void CodeGenerator::visit(ProcedureDeclarationNode &) {

}

void CodeGenerator::visit(RecordTypeNode &) {

}

void CodeGenerator::visit(StatementNode &node) {
    switch (node.getNodeType()) {
        case NodeType::assignment:
            visit(dynamic_cast<AssignmentNode &>(node));
            break;
        case NodeType::if_statement:
            visit(dynamic_cast<IfStatementNode &>(node));
            break;
        case NodeType::procedure_call:
            visit(dynamic_cast<ProcedureCallNode &>(node));
            break;
        case NodeType::repeat_statement:
            visit(dynamic_cast<RepeatStatementNode &>(node));
            break;
        case NodeType::while_statement:
            visit(dynamic_cast<WhileStatementNode &>(node));
            break;
        default:
            return;
    }
}

void CodeGenerator::visit(AssignmentNode &) {

}

void CodeGenerator::visit(IfStatementNode &) {

}

void CodeGenerator::visit(ProcedureCallNode &) {

}

void CodeGenerator::visit(RepeatStatementNode &) {

}

void CodeGenerator::visit(StatementSequenceNode &node) {
    auto statements = node.get_statements();
    for(auto itr = statements->begin(); itr != statements->end(); itr++){
        visit(**itr);
    }
}

void CodeGenerator::visit(WhileStatementNode &) {

}

void CodeGenerator::visit(ModuleNode & node) {

    // define main
    auto main = module_->getOrInsertFunction(node.get_name().first->get_value(), builder_->getInt32Ty());
    auto main_fct = cast<Function>(main.getCallee());
    auto entry = BasicBlock::Create(builder_->getContext(),"entry",main_fct);

    // global declarations
    visit(*node.get_declarations());

    // statements
    visit(*node.get_statements());


}

void CodeGenerator::emit() {
    std::string ext;
    switch (output_type_) {
        case OutputFileType::AssemblyFile:
            ext = ".s";
            break;
        case OutputFileType::LLVMIRFile:
            ext = ".ll";
            break;
        default:
            #if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
            ext = ".obj";
            #else
            ext = ".o";
            #endif
            break;
    }
    std::string file = filename_ + ext;

    // serialize LLVM module to file
    std::error_code ec;
    // open an output stream with open flags "None"
    raw_fd_ostream output(file, ec, llvm::sys::fs::OF_None);
    if (ec) {
        std::cerr << ec.message() << std::endl;
        exit(ec.value());
    }
    if (output_type_ == OutputFileType::LLVMIRFile) {
        module_->print(output, nullptr);
        output.flush();
        return;
    }
    CodeGenFileType ft;

    switch (output_type_) {
        case OutputFileType::AssemblyFile:
            #ifdef _LLVM_18
            ft = CodeGenFileType::AssemblyFile;
            #else
            ft = CodeGenFileType::CGFT_AssemblyFile;
            #endif
            break;
        default:
            #ifdef _LLVM_18
            ft = CodeGenFileType::ObjectFile;
            #else
            ft = CodeGenFileType::CGFT_ObjectFile;
            #endif
            break;
    }

    legacy::PassManager pass;
    if (target_->addPassesToEmitFile(pass, output, nullptr, ft)) {
        std::cerr << "Error: target machine cannot emit a file of this type." << std::endl;
        return;
    }
    pass.run(*module_);
    output.flush();

}

void CodeGenerator::generate_code(ModuleNode &node) {

    visit(node);

    // verify module
    verifyModule(*module_,&errs());

    emit();
}


