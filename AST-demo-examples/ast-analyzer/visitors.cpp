#include "visitors.h"

bool Visitors::VisitRecordDecl(RecordDecl *Declaration) {
    ////Declaration->dump();
    DeclContext *declContext = Declaration;
    std::cout << "Declaration context: " << declContext->getDeclKindName() << "\n";
    std::cout << "Own declaration name: " << Declaration->getNameAsString() << "\n";
    std::cout << "Qualified declaration name: " << Declaration->getQualifiedNameAsString() << "\n";

    FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
    if (FullLocation.isValid()) {
        std::cout << "Found declaration at "
                    << FullLocation.getSpellingLineNumber() << ":"
                    << FullLocation.getSpellingColumnNumber() << "\n";
    }
    
    if(Declaration->hasObjectMember()) {
        std::cout << "Has members\n";
    }
    else {
        std::cout << "Doesn't have members\n";
    }
    return true;
}

bool Visitors::VisitVarDecl(VarDecl *Declaration) {
    Declaration->dump();
    std::cout << "Variable declaration\n";
    auto storageClass = Declaration->getStorageClass();
    if(storageClass == SC_None) {
        std::cout << "Storage class: NONE\n";
    } else {
        std::cout << "Storage class: " << Declaration->getStorageClassSpecifierString(storageClass) << "\n";
    }
    // Проверка на наличие инициализирующей части
    if(Declaration->hasInit()) {
        std::cout << "VarDecl has init part\n";
        //const Expr* expr = Declaration->getAnyInitializer();
        Expr* expr = Declaration->getInit();
    } else {
        std::cout << "VarDecl does not have init part\n";
    }
    // Проверка на наличие инициализирующего выражения
    if(Declaration->getAnyInitializer() != nullptr) {
        std::cout << "VarDecl has AnyInitializer\n";
    } else {
        std::cout << "VarDecl does not have AnyInitializer\n";
    }
    // Проверка на наличие вычисляемого значени
//         if(Declaration->evaluateValue() != nullptr) {
//             std::cout << "VarDecl has evaluateValue\n";
//         } else {
//             std::cout << "VarDecl does not have evaluateValue\n";
//         }
    
    
    ////const char *buf = Declaration->getStorageClassSpecifierString(storageClass);
    ////std::cout << "Storage class: " << Declaration->getStorageClassSpecifierString(storageClass)) << "\n";
    ////std::cout << "Storage class: " << buf << "\n";
    
//         DeclContext *declContext = Declaration;
//         std::cout << "Declaration context: " << declContext->getDeclKindName() << "\n";
//         std::cout << "Own declaration name: " << Declaration->getNameAsString() << "\n";
//         std::cout << "Qualified declaration name: " << Declaration->getQualifiedNameAsString() << "\n";
// 
//         FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
//         if (FullLocation.isValid()) {
//             std::cout << "Found declaration at "
//                         << FullLocation.getSpellingLineNumber() << ":"
//                         << FullLocation.getSpellingColumnNumber() << "\n";
//         }
//      
//         if(Declaration->hasObjectMember()) {
//             std::cout << "Has members\n";
//         }
//         else {
//             std::cout << "Doesn't have members\n";
//         }
    return true;
}

bool Visitors::VisitExpr(Expr *Declaration) {
    Declaration->dump();
    std::cout << "Expression\n";
    
    if(Declaration->isLValue()) {
        std::cout << "  LValue Expression\n";
    }
    else if(Declaration->isRValue()) {
        std::cout << "  RValue Expression\n";
    }

    return true;
}
 
// bool Visitors::VisitFullExpr(FullExpr *Declaration) {
//     Declaration->dump();
//     std::cout << "Full Expression\n";
//     
//     if(Declaration->isLValue()) {
//         std::cout << "  LValue Expression\n";
//     }
//     else if(Declaration->isRValue()) {
//         std::cout << "  RValue Expression\n";
//     }
// 
//     return true;
// }

// bool Visitors::VisitDeclRefExpr(DeclRefExpr *Declaration) {
//     Declaration->dump();
//     std::cout << "DeclRefExpr\n";
//     
//     if(Declaration->isLValue()) {
//         std::cout << "  LValue Expression\n";
//     }
//     else if(Declaration->isRValue()) {
//         std::cout << "  RValue Expression\n";
//     }
// 
//     return true;
// }

bool Visitors::VisitBinaryOperator(BinaryOperator *Declaration) {
    Declaration->dump();
    std::cout << "BinaryOperator\n";
    
    BinaryOperatorKind opc = Declaration->getOpcode();
    std::cout << "  opc = " << Declaration->getOpcodeStr().str() << "\n";
    return true;
}
