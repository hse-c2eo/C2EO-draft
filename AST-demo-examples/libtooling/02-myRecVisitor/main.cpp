/*
 * https://clang.llvm.org/docs/RAVFrontendAction.html
 * How to write RecursiveASTVisitor based ASTFrontendActions
 * using default file test.cpp added
 */


#include <iostream>
#include <fstream>
#include <sstream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class FindNamedClassVisitor
  : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  explicit FindNamedClassVisitor(ASTContext *Context)
    : Context(Context) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
      Declaration->dump();
      ///DeclContext *declContext = Declaration;
      ///std::cout << declContext->getDeclKindName() << "\n";
      //std::cout << Declaration->getQualifiedNameAsString() << "\n";
      if (Declaration->getQualifiedNameAsString() == "n::m::C") {
          FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
          if (FullLocation.isValid())
              //llvm::outs() << "Found declaration at "
              std::cout << "Found declaration at "
                        << FullLocation.getSpellingLineNumber() << ":"
                        << FullLocation.getSpellingColumnNumber() << "\n";
      }
      return true;
  }

private:
  ASTContext *Context;
};

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
  explicit FindNamedClassConsumer(ASTContext *Context)
    : Visitor(Context) {}

  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  FindNamedClassVisitor Visitor;
};

class FindNamedClassAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::make_unique<FindNamedClassConsumer>(&Compiler.getASTContext());
  }
};

int main(int argc, char **argv) {
    if (argc > 1) {
        clang::tooling::runToolOnCode(std::make_unique<FindNamedClassAction>(), argv[1]);
        return 0;
    }
    // Компилируемый модуль читается из файла в строку
    std::string fileName{"../test.cpp"};
    std::ifstream moduleStream(fileName);
    if(moduleStream.fail()) {
        std::cout << "Module" << fileName << " isn't accesible" << std::endl;
        return 1;
    }
    std::stringstream ss;
    ss << moduleStream.rdbuf();
    std::string cModule(ss.str()); // Модуль языка Си (в перспективе)
    /// Тестовый вывод прочитанного артефакта
    std::cout << cModule << std::endl;
    clang::tooling::runToolOnCode(std::make_unique<FindNamedClassAction>(), cModule);
    return 0;
}
