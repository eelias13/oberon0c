//
// Created by M on 19.01.2025.
//

#include "CodeGenerator.h"
#include "../util/panic.h"
#include <utility>


void CodeGenerator::init_target_machine()
{
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
    if (!target)
    {
        std::cerr << error << std::endl;
    }
    else
    {
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

void CodeGenerator::init_builder()
{
    init_target_machine();
    builder_ = new IRBuilder<>(ctx_);

    if (!target_)
    {
        return;
    }

    // set up LLVM module
    module_ = new Module(filename_, ctx_);
    module_->setDataLayout(target_->createDataLayout());
    module_->setTargetTriple(target_->getTargetTriple().getTriple());
}

CodeGenerator::CodeGenerator(string filename, OutputFileType output_type) : filename_(std::move(filename)), output_type_(output_type)
{
    init_target_machine();
    init_builder();
}

void CodeGenerator::visit(ExpressionNode &node)
{
    switch (node.getNodeType())
    {
    case NodeType::unary_expression:
        visit(dynamic_cast<UnaryExpressionNode &>(node));
        break;
    case NodeType::binary_expression:
        visit(dynamic_cast<BinaryExpressionNode &>(node));
        break;
    case NodeType::ident_selector_expression:
        visit(dynamic_cast<IdentSelectorExpressionNode &>(node));
        break;
    case NodeType::integer:
        visit(dynamic_cast<IntNode &>(node));
        break;
    default:
        return;
    }
}

void CodeGenerator::visit(BinaryExpressionNode &expr)
{

    auto op = expr.get_op();

    expr.get_lhs()->accept(*this);
    llvm::Value *lhsValue = values_.back();
    values_.pop_back();

    // add Short-Circuit Evaluation

    expr.get_rhs()->accept(*this);
    llvm::Value *rhsValue = values_.back();
    values_.pop_back();

    switch (op)
    {

    // math operation
    case SourceOperator::PLUS:
        values_.push_back(builder_->CreateAdd(lhsValue, rhsValue, "add"));
        break;
    case SourceOperator::MINUS:
        values_.push_back(builder_->CreateSub(lhsValue, rhsValue, "sub"));
        break;
    case SourceOperator::MULT:
        values_.push_back(builder_->CreateMul(lhsValue, rhsValue, "mul"));
        break;
    case SourceOperator::DIV:
        values_.push_back(builder_->CreateSDiv(lhsValue, rhsValue, "div"));
        break;

    case SourceOperator::MOD:
        values_.push_back(builder_->CreateSRem(lhsValue, rhsValue, "mod"));
        break;
    // logical operation
    case SourceOperator::OR:
        values_.push_back(builder_->CreateOr(lhsValue, rhsValue, "or"));
        break;

    case SourceOperator::AND:
        values_.push_back(builder_->CreateAnd(lhsValue, rhsValue, "and"));
        break;
    // comp operation
    case SourceOperator::EQ:
        values_.push_back(builder_->CreateICmpEQ(lhsValue, rhsValue, "eq"));
        break;
    case SourceOperator::NEQ:
        values_.push_back(builder_->CreateICmpNE(lhsValue, rhsValue, "neq"));
        break;
    case SourceOperator::LT:
        values_.push_back(builder_->CreateICmpSLT(lhsValue, rhsValue, "lt"));
        break;
    case SourceOperator::LEQ:
        values_.push_back(builder_->CreateICmpSLE(lhsValue, rhsValue, "leq"));
        break;
    case SourceOperator::GT:
        values_.push_back(builder_->CreateICmpSGT(lhsValue, rhsValue, "gt"));
        break;
    case SourceOperator::GEQ:
        values_.push_back(builder_->CreateICmpSGE(lhsValue, rhsValue, "geq"));
        break;

    default:
        panic("Unsupported binary operator");
    }
}

void CodeGenerator::visit(UnaryExpressionNode &expr)

{
    expr.get_expr()->accept(*this);
    llvm::Value *exprValue = values_.back();
    values_.pop_back();

    SourceOperator op = expr.get_op();

    switch (op)
    {
    case SourceOperator::NEG:
        values_.push_back(builder_->CreateNeg(exprValue, "neg"));
        break;

    case SourceOperator::NOT:
        values_.push_back(builder_->CreateNot(exprValue, "not"));
        break;

    default:
        panic("Unsupported unary operator");
    }
}

void CodeGenerator::visit(IdentSelectorExpressionNode &)
{
}

void CodeGenerator::visit(IdentNode &node)
{

    std::string name = node.get_value();
    auto var = variables_.find(name);

    if (var == variables_.end())
    {
        panic("variable is not defined");
    }
    values_.push_back(var->second);
}

void CodeGenerator::visit(IntNode &val)
{
    long int_val = val.get_value();
    llvm::Type *longType = llvm::Type::getInt64Ty(ctx_);
    auto *longValue = llvm::ConstantInt::get(longType, int_val);
    values_.push_back(longValue);
}

void CodeGenerator::visit(SelectorNode &)
{
}

void CodeGenerator::visit(TypeNode &node)
{
    switch (node.getNodeType())
    {
    case NodeType::ident:
        visit(dynamic_cast<IdentNode &>(node));
        break;
    case NodeType::array_type:
        visit(dynamic_cast<ArrayTypeNode &>(node));
        break;
    case NodeType::record_type:
        visit(dynamic_cast<UnaryExpressionNode &>(node));
        break;
    default:
        return;
    }
}

void CodeGenerator::visit(ArrayTypeNode &)
{
}

void CodeGenerator::visit(DeclarationsNode &)
{
}

void CodeGenerator::visit(ProcedureDeclarationNode &)
{
}

void CodeGenerator::visit(RecordTypeNode &)
{
}

void CodeGenerator::visit(StatementNode &node)
{
    switch (node.getNodeType())
    {
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

void CodeGenerator::visit(AssignmentNode &)
{
}

void CodeGenerator::visit(IfStatementNode &)
{
}

void CodeGenerator::visit(ProcedureCallNode &)
{
}

void CodeGenerator::visit(RepeatStatementNode &)
{
}

void CodeGenerator::visit(StatementSequenceNode &node)
{
    auto statements = node.get_statements();
    for (auto itr = statements->begin(); itr != statements->end(); itr++)
    {
        visit(**itr);
    }
}

void CodeGenerator::visit(WhileStatementNode &)
{
}

void CodeGenerator::visit(ModuleNode &node)
{

    // define main
    auto main = module_->getOrInsertFunction(node.get_name().first->get_value(), builder_->getInt32Ty());
    auto main_fct = cast<Function>(main.getCallee());
    auto entry = BasicBlock::Create(builder_->getContext(), "entry", main_fct);

    // global declarations
    visit(*node.get_declarations());

    // statements
    visit(*node.get_statements());
}

void CodeGenerator::emit()
{
    std::string ext;
    switch (output_type_)
    {
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
    if (ec)
    {
        std::cerr << ec.message() << std::endl;
        exit(ec.value());
    }
    if (output_type_ == OutputFileType::LLVMIRFile)
    {
        module_->print(output, nullptr);
        output.flush();
        return;
    }
    CodeGenFileType ft;

    switch (output_type_)
    {
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
    if (target_->addPassesToEmitFile(pass, output, nullptr, ft))
    {
        std::cerr << "Error: target machine cannot emit a file of this type." << std::endl;
        return;
    }
    pass.run(*module_);
    output.flush();
}

void CodeGenerator::generate_code(ModuleNode &node)
{

    visit(node);

    // verify module
    verifyModule(*module_, &errs());

    emit();
}
