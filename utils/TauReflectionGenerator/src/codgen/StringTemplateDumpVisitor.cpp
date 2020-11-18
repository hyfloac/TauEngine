#include "codegen/StringTemplateDumpVisitor.hpp"
#include "codegen/ast/StringTemplateAST.hpp"
#include "codegen/ast/StringTemplateExprAST.hpp"
#include "codegen/ast/StringTemplateStringifyAST.hpp"
#include "codegen/ast/StringTemplateVarAST.hpp"
#include "codegen/ast/StringTemplateMemberExprAST.hpp"
#include "codegen/ast/StringTemplateBooleanExprAST.hpp"
#include "codegen/ast/StringTemplateFragmentAST.hpp"
#include "codegen/ast/StringTemplateLoopAST.hpp"
#include "codegen/ast/StringTemplateIfAST.hpp"

#include "llvm/Support/raw_ostream.h"

namespace tau { namespace codegen { namespace string {

void StringTemplateDumpVisitor::visit(ast::StringTemplateRootAST& ast) noexcept
{
    _indent = 0;
    _idMap.clear();

    llvm::outs() << "{\n";
    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"nodes\": [\n";

    ++_indent;

    visit(ast.next());

    --_indent;

    printIndent(_indent);
    llvm::outs() << "]\n";

    --_indent;
    llvm::outs() << "}\n\n";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateWildExprAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"WildExpr\",\n";

    printIndent(_indent);
    llvm::outs() << "\"expr\": ";

    visit(ast.expr());
    llvm::outs() << "\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateStringExprAST& ast) noexcept
{
    llvm::outs() << "{\n";
    printIndent(_indent + 1);
    llvm::outs() << "\"type\": \"StringExpr\",\n";
    printIndent(_indent + 1);
    llvm::outs() << R"("value": ")" << ast.string() << "\"\n";
    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateIntegerExprAST& ast) noexcept
{ 
    llvm::outs() << "{\n";
    printIndent(_indent + 1);
    llvm::outs() << "\"type\": \"IntegerExpr\",\n";
    printIndent(_indent + 1);
    llvm::outs() << R"("value": ")" << ast.intVal() << "\"\n";
    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateVarAssignExprAST& ast) noexcept
{
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"VarAssignExpr\",\n";
    printIndent(_indent);
    llvm::outs() << R"("var-name": ")" << ast.varName() << "\",\n";
    llvm::outs() << "\"value\": ";

    visit(ast.assignmentExpr());
    llvm::outs() << "\n";

    --_indent;
    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateVarRetrieveExprAST& ast) noexcept
{ 
    llvm::outs() << "{\n";
    printIndent(_indent + 1);
    llvm::outs() << "\"type\": \"VarRetrieveExpr\",\n";
    printIndent(_indent + 1);
    llvm::outs() << R"("var-name": ")" << ast.varName() << "\"\n";
    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateMemberAssignExprAST& ast) noexcept
{
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"MemberAssignExpr\",\n";

    printIndent(_indent);
    llvm::outs() << "\"object\": ";
    
    visit(ast.objectExpr());

    llvm::outs() << ",\n";

    printIndent(_indent);
    llvm::outs() << R"("member-name": ")" << ast.memberName() << "\",\n";
    
    printIndent(_indent);
    llvm::outs() << "\"assignment\": ";

    visit(ast.assignmentExpr());
    
    llvm::outs() << "\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "}";

}

void StringTemplateDumpVisitor::visit(ast::StringTemplateMemberAccessExprAST& ast) noexcept
{
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"MemberAccessExpr\",\n";

    printIndent(_indent);
    llvm::outs() << "\"object\": ";
    
    visit(ast.objectExpr());

    llvm::outs() << ",\n";

    printIndent(_indent);
    llvm::outs() << R"("member-name": ")" << ast.memberName() << "\"\n";
    
    --_indent;

    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateMethodAccessExprAST& ast) noexcept
{
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"MethodAccessExpr\",\n";

    printIndent(_indent);
    llvm::outs() << "\"object\": ";
    
    visit(ast.objectExpr());

    llvm::outs() << ",\n";

    printIndent(_indent);
    llvm::outs() << R"("member-name": ")" << ast.methodName() << "\",\n";
    
    printIndent(_indent);
    llvm::outs() << "\"arguments\": [\n";

    ++_indent;

    for(uSys i = 0; i < ast.arguments().size(); ++i)
    {
        printIndent(_indent);
        visit(ast.arguments()[i]);

        if(i != ast.arguments().size() - 1)
        {
            llvm::outs() << ",\n";
        }
    }

    llvm::outs() << "\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "]\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateBooleanInvertExprAST& ast) noexcept
{
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"BooleanInvertExpr\",\n";

    printIndent(_indent);
    llvm::outs() << "\"expr\": ";

    visit(ast.expr());

    llvm::outs() << "\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "}";
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateTextBlockAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"TextBlock\",\n";

    printIndent(_indent);
    llvm::outs() << "\"text\": \"elided\"\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateStringifyAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"Stringify\",\n";

    printIndent(_indent);
    llvm::outs() << "\"expr\": ";

    visit(ast.expr());
    llvm::outs() << "\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next()); 
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateVarDeclAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"VarDecl\",\n";

    printIndent(_indent);
    llvm::outs() << R"("var-name": ")" << ast.varName() << "\"\n";

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateBeginFragmentAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"BeginFragment\",\n";

    printIndent(_indent);
    llvm::outs() << R"("fragment-name": ")" << ast.fragmentName() << "\",\n";

    printIndent(_indent);
    llvm::outs() << "\"id\": " << _id << "\n";

    _idMap[&ast] = _id++;

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateEndFragmentAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"EndFragment\",\n";

    printIndent(_indent);
    llvm::outs() << "\"begin-fragment\": ";

    if(ast.begin())
    {
        if(_idMap.count(ast.begin().get()) == 0)
        {
            llvm::outs() << "-1\n";
        }
        else
        {
            llvm::outs() << _idMap[ast.begin().get()] << "\n";
        }
    }
    else
    {
        llvm::outs() << "null\n";
    }

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateBeginLoopAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"BeginLoop\",\n";

    printIndent(_indent);
    llvm::outs() << R"("var-decl": )";

    visit(ast.varDecl());

    printIndent(_indent);
    llvm::outs() << "\"range-expr\": ";

    visit(ast.rangeExpr());

    llvm::outs() << ",\n";

    printIndent(_indent);
    llvm::outs() << "\"id\": " << _id << "\n";

    _idMap[&ast] = _id++;

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next()); 
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateEndLoopAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"EndLoop\",\n";

    printIndent(_indent);
    llvm::outs() << "\"begin-loop-ptr\": ";

    if(ast.begin())
    {
        if(_idMap.count(ast.begin().get()) == 0)
        {
            llvm::outs() << "-1\n";
        }
        else
        {
            llvm::outs() << _idMap[ast.begin().get()] << "\n";
        }
    }
    else
    {
        llvm::outs() << "null\n";
    }

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateLoopControlAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"LoopControl\",\n";

    printIndent(_indent);
    switch(ast.controlType())
    {
        case ast::LoopControlType::Continue:
            llvm::outs() << "\"control-type\": \"Continue\",\n";
            break;
        case ast::LoopControlType::Break:
            llvm::outs() << "\"control-type\": \"Break\",\n";
            break;
        case ast::LoopControlType::LoopIndex:
            llvm::outs() << "\"control-type\": \"LoopIndex\",\n";
            break;
    }

    printIndent(_indent);
    llvm::outs() << "\"loop-index\": " << ast.loopIndex() << ",\n";

    printIndent(_indent);
    llvm::outs() << "\"begin-loop-ptr\": ";

    if(ast.loop())
    {
        if(_idMap.count(ast.loop().get()) == 0)
        {
            llvm::outs() << "-1\n";
        }
        else
        {
            llvm::outs() << _idMap[ast.loop().get()] << "\n";
        }
    }
    else
    {
        llvm::outs() << "null\n";
    }

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateBeginIfAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"BeginIf\",\n";

    printIndent(_indent);
    llvm::outs() << R"("expr": )";

    visit(ast.controlExpr());

    llvm::outs() << ",\n";

    printIndent(_indent);
    llvm::outs() << "\"id\": " << _id << "\n";

    _idMap[&ast] = _id++;

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateElseIfAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"BeginIf\",\n";

    printIndent(_indent);
    llvm::outs() << R"("expr": )";

    visit(ast.controlExpr());

    llvm::outs() << ",\n";
    
    printIndent(_indent);
    llvm::outs() << "\"if-ptr\": ";

    if(ast.begin())
    {
        if(_idMap.count(ast.begin().get()) == 0)
        {
            llvm::outs() << "-1\n";
        }
        else
        {
            llvm::outs() << _idMap[ast.begin().get()] << "\n";
        }
    }
    else
    {
        llvm::outs() << "null\n";
    }

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";


    visit(ast.controlExpr());
    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateElseAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"Else\",\n";

    printIndent(_indent);
    llvm::outs() << "\"if-ptr\": ";

    if(ast.begin())
    {
        if(_idMap.count(ast.begin().get()) == 0)
        {
            llvm::outs() << "-1\n";
        }
        else
        {
            llvm::outs() << _idMap[ast.begin().get()] << "\n";
        }
    }
    else
    {
        llvm::outs() << "null\n";
    }

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::visit(ast::StringTemplateEndIfAST& ast) noexcept
{
    printIndent(_indent);
    llvm::outs() << "{\n";

    ++_indent;

    printIndent(_indent);
    llvm::outs() << "\"type\": \"EndIf\",\n";

    printIndent(_indent);
    llvm::outs() << "\"if-ptr\": ";

    if(ast.begin())
    {
        if(_idMap.count(ast.begin().get()) == 0)
        {
            llvm::outs() << "-1\n";
        }
        else
        {
            llvm::outs() << _idMap[ast.begin().get()] << "\n";
        }
    }
    else
    {
        llvm::outs() << "null\n";
    }

    --_indent;

    printIndent(_indent);
    llvm::outs() << "},\n";

    visit(ast.next());
}

void StringTemplateDumpVisitor::printIndent(const iSys indent) noexcept
{
    for(iSys i = 0; i < indent; ++i)
    {
        llvm::outs() << "    ";
    }
}

} } }
