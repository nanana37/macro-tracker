#include "MyPPCallbacks.h"

MyPPCallbacks::MyPPCallbacks(CompilerInstance &CI) : CI(CI) {}

void MyPPCallbacks::MacroDefined(const Token &MacroNameTok,
                                 const MacroDirective *MD) {
  const IdentifierInfo *II = MacroNameTok.getIdentifierInfo();
  if (!II)
    return;

  const MacroInfo *MI = MD->getMacroInfo();
  if (!MI)
    return;

  llvm::StringRef MacroName = II->getName();

  std::string MacroValue;
  Preprocessor &PP = CI.getPreprocessor();
  for (const Token &Tok : MI->tokens()) {
    llvm::StringRef TokSpelling = PP.getSpelling(Tok);
    MacroValue += TokSpelling.str() + " ";
  }

  llvm::errs() << "[MacroDefined] " << MacroName << " = " << MacroValue << "\n";
}

void MyPPCallbacks::MacroExpands(const Token &MacroNameTok,
                                 const MacroDefinition &MD, SourceRange Range,
                                 const MacroArgs *Args) {
  llvm::errs() << "[MacroExpands] "
               << MacroNameTok.getIdentifierInfo()->getName() << "\n";
}
