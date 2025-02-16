#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

// (1) PPCallbacks を継承するクラス
class MyPPCallbacks : public PPCallbacks {
public:
  explicit MyPPCallbacks(CompilerInstance &CI) : CI(CI) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
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

    llvm::errs() << "[MacroDefined] " << MacroName << " = " << MacroValue
                 << "\n";
  }

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    llvm::errs() << "[MacroExpands] "
                 << MacroNameTok.getIdentifierInfo()->getName() << "\n";
  }

private:
  CompilerInstance &CI;
};

// (2) PluginASTAction を継承したクラス
namespace {
class MyPPCallbacksAction : public PluginASTAction {
protected:
  // ここで ASTConsumer を返す（今回は何もしないので空）
  std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, llvm::StringRef InFile) override {
    return std::make_unique<ASTConsumer>();
  }

  // プラグイン引数のパース（必要ないなら空でOK）
  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }

  // (3) プラグインのエントリーポイント：BeginSourceFileAction
  //     ソースファイル処理を始めるときに呼ばれるので、ここで PPCallbacks
  //     を登録する
  bool BeginSourceFileAction(CompilerInstance &CI) override {
    // Preprocessor を取得
    Preprocessor &PP = CI.getPreprocessor();
    // addPPCallbacks で自作の PPCallbacks を登録
    PP.addPPCallbacks(std::make_unique<MyPPCallbacks>(CI));
    return true;
  }

  // 実際の AST 解析を走らせる（特に何もしないので親クラスの実装を呼ぶだけ）
  void ExecuteAction() override { PluginASTAction::ExecuteAction(); }
};
} // namespace

// (4) PluginASTAction を Clang に登録
static FrontendPluginRegistry::Add<MyPPCallbacksAction>
    X("my-ppcallback-plugin", "Example plugin with PPCallbacks");
